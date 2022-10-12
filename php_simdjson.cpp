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

#if PHP_VERSION_ID >= 70200
#define SIMDJSON_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
    ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null)
#else
#define SIMDJSON_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
    ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, NULL, allow_null)
#endif

SIMDJSON_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(simdjson_is_valid_arginfo, 0, 1, _IS_BOOL, 1)
        ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(simdjson_decode_arginfo, 0, 0, 1)
        ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, assoc, _IS_BOOL, 0)
        ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(simdjson_key_value_arginfo, 0, 0, 2)
        ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, assoc, _IS_BOOL, 0)
        ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
ZEND_END_ARG_INFO()

SIMDJSON_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(simdjson_key_exists_arginfo, 0, 2, _IS_BOOL, 1)
        ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
ZEND_END_ARG_INFO()

SIMDJSON_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(simdjson_key_count_arginfo, 0, 2, IS_LONG, 1)
        ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
        ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define SIMDJSON_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(simdjson, v)
static simdjson::dom::parser &simdjson_get_reused_parser() {
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
    bool is_json;
    if (SIMDJSON_SHOULD_REUSE_PARSER(ZSTR_LEN(json), depth)) {
        is_json = cplus_simdjson_is_valid(simdjson_get_reused_parser(), ZSTR_VAL(json), ZSTR_LEN(json), depth);
    } else {
        simdjson::dom::parser parser;
        SIMDJSON_TRY_ALLOCATE(parser, ZSTR_LEN(json), depth);
        is_json = cplus_simdjson_is_valid(parser, ZSTR_VAL(json), ZSTR_LEN(json), depth);
    }
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
    if (SIMDJSON_SHOULD_REUSE_PARSER(ZSTR_LEN(json), depth)) {
        cplus_simdjson_parse(simdjson_get_reused_parser(), ZSTR_VAL(json), ZSTR_LEN(json), return_value, assoc, depth);
    } else {
        simdjson::dom::parser parser;
        SIMDJSON_TRY_ALLOCATE(parser, ZSTR_LEN(json), depth);
        cplus_simdjson_parse(parser, ZSTR_VAL(json), ZSTR_LEN(json), return_value, assoc, depth);
    }
}

PHP_FUNCTION (simdjson_key_value) {

    zend_string *json = NULL;
    zend_string *key = NULL;
    zend_bool assoc = 0;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS|bl", &json, &key, &assoc, &depth) == FAILURE) {
        return;
    }
    if (!simdjson_validate_depth(depth)) {
        RETURN_NULL();
    }
    if (SIMDJSON_SHOULD_REUSE_PARSER(ZSTR_LEN(json), depth)) {
        cplus_simdjson_key_value(simdjson_get_reused_parser(), ZSTR_VAL(json), ZSTR_LEN(json), ZSTR_VAL(key), return_value, assoc, depth);
    } else {
        simdjson::dom::parser parser;
        SIMDJSON_TRY_ALLOCATE(parser, ZSTR_LEN(json), depth);
        cplus_simdjson_key_value(parser, ZSTR_VAL(json), ZSTR_LEN(json), ZSTR_VAL(key), return_value, assoc, depth);
    }
}

PHP_FUNCTION (simdjson_key_count) {
    zend_string *json = NULL;
    zend_string *key = NULL;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS|l", &json, &key, &depth) == FAILURE) {
        return;
    }
    if (!simdjson_validate_depth(depth)) {
        RETURN_NULL();
    }
    if (SIMDJSON_SHOULD_REUSE_PARSER(ZSTR_LEN(json), depth)) {
        cplus_simdjson_key_count(simdjson_get_reused_parser(), ZSTR_VAL(json), ZSTR_LEN(json), ZSTR_VAL(key), return_value, depth);
    } else {
        simdjson::dom::parser parser;
        SIMDJSON_TRY_ALLOCATE(parser, ZSTR_LEN(json), depth);
        cplus_simdjson_key_count(parser, ZSTR_VAL(json), ZSTR_LEN(json), ZSTR_VAL(key), return_value, depth);
    }
}

PHP_FUNCTION (simdjson_key_exists) {
    zend_string *json = NULL;
    zend_string *key = NULL;
    zend_long depth = SIMDJSON_PARSE_DEFAULT_DEPTH;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS|l", &json, &key, &depth) == FAILURE) {
        return;
    }
    if (!simdjson_validate_depth(depth)) {
        return;
    }
    u_short stats;
    if (SIMDJSON_SHOULD_REUSE_PARSER(ZSTR_LEN(json), depth)) {
        stats = cplus_simdjson_key_exists(simdjson_get_reused_parser(), ZSTR_VAL(json), ZSTR_LEN(json), ZSTR_VAL(key), depth);
    } else {
        simdjson::dom::parser parser;
        SIMDJSON_TRY_ALLOCATE(parser, ZSTR_LEN(json), depth);
        stats = cplus_simdjson_key_exists(parser, ZSTR_VAL(json), ZSTR_LEN(json), ZSTR_VAL(key), depth);
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
    php_info_print_table_header(2, "simdjson support", "enabled");

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
