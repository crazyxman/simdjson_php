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
ZEND_END_ARG_INFO()

extern int cplus_isvalid(const char *json);
extern int cplus_parse(const char *json, zval *return_value, u_short assoc);

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
    zend_string *version = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|b", &version, &assoc) == FAILURE) {
        return;
    }
    cplus_parse(ZSTR_VAL(version), return_value, assoc);
}

/* {{{ simdjson_functions[]
*/
zend_function_entry simdjson_functions[] = {
    PHP_FE(simdjson_isvalid, simdjson_isvalid)
    PHP_FE(simdjson_decode, simdjson_decode)
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
    php_info_print_table_row(2, "Supports", SIMDJSON_SUPPORT_URL);
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

