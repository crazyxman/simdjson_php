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

// see https://github.com/simdjson/simdjson/blob/master/doc/performance.md#reusing-the-parser-for-maximum-efficiency
simdjson::dom::parser parser;

WARN_UNUSED
simdjson::dom::element build_parsed_json_cust(const uint8_t *buf, size_t len, bool realloc_if_needed, u_short depth = simdjson::DEFAULT_MAX_DEPTH) {
    simdjson::dom::element doc;
    simdjson::error_code error = parser.allocate(len, depth);

    if (error) {
        std::cerr << "failure during memory allocation " << std::endl;
    } else {
        try {
            doc = parser.parse(buf, len, realloc_if_needed);
        } catch (simdjson::simdjson_error &e) {
            zend_throw_exception(spl_ce_RuntimeException, e.what(), 0 );
            doc = parser.parse("{}"_padded);
        }
    }

    return doc;
}


static bool simdjsonphp::is_valid(const std::string& p) /* {{{ */ {
    simdjson::dom::element doc;
    auto error = parser.parse(p).get(doc);

    if (error) {
        return false;
    }
    return true;
}

/* }}} */

bool cplus_simdjson_is_valid(const char *json) /* {{{ */ {
    return simdjsonphp::is_valid(json);
}

/* }}} */

static void simdjsonphp::parse(const std::string& p, zval *return_value, unsigned char assoc, u_short depth) /* {{{ */ {

    simdjson::dom::element doc = build_parsed_json_cust(reinterpret_cast<const uint8_t *>(p.data()), p.length(), true, depth);

    if (assoc) {
        *return_value = simdjsonphp::make_array(doc);
    } else {
        *return_value = simdjsonphp::make_object(doc);
    }
}

/* }}} */

void cplus_simdjson_parse(const char *json, zval *return_value, unsigned char assoc, u_short depth) /* {{{ */ {
    simdjsonphp::parse(json, return_value, assoc, depth);
}

/* }}} */

static zval simdjsonphp::make_array(simdjson::dom::element element) /* {{{ */ {
    zval v;

    switch (element.type()) {
        //ASCII sort
        case simdjson::dom::element_type::STRING :
            ZVAL_STRING(&v, std::string_view(element).data());
            break;
        case simdjson::dom::element_type::INT64 :
        ZVAL_LONG(&v,  int64_t(element));
            break;
        case simdjson::dom::element_type::UINT64 :
        ZVAL_LONG(&v,  uint64_t(element));
            break;
        case simdjson::dom::element_type::DOUBLE :
        ZVAL_DOUBLE(&v, double(element));
            break;
        case simdjson::dom::element_type::BOOL :
            ZVAL_BOOL(&v, bool(element));
            break;
        case simdjson::dom::element_type::NULL_VALUE :
            ZVAL_NULL(&v);
            break;
        case simdjson::dom::element_type::ARRAY :
            zval arr;
            array_init(&arr);

            for (simdjson::dom::element child : simdjson::dom::array(element)) {
                zval value = simdjsonphp::make_array(child);
                add_next_index_zval(&arr, &value);
            }

            v = arr;
            break;
        case simdjson::dom::element_type::OBJECT :
            zval obj;
            array_init(&obj);

            for (simdjson::dom::key_value_pair field : simdjson::dom::object(element)) {
                zval value = simdjsonphp::make_array(field.value);
                add_assoc_zval_ex(&obj, field.key.data(), strlen(field.key.data()), &value);
            }
            v = obj;
            break;
        default:
            break;
    }

    return v;
}

/* }}} */

static zval simdjsonphp::make_object(simdjson::dom::element element) /* {{{ */ {
    zval v;

    switch (element.type()) {
        //ASCII sort
        case simdjson::dom::element_type::STRING :
            ZVAL_STRING(&v, std::string_view(element).data());
            break;
        case simdjson::dom::element_type::INT64 :
            ZVAL_LONG(&v,  int64_t(element));
            break;
        case simdjson::dom::element_type::UINT64 :
            ZVAL_LONG(&v,  uint64_t(element));
            break;
        case simdjson::dom::element_type::DOUBLE :
            ZVAL_DOUBLE(&v, double(element));
            break;
        case simdjson::dom::element_type::BOOL :
            ZVAL_BOOL(&v, bool(element));
            break;
        case simdjson::dom::element_type::NULL_VALUE :
            ZVAL_NULL(&v);
            break;
        case simdjson::dom::element_type::ARRAY :
            zval arr;
            array_init(&arr);

            for (simdjson::dom::element child : simdjson::dom::array(element)) {
                zval value = simdjsonphp::make_object(child);
                add_next_index_zval(&arr, &value);
            }

            v = arr;
            break;
        case simdjson::dom::element_type::OBJECT :
            zval obj;
            object_init(&obj);

            for (simdjson::dom::key_value_pair field : simdjson::dom::object(element)) {
                zval value = simdjsonphp::make_object(field.value);
                add_property_zval_ex(&obj, field.key.data(), strlen(field.key.data()), &value);
            }
            v = obj;
            break;
        default:
            break;
    }
    return v;
}

/* }}} */

void cplus_simdjson_key_value(const char *json, const char *key, zval *return_value, unsigned char assoc, u_short depth) /* {{{ */ {

    simdjson::dom::element doc = build_parsed_json_cust(reinterpret_cast<const uint8_t *>(json), strlen(json), true, depth);
    simdjson::dom::element element;

    try {
        element = doc.at(key);
    } catch (simdjson::simdjson_error &e) {
        zend_throw_exception(spl_ce_RuntimeException, e.what(), 0 );
        return;
    }

    if (assoc) {
        *return_value = simdjsonphp::make_array(element);
    } else {
        *return_value = simdjsonphp::make_object(element);
    }
}

/* }}} */

u_short cplus_simdjson_key_exists(const char *json, const char *key, u_short depth) /* {{{ */ {

    simdjson::dom::element doc = build_parsed_json_cust(reinterpret_cast<const uint8_t *>(json), strlen(json), true, depth);

    auto error = doc.at(key).error();

    if (error) {
        return SIMDJSON_PARSE_KEY_NOEXISTS;
    }
    return SIMDJSON_PARSE_KEY_EXISTS;
}

/* }}} */


void cplus_simdjson_key_count(const char *json, const char *key, zval *return_value, u_short depth) /* {{{ */ {

    simdjson::dom::element doc = build_parsed_json_cust(reinterpret_cast<const uint8_t *>(json), strlen(json), true, depth);
    simdjson::dom::element element;

    try {
        element = doc.at(key);
    } catch (simdjson::simdjson_error &e) {
        zend_throw_exception(spl_ce_RuntimeException, e.what(), 0 );
        return;
    }

    zval v;

    switch (element.type()) {
        //ASCII sort
        case simdjson::dom::element_type::ARRAY :
            ZVAL_LONG(&v, uint64_t(simdjson::dom::array(element).size()));
            break;
        case simdjson::dom::element_type::OBJECT :
            ZVAL_LONG(&v, uint64_t(simdjson::dom::object(element).size()));
            break;
        default:
            ZVAL_LONG(&v, 0);
            break;
    }
    *return_value = v;
}

/* }}} */