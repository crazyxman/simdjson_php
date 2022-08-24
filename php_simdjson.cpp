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

extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "zend_exceptions.h"
#include "main/SAPI.h"
#include "ext/standard/info.h"

#include "php_simdjson.h"
}

#include "src/bindings.h"

ZEND_DECLARE_MODULE_GLOBALS(simdjson);

ZEND_BEGIN_ARG_INFO_EX(simdjson_is_valid_arginfo, 0, 0, 1)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(simdjson_decode_arginfo, 0, 0, 1)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_INFO(0, assoc)
        ZEND_ARG_INFO(0, depth)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(simdjson_key_value_arginfo, 0, 0, 2)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, assoc, _IS_BOOL, 0)
        ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(simdjson_key_exists_arginfo, 0, 0, 2)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(simdjson_key_count_arginfo, 0, 0, 2)
        ZEND_ARG_INFO(0, json)
        ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
ZEND_END_ARG_INFO()

extern simdjson::dom::parser* cplus_simdjson_create_parser(void);

extern void cplus_simdjson_free_parser(simdjson::dom::parser* parser);

extern bool cplus_simdjson_is_valid(simdjson::dom::parser& parser, const char *json, size_t len, size_t depth);

extern void cplus_simdjson_parse(simdjson::dom::parser& parser, const char *json, size_t len, zval *return_value, unsigned char assoc, size_t depth);

extern void cplus_simdjson_key_value(simdjson::dom::parser& parser, const char *json, size_t len, const char *key, zval *return_value, unsigned char assoc, size_t depth);

extern u_short cplus_simdjson_key_exists(simdjson::dom::parser& parser, const char *json, size_t len, const char *key, size_t depth);

extern void cplus_simdjson_key_count(simdjson::dom::parser& parser, const char *json, size_t len, const char *key, zval *return_value, size_t depth);

#define SIMDJSON_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(simdjson, v)
static simdjson::dom::parser &simdjson_get_parser() {
    simdjson::dom::parser *parser = (simdjson::dom::parser *)SIMDJSON_G(parser);
    if (parser == NULL) {
        parser = cplus_simdjson_create_parser();
        SIMDJSON_G(parser) = parser;
        ZEND_ASSERT(parser != NULL);
    }
    return *parser;
}

// The simdjson parser accepts strings with at most 32-bit lengths, for now.
#define SIMDJSON_MAX_DEPTH ((zend_long)((SIZE_MAX / 8) < (UINT32_MAX / 2) ? (SIZE_MAX / 8) : (UINT32_MAX / 2)))

static bool simdjson_validate_depth(zend_long depth) {
    if (UNEXPECTED(depth <= 0)) {
        php_error_docref(NULL, E_WARNING, "Depth must be greater than zero");
        return false;
    } else if (UNEXPECTED(depth > SIMDJSON_MAX_DEPTH)) {
        php_error_docref(NULL, E_WARNING, "Depth exceeds maximum allowed value of " ZEND_LONG_FMT, SIMDJSON_MAX_DEPTH);
        return false;
    }
    return true;
}

PHP_FUNCTION (simdjson_is_valid) {
    zend_string *json = NULL;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|l", &json, &depth) == FAILURE) {
        return;
    }
    if (!simdjson_validate_depth(depth)) {
        RETURN_NULL();
    }
    bool is_json = cplus_simdjson_is_valid(simdjson_get_parser(), ZSTR_VAL(json), ZSTR_LEN(json), depth);
    ZVAL_BOOL(return_value, is_json);
}

PHP_FUNCTION (simdjson_decode) {
    zend_bool assoc = 0;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    zend_string *json = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|bl", &json, &assoc, &depth) == FAILURE) {
        return;
    }
    if (!simdjson_validate_depth(depth)) {
        RETURN_NULL();
    }
    cplus_simdjson_parse(simdjson_get_parser(), ZSTR_VAL(json), ZSTR_LEN(json), return_value, assoc, depth);
}

PHP_FUNCTION (simdjson_key_value) {

    zend_bool assoc = 0;
    zval *json = NULL;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    zend_string *key = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zS|bl", &json, &key, &assoc, &depth) == FAILURE) {
        return;
    }
    if (!simdjson_validate_depth(depth)) {
        RETURN_NULL();
    }
    if (IS_STRING == Z_TYPE_P(json)) {
        zend_string *zd_json = Z_STR_P(json);
        cplus_simdjson_key_value(simdjson_get_parser(), ZSTR_VAL(zd_json), ZSTR_LEN(zd_json), ZSTR_VAL(key), return_value, assoc, depth);
    } else {
        php_error_docref(NULL, E_WARNING, "expects parameter 1 to be string");
    }

}

PHP_FUNCTION (simdjson_key_count) {
    zval *json = NULL;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    zend_string *key = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zS|l", &json, &key, &depth) == FAILURE) {
        return;
    }
    if (!simdjson_validate_depth(depth)) {
        RETURN_NULL();
    }
    if (IS_STRING == Z_TYPE_P(json)) {
        zend_string *zd_json = Z_STR_P(json);
        cplus_simdjson_key_count(simdjson_get_parser(), ZSTR_VAL(zd_json), ZSTR_LEN(zd_json), ZSTR_VAL(key), return_value, depth);
    } else {
        php_error_docref(NULL, E_WARNING, "expects parameter 1 to be string");
    }

}

PHP_FUNCTION (simdjson_key_exists) {
    zval *json = NULL;
    zend_string *key = NULL;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zS|l", &json, &key, &depth) == FAILURE) {
        return;
    }
    if (!simdjson_validate_depth(depth)) {
        return;
    }
    u_short stats = SIMDJSON_PARSE_FAIL;
    if (IS_STRING == Z_TYPE_P(json)) {
        zend_string *zd_json = Z_STR_P(json);
        stats = cplus_simdjson_key_exists(simdjson_get_parser(), ZSTR_VAL(zd_json), ZSTR_LEN(zd_json), ZSTR_VAL(key), depth);
    } else {
        php_error_docref(NULL, E_WARNING, "expects parameter 1 to be string");
    }
    if (SIMDJSON_PARSE_FAIL == stats) {
        RETURN_NULL();
    } else if (SIMDJSON_PARSE_KEY_EXISTS == stats) {
        RETURN_TRUE;
    } else if (SIMDJSON_PARSE_KEY_NOEXISTS == stats) {
        RETURN_FALSE;
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
    {NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_GINIT_FUNCTION
*/
PHP_GINIT_FUNCTION (simdjson) {
#if defined(COMPILE_DL_SIMDJSON) && defined(ZTS)
ZEND_TSRMLS_CACHE_UPDATE();
#endif
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
    SIMDJSON_G(parser) = NULL;
    return SUCCESS;
}
/* }}} */

/** {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION (simdjson) {
    void *parser = SIMDJSON_G(parser);
    if (parser != NULL) {
        cplus_simdjson_free_parser((simdjson::dom::parser *) parser);
        SIMDJSON_G(parser) = NULL;
    }
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
    php_info_print_table_row(2, "Implementation", simdjson::get_active_implementation()->description().c_str());

    php_info_print_table_end();
}
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

/** {{{ DL support
 */
#ifdef COMPILE_DL_SIMDJSON
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif

ZEND_GET_MODULE(simdjson)
#endif
/* }}} */
