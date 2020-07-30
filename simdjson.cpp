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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "main/SAPI.h"
#include "ext/standard/info.h"

#include "php_simdjson.h"
#include "src/bindings.h"

ZEND_DECLARE_MODULE_GLOBALS(simdjson);

ZEND_BEGIN_ARG_INFO(simdjson_is_valid_arginfo, 1)
        ZEND_ARG_INFO(0, json)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(simdjson_decode_arginfo, 1)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_INFO(0, assoc)
        ZEND_ARG_INFO(0, depth)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(simdjson_key_value_arginfo, 2)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_INFO(0, key)
        ZEND_ARG_INFO(0, assoc)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(simdjson_key_exists_arginfo, 2)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(simdjson_key_count_arginfo, 2)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(simdjson_resource_arginfo, 1)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_INFO(0, depth)
ZEND_END_ARG_INFO()

static int simdjson_pjh_resource_type;
static int simdjson_pj_resource_type;

extern bool cplus_simdjson_is_valid(const char *json);

extern void cplus_simdjson_parse(const char *json, zval *return_value, unsigned char assoc, u_short depth);

extern void cplus_simdjson_key_value(const char *json, const char *key, zval *return_value, unsigned char assoc, u_short depth);

extern void cplus_simdjson_key_value_pjh(void *pjh, const char *key, zval *return_value, unsigned char assoc);

extern u_short cplus_simdjson_key_exists(const char *json, const char *key, u_short depth);

extern u_short cplus_simdjson_key_exists_pjh(void *pjh, const char *key);

extern void cplus_simdjson_key_count(const char *json, const char *key, zval *return_value, u_short depth);

extern void cplus_simdjson_key_count_pjh(void *pjh, const char *key, zval *return_value);

extern void *cplus_simdjson_resource(const char *json, void *pj, u_short depth);

extern void cplus_simdjson_dtor(void *handle, u_short type);

PHP_FUNCTION (simdjson_is_valid) {
    zend_string *json = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &json) == FAILURE) {
        return;
    }
    short is_json = cplus_simdjson_is_valid(ZSTR_VAL(json));
    ZVAL_BOOL(return_value, is_json);
}

PHP_FUNCTION (simdjson_decode) {
    zend_bool assoc = 0;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    zend_string *json = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|bl", &json, &assoc, &depth) == FAILURE) {
        return;
    }
    cplus_simdjson_parse(ZSTR_VAL(json), return_value, assoc, depth + 1);
}

PHP_FUNCTION (simdjson_key_value) {

    zend_bool assoc = 0;
    zval *json = NULL;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    zend_string *key = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zS|bl", &json, &key, &assoc, &depth) == FAILURE) {
        return;
    }
    if (IS_STRING == Z_TYPE_P(json)) {
        cplus_simdjson_key_value(Z_STRVAL_P(json), ZSTR_VAL(key), return_value, assoc, depth + 1);
    } else if (IS_RESOURCE == Z_TYPE_P(json)) {
        void *pjh = zend_fetch_resource(Z_RES_P(json), "simdjson_pjh", simdjson_pjh_resource_type);
        if (NULL == pjh) {
            return;
        }
        cplus_simdjson_key_value_pjh(pjh, ZSTR_VAL(key), return_value, assoc);
    } else {
        php_error_docref(NULL, E_WARNING, "expects parameter 1 to be resource or string");
    }

}

PHP_FUNCTION (simdjson_key_count) {

    zval *json = NULL;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    zend_string *key = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zS|bl", &json, &key, &depth) == FAILURE) {
        return;
    }
    if (IS_STRING == Z_TYPE_P(json)) {
        cplus_simdjson_key_count(Z_STRVAL_P(json), ZSTR_VAL(key), return_value, depth + 1);
    } else if (IS_RESOURCE == Z_TYPE_P(json)) {
        void *pjh = zend_fetch_resource(Z_RES_P(json), "simdjson_pjh", simdjson_pjh_resource_type);
        if (NULL == pjh) {
            return;
        }
        cplus_simdjson_key_count_pjh(pjh, ZSTR_VAL(key), return_value);
    } else {
        php_error_docref(NULL, E_WARNING, "expects parameter 1 to be resource or string");
    }

}

