/*
  +----------------------------------------------------------------------+
  | simdjson_php                                                         |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.0 of the Apache license,    |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.apache.org/licenses/LICENSE-2.0.html                      |
  +----------------------------------------------------------------------+
  | Author: Jinxi Wang  <1054636713@qq.com>                              |
  +----------------------------------------------------------------------+
*/


extern "C" {
#include <ext/spl/spl_exceptions.h>
#include <Zend/zend_exceptions.h>
#include "php.h"
#include "php_simdjson.h"
}

#include "simdjson.h"
#include "bindings.h"

#if PHP_VERSION_ID < 70300
#define zend_string_release_ex(s, persistent) zend_string_release((s))
#endif

#define SIMDJSON_DEPTH_CHECK_THRESHOLD 100000

static void simdjson_throw_jsonexception(simdjson::error_code error)
{
    zend_throw_exception(simdjson_exception_ce, simdjson::error_message(error), (zend_long) error);
}

static inline simdjson::simdjson_result<simdjson::dom::element>
get_key_with_optional_prefix(simdjson::dom::element &doc, std::string_view json_pointer)
{
    auto std_pointer = (json_pointer.empty() ? "" : "/") + std::string(json_pointer.begin(), json_pointer.end());
    return doc.at_pointer(std_pointer);
}

static simdjson::error_code
build_parsed_json_cust(simdjson::dom::parser& parser, simdjson::dom::element &doc, const char *buf, size_t len, bool realloc_if_needed,
                       size_t depth = simdjson::DEFAULT_MAX_DEPTH) {
    if (UNEXPECTED(depth > SIMDJSON_DEPTH_CHECK_THRESHOLD) && depth > len && depth > parser.max_depth()) {
        /*
         * Choose the depth in a way that both avoids frequent reallocations
         * and avoids excessive amounts of wasted memory beyond multiples of the largest string ever decoded.
         *
         * If the depth is already sufficient to parse a string of length `len`,
         * then use the parser's previous depth.
         *
         * Precondition: depth > len
         * Postcondition: depth <= original_depth && depth > len
         */
        if (len < SIMDJSON_DEPTH_CHECK_THRESHOLD) {
            depth = SIMDJSON_DEPTH_CHECK_THRESHOLD;
        } else if (depth > len * 2) {
            // In callers, simdjson_validate_depth ensures depth <= SIMDJSON_MAX_DEPTH (which is <= SIZE_MAX/8),
            // so len * 2 is even smaller than the previous depth and won't overflow.
            depth = len * 2;
        }
    }
    auto error = parser.allocate(len, depth);

    if (error) {
        return error;
    }

    error = parser.parse(buf, len, realloc_if_needed).get(doc);
    if (error) {
        return error;
    }

    return simdjson::SUCCESS;
}

static zend_always_inline void simdjson_set_zval_to_string(zval *v, const char *buf, size_t len) {
    /* In php 7.1, the ZSTR_CHAR macro doesn't exist, and CG(one_char_string)[chr] may or may not be null */
#if PHP_VERSION_ID >= 70200
    if (len <= 1) {
        /*
        A note on performance benefits of the use of interned strings here and elsewhere:

        - PHP doesn't need to allocate a temporary string and initialize it
        - PHP doesn't need to free the temporary string
        - PHP doesn't need to compute the hash of the temporary string
        - Memory usage is reduced because the string representation is reused
        - String comparisons are faster when the strings are the exact same pointer.
        - CPU caches may already have this interned string
        - If all array keys are interned strings, then php can skip the step of
          freeing array keys when garbage collecting the array.
         */
        zend_string *key = len == 1 ? ZSTR_CHAR(buf[0]) : ZSTR_EMPTY_ALLOC();
        ZVAL_INTERNED_STR(v, key);
        return;
    }
#endif
    ZVAL_STRINGL(v, buf, len);
}

static zend_always_inline void simdjson_add_key_to_symtable(HashTable *ht, const char *buf, size_t len, zval *value) {
#if PHP_VERSION_ID >= 70200
    if (len <= 1) {
        /* Look up the interned string (i.e. not reference counted) */
        zend_string *key = len == 1 ? ZSTR_CHAR(buf[0]) : ZSTR_EMPTY_ALLOC();
        /* Add the key or update the existing value of the key. */
        zend_symtable_update(ht, key, value);
        /* zend_string_release_ex is a no-op for interned strings */
        return;
    }
#endif
    zend_string *key = zend_string_init(buf, len, 0);
    zend_symtable_update(ht, key, value);
    /* Release the reference counted key */
    zend_string_release_ex(key, 0);
}

static zend_always_inline void simdjson_set_zval_to_int64(zval *zv, const int64_t value) {
#if SIZEOF_ZEND_LONG < 8
    if (value != (zend_long)value) {
        ZVAL_DOUBLE(zv, value);
        return;
    }
#endif
    ZVAL_LONG(zv, value);
}

