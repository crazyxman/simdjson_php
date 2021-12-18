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


#include <ext/spl/spl_exceptions.h>
#include <Zend/zend_exceptions.h>
#include "php.h"
#include "php_simdjson.h"

#include "simdjson.h"
#include "bindings.h"

#if PHP_VERSION_ID < 70300
#define zend_string_release_ex(s, persistent) zend_string_release((s))
#endif
// see https://github.com/simdjson/simdjson/blob/master/doc/performance.md#reusing-the-parser-for-maximum-efficiency
simdjson::dom::parser parser;

static inline simdjson::simdjson_result<simdjson::dom::element>
get_key_with_optional_prefix(simdjson::dom::element &doc, std::string_view json_pointer)
{
    auto std_pointer = (json_pointer.empty() ? "" : "/") + std::string(json_pointer.begin(), json_pointer.end());
    return doc.at_pointer(std_pointer);
}

static simdjson::error_code
build_parsed_json_cust(simdjson::dom::element &doc, const char *buf, size_t len, bool realloc_if_needed,
                       size_t depth = simdjson::DEFAULT_MAX_DEPTH) {
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

/* }}} */

static zval create_array(simdjson::dom::element element) /* {{{ */ {
    zval v;

    switch (element.type()) {
        //ASCII sort
        case simdjson::dom::element_type::STRING :
            ZVAL_STRINGL(&v, element.get_c_str(), element.get_string_length());
            break;
        case simdjson::dom::element_type::INT64 : ZVAL_LONG(&v, int64_t(element));
            break;
        case simdjson::dom::element_type::UINT64 : ZVAL_LONG(&v, uint64_t(element));
            break;
        case simdjson::dom::element_type::DOUBLE : ZVAL_DOUBLE(&v, double(element));
            break;
        case simdjson::dom::element_type::BOOL :
            ZVAL_BOOL(&v, bool(element));
            break;
        case simdjson::dom::element_type::NULL_VALUE :
            ZVAL_NULL(&v);
            break;
        case simdjson::dom::element_type::ARRAY : {
            const auto json_array = simdjson::dom::array(element);
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
            const auto json_object = simdjson::dom::object(element);
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
                zend_string *key = zend_string_init(field.key.data(), field.key.size(), 0);
                zend_symtable_update(arr, key, &value);
                zend_string_release_ex(key, 0);
            }
            break;
        }
        default:
            break;
    }

    return v;
}

/* }}} */

static zval create_object(simdjson::dom::element element) /* {{{ */ {
    zval v;

    switch (element.type()) {
        //ASCII sort
        case simdjson::dom::element_type::STRING :
            ZVAL_STRINGL(&v, element.get_c_str(), element.get_string_length());
            break;
        case simdjson::dom::element_type::INT64 : ZVAL_LONG(&v, int64_t(element));
            break;
        case simdjson::dom::element_type::UINT64 : ZVAL_LONG(&v, uint64_t(element));
            break;
        case simdjson::dom::element_type::DOUBLE : ZVAL_DOUBLE(&v, double(element));
            break;
        case simdjson::dom::element_type::BOOL :
            ZVAL_BOOL(&v, bool(element));
            break;
        case simdjson::dom::element_type::NULL_VALUE :
            ZVAL_NULL(&v);
            break;
        case simdjson::dom::element_type::ARRAY : {
            const auto json_array = simdjson::dom::array(element);
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
            object_init(&v);
#if PHP_VERSION_ID >= 80000
            zend_object *obj = Z_OBJ(v);
#endif

            for (simdjson::dom::key_value_pair field : simdjson::dom::object(element)) {
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
#if PHP_VERSION_ID >= 80000
                /* TODO consider using zend_string_init_existing_interned in php 8.1+ to save memory and time freeing strings. */
                zend_string *key = zend_string_init(data, size, 0);
                obj->handlers->write_property(obj, key, &value, NULL);
                zend_string_release_ex(key, 0);
#else
                add_property_zval_ex(&v, data, size, &value);
#endif
                zval_ptr_dtor_nogc(&value);
            }
            break;
        }
        default:
            break;
    }
    return v;
}


/* }}} */

bool cplus_simdjson_is_valid(const char *json, size_t len) /* {{{ */ {
    simdjson::dom::element doc;
    auto error = parser.parse(json, len).get(doc);
    if (error) {
        return false;
    }
    return true;
}

/* }}} */

void cplus_simdjson_parse(const char *json, size_t len, zval *return_value, unsigned char assoc, size_t depth) /* {{{ */ {
    simdjson::dom::element doc;
    auto error = build_parsed_json_cust(doc, json, len, true, depth);
    if (error) {
        zend_throw_exception(spl_ce_RuntimeException, simdjson::error_message(error), 0);
        return;
    }

    if (assoc) {
        *return_value = create_array(doc);
    } else {
        *return_value = create_object(doc);
    }
}
/* }}} */
void cplus_simdjson_key_value(const char *json, size_t len, const char *key, zval *return_value, unsigned char assoc,
                              size_t depth) /* {{{ */ {
    simdjson::dom::element doc;
    simdjson::dom::element element;
    auto error = build_parsed_json_cust(doc, json, len, true, depth);
    if (error) {
        zend_throw_exception(spl_ce_RuntimeException, simdjson::error_message(error), 0);
        return;
    }

    error = get_key_with_optional_prefix(doc, key).get(element);

    if (error) {
        zend_throw_exception(spl_ce_RuntimeException, simdjson::error_message(error), 0);
        return;
    }

    if (assoc) {
        *return_value = create_array(element);
    } else {
        *return_value = create_object(element);
    }
}

/* }}} */

u_short cplus_simdjson_key_exists(const char *json, size_t len, const char *key, size_t depth) /* {{{ */ {
    simdjson::dom::element doc;
    auto error = build_parsed_json_cust(doc, json, len, true, depth);
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


void cplus_simdjson_key_count(const char *json, size_t len, const char *key, zval *return_value, size_t depth) /* {{{ */ {
    simdjson::dom::element doc;
    simdjson::dom::element element;

    auto error = build_parsed_json_cust(doc, json, len, true, depth);
    if (error) {
        zend_throw_exception(spl_ce_RuntimeException, simdjson::error_message(error), 0);
        return;
    }

    error = get_key_with_optional_prefix(doc, key).get(element);
    if (error) {
        zend_throw_exception(spl_ce_RuntimeException, simdjson::error_message(error), 0);
        return;
    }

    zval v;
    switch (element.type()) {
        //ASCII sort
        case simdjson::dom::element_type::ARRAY : ZVAL_LONG(&v, uint64_t(simdjson::dom::array(element).size()));
            break;
        case simdjson::dom::element_type::OBJECT : ZVAL_LONG(&v, uint64_t(simdjson::dom::object(element).size()));
            break;
        default: ZVAL_LONG(&v, 0);
            break;
    }
    *return_value = v;
}

/* }}} */
