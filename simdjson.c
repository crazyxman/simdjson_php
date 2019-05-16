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

ZEND_BEGIN_ARG_INFO(simdjson_isvalid, 1)
        ZEND_ARG_INFO(0, json)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(simdjson_decode, 1)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_INFO(0, assoc)
        ZEND_ARG_INFO(0, depth)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(simdjson_fastget, 2)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_INFO(0, key)
        ZEND_ARG_INFO(0, assoc)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(simdjson_key_exists, 2)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

extern unsigned char cplus_isvalid(const char *json);

extern void cplus_parse(const char *json, zval *return_value, unsigned char assoc, u_short depth);

extern void cplus_fastget(const char *json, const char *key, zval *return_value, unsigned char assoc);

extern bool cplus_key_exists(const char *json, const char *key);

PHP_FUNCTION (simdjson_isvalid) {
    zend_string *version = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &version) == FAILURE) {
        return;
    }
    short is_json = cplus_isvalid(ZSTR_VAL(version));
    ZVAL_BOOL(return_value, is_json);
}

PHP_FUNCTION (simdjson_decode) {
    zend_bool assoc = 0;
    zend_long depth = 512;
    zend_string *json = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|bl", &json, &assoc, &depth) == FAILURE) {
        return;
    }
    cplus_parse(ZSTR_VAL(json), return_value, assoc, depth + 1);
}

PHP_FUNCTION (simdjson_fastget) {
    zend_bool assoc = 0;
    zend_string *json = NULL, *key = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS|b", &json, &key, &assoc) == FAILURE) {
        return;
    }
    cplus_fastget(ZSTR_VAL(json), ZSTR_VAL(key), return_value, assoc);
}

PHP_FUNCTION (simdjson_key_exists) {
    zend_string *json = NULL, *key = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS", &json, &key) == FAILURE) {
        return;
    }
    unsigned char is_exists = cplus_key_exists(ZSTR_VAL(json), ZSTR_VAL(key));
    RETURN_BOOL(is_exists);
}

/* {{{ simdjson_functions[]
*/
zend_function_entry simdjson_functions[] = {
    PHP_FE(simdjson_isvalid, simdjson_isvalid)
    PHP_FE(simdjson_decode, simdjson_decode)
    PHP_FE(simdjson_fastget, simdjson_fastget)
    PHP_FE(simdjson_key_exists, simdjson_key_exists)
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

