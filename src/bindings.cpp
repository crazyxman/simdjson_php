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

#include "php.h"
#include "simdjson.h"
#include "bindings.h"

static bool simdjsonphp::isvalid(std::string p) /* {{{ */ {
    ParsedJson pj = build_parsed_json(p);
    return pj.isValid();
}

/* }}} */

bool cplus_isvalid(const char *json) /* {{{ */ {
    return simdjsonphp::isvalid(json);
}

/* }}} */

static void simdjsonphp::parse(std::string p, zval *return_value, unsigned char assoc, u_short depth) /* {{{ */ {
    ParsedJson pj = build_parsed_json(p, depth);
    if (!pj.isValid()) {
        return;
    }
    ParsedJson::iterator pjh(pj);
    if (assoc) {
        *return_value = simdjsonphp::make_array(pjh);
    } else {
        *return_value = simdjsonphp::make_object(pjh);
    }
}

/* }}} */

void cplus_parse(const char *json, zval *return_value, unsigned char assoc, u_short depth) /* {{{ */ {
    simdjsonphp::parse(json, return_value, assoc, depth);
}

/* }}} */

static zval simdjsonphp::make_array(ParsedJson::iterator &pjh) /* {{{ */ {
    zval v;
    switch (pjh.get_type()) {
        //ASCII sort
        case SIMDJSON_NODE_TYPE_STRING :
            ZVAL_STRING(&v, pjh.get_string());
            break;
        case SIMDJSON_NODE_TYPE_DOUBLE : ZVAL_DOUBLE(&v, pjh.get_double());
            break;
        case SIMDJSON_NODE_TYPE_FALSE:
            ZVAL_FALSE(&v);
            break;
        case SIMDJSON_NODE_TYPE_LONG : ZVAL_LONG(&v, pjh.get_integer());
            break;
        case SIMDJSON_NODE_TYPE_NULL:
            ZVAL_NULL(&v);
            break;
        case SIMDJSON_NODE_TYPE_TRUE:
            ZVAL_TRUE(&v);
            break;
        case SIMDJSON_NODE_TYPE_ARRAY :
            zval arr;
            array_init(&arr);
            if (pjh.down()) {
                zval value = simdjsonphp::make_array(pjh);
                add_next_index_zval(&arr, &value);
                while (pjh.next()) {
                    zval value = simdjsonphp::make_array(pjh);
                    add_next_index_zval(&arr, &value);
                }
                pjh.up();
            }
            v = arr;
            break;
        case SIMDJSON_NODE_TYPE_OBJECT :
            zval obj;
            array_init(&obj);
            if (pjh.down()) {
                const char *key = pjh.get_string();
                pjh.next();
                zval value = simdjsonphp::make_array(pjh);
                add_assoc_zval(&obj, key, &value);
                while (pjh.next()) {
                    key = pjh.get_string();
                    pjh.next();
                    zval value = simdjsonphp::make_array(pjh);
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

/* }}} */

static zval simdjsonphp::make_object(ParsedJson::iterator &pjh) /* {{{ */ {
    zval v;
    switch (pjh.get_type()) {
        //ASCII sort
        case SIMDJSON_NODE_TYPE_STRING :
            ZVAL_STRING(&v, pjh.get_string());
            break;
        case SIMDJSON_NODE_TYPE_DOUBLE : ZVAL_DOUBLE(&v, pjh.get_double());
            break;
        case SIMDJSON_NODE_TYPE_FALSE:
            ZVAL_FALSE(&v);
            break;
        case SIMDJSON_NODE_TYPE_LONG : ZVAL_LONG(&v, pjh.get_integer());
            break;
        case SIMDJSON_NODE_TYPE_NULL:
            ZVAL_NULL(&v);
            break;
        case SIMDJSON_NODE_TYPE_TRUE:
            ZVAL_TRUE(&v);
            break;
        case SIMDJSON_NODE_TYPE_ARRAY :
            zval arr;
            array_init(&arr);
            if (pjh.down()) {
                zval value = simdjsonphp::make_object(pjh);
                add_next_index_zval(&arr, &value);
                while (pjh.next()) {
                    zval value = simdjsonphp::make_object(pjh);
                    add_next_index_zval(&arr, &value);
                }
                pjh.up();
            }
            v = arr;
            break;
        case SIMDJSON_NODE_TYPE_OBJECT :
            zval obj;
            object_init(&obj);
            if (pjh.down()) {
                const char *key = pjh.get_string();
                pjh.next();
                zval value = simdjsonphp::make_object(pjh);
                add_property_zval(&obj, key, &value);
                zval_ptr_dtor(&value);
                while (pjh.next()) {
                    key = pjh.get_string();
                    pjh.next();
                    zval value = simdjsonphp::make_object(pjh);
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

/* }}} */



static bool cplus_find_node(const char *json, const char *key, ParsedJson::iterator &pjh) /* {{{ */ {

    char *pkey = estrdup(key);
    char const *seps = "\t";
    char *token = strtok(pkey, seps);
    bool found = false;

    while (token != NULL) {
        found = false;
        if (pjh.is_object()) {
            if (pjh.down()) {
                do {
                    if (strcmp(pjh.get_string(), token) == 0) {
                        found = true;
                        pjh.next();
                        break;
                    }
                    pjh.next();
                } while (pjh.next());
            }
        } else if (pjh.is_array()) {
            if (pjh.down()) {
                int n = 0, index = 0;
                try {
                    index = std::stoul(token);
                } catch (...) {
                    break;
                }
                do {
                    if (n == index) {
                        found = true;
                        break;
                    }
                    n++;
                } while (pjh.next());
            }
        }
        if (!found) {
            break;
        }
        token = strtok(NULL, seps);
    }
    efree(pkey);
    if (found) {
        return true;
    }
    return false;
}

/* }}} */

void cplus_fastget(const char *json, const char *key, zval *return_value, unsigned char assoc) /* {{{ */ {

    ParsedJson pj = build_parsed_json(json);
    if (!pj.isValid()) {
        return;
    }
    ParsedJson::iterator pjh(pj);
    cplus_find_node(json, key, pjh);

    if (assoc) {
        *return_value = simdjsonphp::make_array(pjh);
    } else {
        *return_value = simdjsonphp::make_object(pjh);
    }
}

/* }}} */

bool cplus_key_exists(const char *json, const char *key) /* {{{ */ {

    ParsedJson pj = build_parsed_json(json);
    if (!pj.isValid()) {
        return false;
    }
    ParsedJson::iterator pjh(pj);
    return cplus_find_node(json, key, pjh);
}

/* }}} */

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */