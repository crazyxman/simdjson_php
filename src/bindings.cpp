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
#include "php_simdjson.h"

#include "simdjson.h"
#include "bindings.h"


WARN_UNUSED
ParsedJson* build_parsed_json_cust(const uint8_t *buf, size_t len, bool reallocifneeded, u_short depth = DEFAULTMAXDEPTH) {
    ParsedJson *pj = new ParsedJson();
    bool ok = pj->allocateCapacity(len, depth);
    if(ok) {
        int res = json_parse(buf, len, *pj, reallocifneeded);
        ok = res == simdjson::SUCCESS;
        assert(ok == pj->isValid());
    } else {
        std::cerr << "failure during memory allocation " << std::endl;
    }
    return pj;
}


static bool simdjsonphp::isvalid(std::string p) /* {{{ */ {
    ParsedJson *pj = build_parsed_json_cust(reinterpret_cast<const uint8_t *>(p.data()), p.length(), true);
    bool isvalid = pj->isValid();
    delete pj;
    return isvalid;
}

/* }}} */

bool cplus_simdjson_isvalid(const char *json) /* {{{ */ {
    return simdjsonphp::isvalid(json);
}

/* }}} */

void* cplus_simdjson_resource(const char *json, void *return_pj, u_short depth) /* {{{ */ {
    ParsedJson *pj = build_parsed_json_cust(reinterpret_cast<const uint8_t *>(json), strlen(json), true, depth);
    if (!pj->isValid()) {
        delete pj;
        return nullptr;
    }
    ParsedJson::iterator *pjh = new  ParsedJson::iterator(*pj);
    return_pj = reinterpret_cast<void *>(pj);
    return reinterpret_cast<void *>(pjh);
}

/* }}} */

void cplus_simdjson_dtor(void *handle, u_short type) /* {{{ */ {

    if(SIMDJSON_RESOUCE_PJH_TYPE == type) {
        ParsedJson::iterator *pjh = reinterpret_cast<ParsedJson::iterator *>(handle);
        delete pjh;
    } else if(SIMDJSON_RESOUCE_PJ_TYPE == type) {
        ParsedJson *pj = reinterpret_cast<ParsedJson *>(handle);
        delete pj;
    }

}

/* }}} */
static void simdjsonphp::parse(std::string p, zval *return_value, unsigned char assoc, u_short depth) /* {{{ */ {
    ParsedJson *pj = build_parsed_json_cust(reinterpret_cast<const uint8_t *>(p.data()), p.length(), true, depth);
    if (!pj->isValid()) {
        delete pj;
        return;
    }
    ParsedJson::iterator pjh(*pj);
    if (assoc) {
        *return_value = simdjsonphp::make_array(pjh);
    } else {
        *return_value = simdjsonphp::make_object(pjh);
    }
    delete pj;
}

/* }}} */

void cplus_simdjson_parse(const char *json, zval *return_value, unsigned char assoc, u_short depth) /* {{{ */ {
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



static bool cplus_find_node(const char *key, ParsedJson::iterator &pjh) /* {{{ */ {

    char *pkey = estrdup(key);
    char const *seps = "\t";
    char *token = strtok(pkey, seps);
    bool found = false;

    while (token != NULL) {
        found = false;
        switch (pjh.get_type()) {
            case SIMDJSON_NODE_TYPE_ARRAY :
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
                break;
            case SIMDJSON_NODE_TYPE_OBJECT :
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
                break;
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

void cplus_simdjson_key_value(const char *json, const char *key, zval *return_value, unsigned char assoc, u_short depth) /* {{{ */ {

    ParsedJson *pj = build_parsed_json_cust(reinterpret_cast<const uint8_t *>(json), strlen(json), true, depth);
    if (!pj->isValid()) {
        delete pj;
        return;
    }
    ParsedJson::iterator pjh(*pj);
    bool is_found = cplus_find_node(key, pjh);
    if(!is_found) {
        goto _return_null;
    }
    if (assoc) {
        *return_value = simdjsonphp::make_array(pjh);
    } else {
        *return_value = simdjsonphp::make_object(pjh);
    }

    _return_null:
    delete pj;

}

/* }}} */

void cplus_simdjson_key_value_pjh(void *pjh, const char *key, zval *return_value, unsigned char assoc) /* {{{ */ {

    ParsedJson::iterator *pjh_v = reinterpret_cast<ParsedJson::iterator *>(pjh);
    while (pjh_v->up()) {}
    bool is_found = cplus_find_node(key, *pjh_v);
    if(!is_found) {
        return;
    }
    if (assoc) {
        *return_value = simdjsonphp::make_array(*pjh_v);
    } else {
        *return_value = simdjsonphp::make_object(*pjh_v);
    }

}

/* }}} */

u_short cplus_simdjson_key_exists(const char *json, const char *key, u_short depth) /* {{{ */ {

    ParsedJson *pj = build_parsed_json_cust(reinterpret_cast<const uint8_t *>(json), strlen(json), true, depth);
    if (!pj->isValid()) {
        delete pj;
        return SIMDJSON_PARSE_FAIL;
    }
    ParsedJson::iterator pjh(*pj);
    bool is_found = cplus_find_node(key, pjh);
    delete pj;
    if (is_found) {
        return SIMDJSON_PARSE_KEY_EXISTS;
    } else {
        return SIMDJSON_PARSE_KEY_NOEXISTS;
    }

}

/* }}} */


u_short cplus_simdjson_key_exists_pjh(void *pjh, const char *key) /* {{{ */ {

    ParsedJson::iterator *pjh_v = reinterpret_cast<ParsedJson::iterator *>(pjh);
    while (pjh_v->up()) {}
    bool is_found = cplus_find_node(key, *pjh_v);
    if (is_found) {
        return SIMDJSON_PARSE_KEY_EXISTS;
    } else {
        return SIMDJSON_PARSE_KEY_NOEXISTS;
    }

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