PHP_FUNCTION (simdjson_key_exists) {
    zval *json = NULL;
    zend_string *key = NULL;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zS|l", &json, &key, &depth) == FAILURE) {
        return;
    }
    u_short stats = SIMDJSON_PARSE_FAIL;
    if (IS_STRING == Z_TYPE_P(json)) {
        stats = cplus_simdjson_key_exists(Z_STRVAL_P(json), ZSTR_VAL(key), depth + 1);
    } else if (IS_RESOURCE == Z_TYPE_P(json)) {
        void *pjh = zend_fetch_resource(Z_RES_P(json), "simdjson_pjh", simdjson_pjh_resource_type);
        if (NULL != pjh) {
            stats = cplus_simdjson_key_exists_pjh(pjh, ZSTR_VAL(key));
        }
    } else {
        php_error_docref(NULL, E_WARNING, "expects parameter 1 to be resource or string");
    }
    if (SIMDJSON_PARSE_FAIL == stats) {
        RETURN_NULL();
    } else if (SIMDJSON_PARSE_KEY_EXISTS == stats) {
        RETURN_TRUE;
    } else if (SIMDJSON_PARSE_KEY_NOEXISTS == stats) {
        RETURN_FALSE;
    }
}

PHP_FUNCTION (simdjson_resource) {
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    zend_string *json = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|l", &json, &depth) == FAILURE) {
        return;
    }
    void *pj = NULL;
    void *pjh = cplus_simdjson_resource(ZSTR_VAL(json), pj, depth);
    if (NULL == pjh) {
        RETURN_NULL();
    }
    zend_resource *res = zend_register_resource(pjh, simdjson_pjh_resource_type);
    zend_register_resource(pj, simdjson_pj_resource_type);
    RETURN_RES(res);
}

void simdjson_dtor(zend_resource *res) {

    if (simdjson_pjh_resource_type == res->type) {
        cplus_simdjson_dtor(res->ptr, SIMDJSON_RESOUCE_PJH_TYPE);
    } else if (simdjson_pj_resource_type == res->type) {
        cplus_simdjson_dtor(res->ptr, SIMDJSON_RESOUCE_PJ_TYPE);
    }

}

/* {{{ simdjson_functions[]
*/
zend_function_entry simdjson_functions[] = {
    PHP_FE(simdjson_is_valid, simdjson_is_valid_arginfo)
    PHP_FE(simdjson_decode, simdjson_decode_arginfo)
    PHP_FE(simdjson_key_value, simdjson_key_value_arginfo)
    PHP_FE(simdjson_key_exists, simdjson_key_exists_arginfo)
    PHP_FE(simdjson_key_count, simdjson_key_count_arginfo)
    PHP_FE(simdjson_resource, simdjson_resource_arginfo)
    {NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_GINIT_FUNCTION
*/
PHP_GINIT_FUNCTION (simdjson) {

}
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION (simdjson) {
    simdjson_pjh_resource_type = zend_register_list_destructors_ex(simdjson_dtor, NULL, "simdjson_pjh", module_number);
    simdjson_pj_resource_type = zend_register_list_destructors_ex(simdjson_dtor, NULL, "simdjson_pj", module_number);
    return SUCCESS;
}
/* }}} */

/** {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION (simdjson) {
    return SUCCESS;
}
/* }}} */

/** {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION (simdjson) {
    return SUCCESS;
}
/* }}} */

/** {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION (simdjson) {
    return SUCCESS;
}
/* }}} */

/** {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION (simdjson) {
    php_info_print_table_start();
    if (PG(expose_php) && !sapi_module.phpinfo_as_text) {
        php_info_print_table_header(2, "simdjson support", "enabled");
    } else {
        php_info_print_table_header(2, "simdjson support", "enabled");
    }

    php_info_print_table_row(2, "Version", PHP_SIMDJSON_VERSION);
    php_info_print_table_row(2, "Support", SIMDJSON_SUPPORT_URL);
    php_info_print_table_end();
}
/* }}} */

/** {{{ DL support
 */
#ifdef COMPILE_DL_SIMDJSON
ZEND_GET_MODULE(simdjson)
#endif
/* }}} */

/** {{{ module depends
 */
zend_module_dep simdjson_deps[] = {
    {NULL, NULL, NULL}
};
/* }}} */

/** {{{ simdjson_module_entry
*/
zend_module_entry simdjson_module_entry = {
    STANDARD_MODULE_HEADER_EX, NULL,
    simdjson_deps,
    "simdjson",
    simdjson_functions,
    PHP_MINIT(simdjson),
    PHP_MSHUTDOWN(simdjson),
    PHP_RINIT(simdjson),
    PHP_RSHUTDOWN(simdjson),
    PHP_MINFO(simdjson),
    PHP_SIMDJSON_VERSION,
    PHP_MODULE_GLOBALS(simdjson),
    PHP_GINIT(simdjson),
    NULL,
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