static zval create_array(simdjson::dom::element element) /* {{{ */ {
    zval v;

    switch (element.type()) {
        //ASCII sort
        case simdjson::dom::element_type::STRING :
            simdjson_set_zval_to_string(&v, element.get_c_str().value_unsafe(), element.get_string_length().value_unsafe());
            break;
        case simdjson::dom::element_type::INT64 :
            simdjson_set_zval_to_int64(&v, element.get_int64().value_unsafe());
            break;
            /* UINT64 is used for positive values exceeding INT64_MAX */
        case simdjson::dom::element_type::UINT64 : ZVAL_DOUBLE(&v, (double)element.get_uint64().value_unsafe());
            break;
        case simdjson::dom::element_type::DOUBLE : ZVAL_DOUBLE(&v, element.get_double().value_unsafe());
            break;
        case simdjson::dom::element_type::BOOL :
            ZVAL_BOOL(&v, element.get_bool().value_unsafe());
            break;
        case simdjson::dom::element_type::NULL_VALUE :
            ZVAL_NULL(&v);
            break;
        case simdjson::dom::element_type::ARRAY : {
            const auto json_array = element.get_array().value_unsafe();
#if PHP_VERSION_ID >= 70300
            if (json_array.size() == 0) {
                /* Reuse the immutable empty array to save memory */
                ZVAL_EMPTY_ARRAY(&v);
                break;
            }
#endif
            zend_array *arr;
            array_init(&v);
            arr = Z_ARR(v);

            for (simdjson::dom::element child : json_array) {
                zval value = create_array(child);
                zend_hash_next_index_insert(arr, &value);
            }

            break;
        }
        case simdjson::dom::element_type::OBJECT : {
            const auto json_object = element.get_object().value_unsafe();
#if PHP_VERSION_ID >= 70300
            if (json_object.size() == 0) {
                /* Reuse the immutable empty array to save memory */
                ZVAL_EMPTY_ARRAY(&v);
                break;
            }
#endif
            zend_array *arr;
            array_init(&v);
            arr = Z_ARR(v);

            for (simdjson::dom::key_value_pair field : json_object) {
                zval value = create_array(field.value);
                /* TODO consider using zend_string_init_existing_interned in php 8.1+ to save memory and time freeing strings. */
                simdjson_add_key_to_symtable(arr, field.key.data(), field.key.size(), &value);
            }
            break;
        }
        EMPTY_SWITCH_DEFAULT_CASE();
    }

    return v;
}

/* }}} */

static zval create_object(simdjson::dom::element element) /* {{{ */ {
    zval v;

    switch (element.type()) {
        //ASCII sort
        case simdjson::dom::element_type::STRING :
            simdjson_set_zval_to_string(&v, element.get_c_str().value_unsafe(), element.get_string_length().value_unsafe());
            break;
        case simdjson::dom::element_type::INT64 :
            simdjson_set_zval_to_int64(&v, element.get_int64().value_unsafe());
            break;
            /* UINT64 is used for positive values exceeding INT64_MAX */
        case simdjson::dom::element_type::UINT64 : ZVAL_DOUBLE(&v, (double)element.get_uint64().value_unsafe());
            break;
        case simdjson::dom::element_type::DOUBLE : ZVAL_DOUBLE(&v, element.get_double().value_unsafe());
            break;
        case simdjson::dom::element_type::BOOL :
            ZVAL_BOOL(&v, element.get_bool().value_unsafe());
            break;
        case simdjson::dom::element_type::NULL_VALUE :
            ZVAL_NULL(&v);
            break;
        case simdjson::dom::element_type::ARRAY : {
            const auto json_array = element.get_array().value_unsafe();
#if PHP_VERSION_ID >= 70300
            if (json_array.size() == 0) {
                /* Reuse the immutable empty array to save memory */
                ZVAL_EMPTY_ARRAY(&v);
                break;
            }
#endif
            zend_array *arr;
            array_init(&v);
            arr = Z_ARR(v);

            for (simdjson::dom::element child : json_array) {
                zval value = create_object(child);
                zend_hash_next_index_insert(arr, &value);
            }
            break;
        }
        case simdjson::dom::element_type::OBJECT : {
            const auto json_object = element.get_object().value_unsafe();
            object_init(&v);
#if PHP_VERSION_ID >= 80000
            zend_object *obj = Z_OBJ(v);
#endif

            for (simdjson::dom::key_value_pair field : json_object) {
                const char *data = field.key.data();
                size_t size = field.key.size();
				/* PHP 7.1 allowed using the empty string as a property of an object */
                if (UNEXPECTED(data[0] == '\0') && (PHP_VERSION_ID < 70100 || UNEXPECTED(size > 0))) {
                    if (!EG(exception)) {
                        zend_throw_exception(spl_ce_RuntimeException, "Invalid property name", 0);
                    }
                    return v;
                }
                zval value = create_object(field.value);

                /* Add the key to the object */
#if PHP_VERSION_ID >= 80000
                zend_string *key;
                if (size <= 1) {
                    key = size == 1 ? ZSTR_CHAR(data[0]) : ZSTR_EMPTY_ALLOC();
                } else {
                    key = zend_string_init(data, size, 0);
                }
                zend_std_write_property(obj, key, &value, NULL);
                zend_string_release_ex(key, 0);
#else

# if PHP_VERSION_ID >= 70200
                if (size <= 1) {
                    zval zkey;
                    zend_string *key = size == 1 ? ZSTR_CHAR(data[0]) : ZSTR_EMPTY_ALLOC();
                    ZVAL_INTERNED_STR(&zkey, key);
                    zend_std_write_property(&v, &zkey, &value, NULL);
                } else
# endif
                {
                    zval zkey;
                    ZVAL_STRINGL(&zkey, data, size);
                    zend_std_write_property(&v, &zkey, &value, NULL);
                    zval_ptr_dtor_nogc(&zkey);
                }
#endif
                /* After the key is added to the object (incrementing the reference count) ,
                 * decrement the reference count of the value by one */
                zval_ptr_dtor_nogc(&value);
            }
            break;
        }
        EMPTY_SWITCH_DEFAULT_CASE();
    }
    return v;
}


