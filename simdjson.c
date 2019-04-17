/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Jinxi Wang <1054636713@qq.com>                                                             |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "main/SAPI.h"
#include "Zend/zend_alloc.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#include "php_simdjson.h"
#include "src/bindings.h"

ZEND_DECLARE_MODULE_GLOBALS(simdjson);

ZEND_BEGIN_ARG_INFO(simd_json_isvalid, 1)
ZEND_ARG_INFO(0, json)
ZEND_END_ARG_INFO()

extern int cplus_isValid(const char *json);
extern int cplus_parse(const char *json, zval* return_value);

PHP_FUNCTION(simd_json_valid)
{
    zend_string *version = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &version) == FAILURE) {
        return;
    }
    short is_json = cplus_isValid(ZSTR_VAL(version));
    ZVAL_BOOL(return_value, is_json);
}

PHP_FUNCTION(simd_json_decode)
{
    zend_string *version = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &version) == FAILURE) {
        return;
    }

    cplus_parse(ZSTR_VAL(version), return_value);
}

zend_function_entry simdjson_functions[] = {
        PHP_FE(simd_json_valid,                                  simd_json_isvalid)
        PHP_FE(simd_json_decode,                                 simd_json_isvalid)
        {NULL, NULL, NULL}
};

PHP_GINIT_FUNCTION(simdjson)
{
     memset(simdjson_globals, 0, sizeof(*simdjson_globals));
}

PHP_MINIT_FUNCTION(simdjson)
{
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(simdjson)
{
    return SUCCESS;
}

PHP_RINIT_FUNCTION(simdjson)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(simdjson)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(simdjson)
{
    php_info_print_table_start();
    if (PG(expose_php) && !sapi_module.phpinfo_as_text) {
        php_info_print_table_header(2, "simdjson support", "enabled");
    } else {
        php_info_print_table_header(2, "simdjson support", "enabled");
    }

    php_info_print_table_row(2, "Version", PHP_SIMDJSON_VERSION);
    php_info_print_table_row(2, "Supports", "http://www.lycitea.com");
    php_info_print_table_end();


}

#ifdef COMPILE_DL_SIMDJSON
ZEND_GET_MODULE(simdjson)
#endif

#if ZEND_MODULE_API_NO >= 20050922
zend_module_dep simdjson_deps[] = {
	{NULL, NULL, NULL}
};
#endif

zend_module_entry simdjson_module_entry = {
#if ZEND_MODULE_API_NO >= 20050922
STANDARD_MODULE_HEADER_EX, NULL,
simdjson_deps,
#else
    STANDARD_MODULE_HEADER,
#endif
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





