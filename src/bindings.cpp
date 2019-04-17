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

#ifdef __AVX2__
#include "bindings.h"

bool simdjsonphp::isValid(std::string_view p) {
  ParsedJson pj = build_parsed_json(p);
  return pj.isValid();
}

bool cplus_isValid(const char *json){
    return simdjsonphp::isValid(json);
}

void simdjsonphp::parse(std::string_view p, zval *return_value, u_short assoc) {
    ParsedJson pj = build_parsed_json(p);
    if (!pj.isValid()) {
        return;
    }
    ParsedJson::iterator pjh(pj);
    if(assoc){
        *return_value = simdjsonphp::makeArray(pjh);
    }else{
        *return_value = simdjsonphp::makeObject(pjh);
    }
}

void cplus_parse(const char *json, zval *return_value, u_short assoc) {
    simdjsonphp::parse(json, return_value, assoc);
}

zval simdjsonphp::makeArray(ParsedJson::iterator &pjh) {
    zval v;
    uint8_t type = pjh.get_type();
    switch (type) {
        //ASCII sort
        case '"' :
            ZVAL_STRING(&v, pjh.get_string());
            break;
        case 'd' : ZVAL_DOUBLE(&v, pjh.get_double());
            break;
        case 'f':
            ZVAL_FALSE(&v);
            break;
        case 'l' : ZVAL_LONG(&v, pjh.get_integer());
            break;
        case 'n':
            ZVAL_NULL(&v);
            break;
        case 't':
            ZVAL_TRUE(&v);
            break;
        case '[' :
            zval arr;
            array_init(&arr);
            if (pjh.down()) {
                zval value = simdjsonphp::makeArray(pjh);
                add_next_index_zval(&arr, &value);
                while (pjh.next()) {
                    zval value = simdjsonphp::makeArray(pjh);
                    add_next_index_zval(&arr, &value);
                }
                pjh.up();
            }
            v = arr;
            break;
        case '{' :
            zval obj;
            array_init(&obj);
            if (pjh.down()) {
                const char *key = pjh.get_string();
                pjh.next();
                zval value = simdjsonphp::makeArray(pjh);
                add_assoc_zval(&obj, key, &value);
                while (pjh.next()) {
                    key = pjh.get_string();
                    pjh.next();
                    zval value = simdjsonphp::makeArray(pjh);
                    add_assoc_zval(&obj, key, &value);
                }
                pjh.up();
            }
            v = obj;
            break;
        default:
            break;
    }
    return v;
}

zval simdjsonphp::makeObject(ParsedJson::iterator &pjh) {
    zval v;
    uint8_t type = pjh.get_type();
    switch (type) {
        //ASCII sort
        case '"' :
            ZVAL_STRING(&v, pjh.get_string());
            break;
        case 'd' : ZVAL_DOUBLE(&v, pjh.get_double());
            break;
        case 'f':
            ZVAL_FALSE(&v);
            break;
        case 'l' : ZVAL_LONG(&v, pjh.get_integer());
            break;
        case 'n':
            ZVAL_NULL(&v);
            break;
        case 't':
            ZVAL_TRUE(&v);
            break;
        case '[' :
            zval arr;
            array_init(&arr);
            if (pjh.down()) {
                zval value = simdjsonphp::makeObject(pjh);
                add_next_index_zval(&arr, &value);
                while (pjh.next()) {
                    zval value = simdjsonphp::makeObject(pjh);
                    add_next_index_zval(&arr, &value);
                }
                pjh.up();
            }
            v = arr;
            break;
        case '{' :
            zval obj;
            object_init(&obj);
            if (pjh.down()) {
                const char *key = pjh.get_string();
                pjh.next();
                zval value = simdjsonphp::makeObject(pjh);
                add_property_zval(&obj, key, &value);
                zval_ptr_dtor(&value);
                while (pjh.next()) {
                    key = pjh.get_string();
                    pjh.next();
                    zval value = simdjsonphp::makeObject(pjh);
                    add_property_zval(&obj, key, &value);
                    zval_ptr_dtor(&value);
                }
                pjh.up();
            }
            v = obj;
            break;
        default:
            break;
    }
    return v;
}

#endif