/* }}} */

simdjson::dom::parser* cplus_simdjson_create_parser(void) /* {{{ */ {
    return new simdjson::dom::parser();
}

void cplus_simdjson_free_parser(simdjson::dom::parser* parser) /* {{{ */ {
    delete parser;
}

bool cplus_simdjson_is_valid(simdjson::dom::parser& parser, const char *json, size_t len, size_t depth) /* {{{ */ {
    simdjson::dom::element doc;
    /* The depth is passed in to ensure this behaves the same way for the same arguments */
    auto error = build_parsed_json_cust(parser, doc, json, len, true, depth);
    if (error) {
        return false;
    }
    return true;
}

/* }}} */

void cplus_simdjson_parse(simdjson::dom::parser& parser, const char *json, size_t len, zval *return_value, unsigned char assoc, size_t depth) /* {{{ */ {
    simdjson::dom::element doc;
    auto error = build_parsed_json_cust(parser, doc, json, len, true, depth);
    if (error) {
        simdjson_throw_jsonexception(error);
        return;
    }

    if (assoc) {
        *return_value = create_array(doc);
    } else {
        *return_value = create_object(doc);
    }
}
/* }}} */
void cplus_simdjson_key_value(simdjson::dom::parser& parser, const char *json, size_t len, const char *key, zval *return_value, unsigned char assoc,
                              size_t depth) /* {{{ */ {
    simdjson::dom::element doc;
    simdjson::dom::element element;
    auto error = build_parsed_json_cust(parser, doc, json, len, true, depth);
    if (error) {
        simdjson_throw_jsonexception(error);
        return;
    }

    error = get_key_with_optional_prefix(doc, key).get(element);

    if (error) {
        simdjson_throw_jsonexception(error);
        return;
    }

    if (assoc) {
        *return_value = create_array(element);
    } else {
        *return_value = create_object(element);
    }
}

/* }}} */

u_short cplus_simdjson_key_exists(simdjson::dom::parser& parser, const char *json, size_t len, const char *key, size_t depth) /* {{{ */ {
    simdjson::dom::element doc;
    auto error = build_parsed_json_cust(parser, doc, json, len, true, depth);
    if (error) {
        return SIMDJSON_PARSE_KEY_NOEXISTS;
    }
    error = get_key_with_optional_prefix(doc, key).error();
    if (error) {
        return SIMDJSON_PARSE_KEY_NOEXISTS;
    }
    return SIMDJSON_PARSE_KEY_EXISTS;
}

/* }}} */


void cplus_simdjson_key_count(simdjson::dom::parser& parser, const char *json, size_t len, const char *key, zval *return_value, size_t depth) /* {{{ */ {
    simdjson::dom::element doc;
    simdjson::dom::element element;

    auto error = build_parsed_json_cust(parser, doc, json, len, true, depth);
    if (error) {
        simdjson_throw_jsonexception(error);
        return;
    }

    error = get_key_with_optional_prefix(doc, key).get(element);
    if (error) {
        simdjson_throw_jsonexception(error);
        return;
    }

    zend_long key_count;
    switch (element.type()) {
        //ASCII sort
        case simdjson::dom::element_type::ARRAY : {
            auto json_array = element.get_array().value_unsafe();
            key_count = zend_long(json_array.size());
            if (UNEXPECTED(key_count == 0xFFFFFF)) {
                /* The C simdjson library represents array sizes larger than 0xFFFFFF as 0xFFFFFF. */
                key_count = 0;
                for (auto it: json_array)  {
                    key_count++;
                }
                ZEND_ASSERT(key_count >= 0xFFFFFF);
            }
            break;
        }
        case simdjson::dom::element_type::OBJECT : {
            auto json_object = element.get_object().value_unsafe();
            key_count = zend_long(json_object.size());
            if (UNEXPECTED(key_count == 0xFFFFFF)) {
                /* The C simdjson library represents object sizes larger than 0xFFFFFF as 0xFFFFFF. */
                key_count = 0;
                for (auto it: json_object) {
                    key_count++;
                }
                ZEND_ASSERT(key_count >= 0xFFFFFF);
            }
            break;
        }
        default:
            key_count = 0;
            break;
    }
    ZVAL_LONG(return_value, key_count);
}

/* }}} */
