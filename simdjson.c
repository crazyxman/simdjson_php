/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 王进喜
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

ZEND_BEGIN_ARG_INFO(simdjson_isvalid, 1)
ZEND_ARG_INFO(0, json)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(simdjson_decode, 1)
ZEND_ARG_INFO(0, json)
ZEND_ARG_INFO(0, assoc)
ZEND_END_ARG_INFO()

extern int cplus_isValid(const char *json);
extern int cplus_parse(const char *json, zval* return_value, u_short assoc);

PHP_FUNCTION(simdjson_isvalid)
{
    zend_string *version = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &version) == FAILURE) {
        return;
    }
    short is_json = cplus_isValid(ZSTR_VAL(version));
    ZVAL_BOOL(return_value, is_json);
}

PHP_FUNCTION(simdjson_decode)
{
    zend_bool assoc = 0;
    zend_string *version = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|b", &version, &assoc) == FAILURE) {
        return;
    }
    cplus_parse(ZSTR_VAL(version), return_value, assoc);
}

zend_function_entry simdjson_functions[] = {
        PHP_FE(simdjson_isvalid,                                 simdjson_isvalid)
        PHP_FE(simdjson_decode,                                 simdjson_decode)
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





