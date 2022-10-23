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

#ifndef PHP_SIMDJSON_H
#define PHP_SIMDJSON_H

/*
 * Error constant implementation notes:
 *
 * - 0 always means success, and non-0 is a failure condition.
 * - Prefix these with SIMDJSON_PHP_ERR_ to distinguish them from other values.
 * - The error codes (value or labels) belonging to the C simdjson project may change in the future.
 *
 *   Maybe these should be exposed as extern const once there's a project that needs the other values.
 *   For now, they're also exposed as `REGISTER_LONG_CONSTANT("SIMDJSON_ERR_" #errcode, (val), CONST_PERSISTENT)`
 */
#define SIMDJSON_PHP_ERR_SUCCESS 0
#define SIMDJSON_PHP_ERR_INVALID_PHP_PROPERTY 255
#define SIMDJSON_PHP_ERR_KEY_COUNT_NOT_COUNTABLE 254

/*
 * Put all of the publicly visible functionality and macros into the same header file
 * (On windows, the include paths used by the c compiler may be different)
 */
#include "Zend/zend.h"
#include "Zend/zend_portability.h"

/*
 * All code in this header file should be changed to go within BEGIN_EXTERN_C/END_EXTERN_C macros
 * (both header definitions, and C++ declarations, including function implementations),
 * so that pecls written in C can use this functionality without separate C++ files to load bindings.h.
 *
 * This header file deliberately does not depend on other header files in this project,
 * to make including this header file easier for other PECLs (avoid include path issues)
 *
 * BEGIN_EXTERN_C is needed for symbols to be mangled using C rules instead of C++ rules in all includers.
 * (This macro can be used from both C and C++ source files)
 */
BEGIN_EXTERN_C()

extern zend_module_entry simdjson_module_entry;
#define phpext_simdjson_ptr &simdjson_module_entry

#define PHP_SIMDJSON_VERSION                  "4.0.1dev"
/**
 * PHP_SIMDJSON_VERSION_ID has the same format as PHP_VERSION_ID: Major version * 10000 + Minor version * 100 + Patch version.
 * This is meant for use by PECL extensions that depend on simdjson.
 * (e.g. 4.5.6dev and 4.5.6 would be 40506)
 */
#define PHP_SIMDJSON_VERSION_ID               40001

#define SIMDJSON_SUPPORT_URL                  "https://github.com/crazyxman/simdjson_php"

#define SIMDJSON_PARSE_DEFAULT_DEPTH          512

/*
 * NOTE: Namespaces and references(&) are C++ only functionality.
 * To expose this functionality to other C PECLs,
 * switch to a forward struct declaration of a struct that only wraps simdjson::dom::parser
 */
struct simdjson_php_parser;

ZEND_BEGIN_MODULE_GLOBALS(simdjson)
    /*
     * php::simdjson::parser pointer, constructed on first use with request-scope lifetime.
     * Note that in ZTS builds, the thread for each request will deliberately have different instances for each concurrently running request.
     * (The simdjson library is not thread safe)
     *
     * This is similar to php-src's ext/session session data storage.
     */
    struct simdjson_php_parser *parser;
ZEND_END_MODULE_GLOBALS(simdjson)

PHP_MINIT_FUNCTION(simdjson);
PHP_MSHUTDOWN_FUNCTION(simdjson);
PHP_RINIT_FUNCTION(simdjson);
PHP_RSHUTDOWN_FUNCTION(simdjson);
PHP_MINFO_FUNCTION(simdjson);

#ifdef ZTS
#ifdef COMPILE_DL_SIMDJSON
ZEND_TSRMLS_CACHE_EXTERN()
#endif
#endif

/* Only the functions and variables defined with PHP_SIMDJSON_API can be loaded by other PECLs */
#ifdef PHP_WIN32
#	define PHP_SIMDJSON_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SIMDJSON_API __attribute__ ((visibility("default")))
#else
#	define PHP_SIMDJSON_API /* nothing special */
#endif

/** Defines 'class SimdJsonException' */
extern PHP_SIMDJSON_API zend_class_entry *simdjson_exception_ce;
extern PHP_SIMDJSON_API zend_class_entry *simdjson_value_error_ce;

/**
 * NOTE: Namespaces and references(&) and classes (instead of structs) are C++ only functionality.
 *
 * To expose this functionality to other C PECLs,
 * switch to a forward class declaration of a class that only wraps simdjson::dom::parser
 */
typedef uint8_t simdjson_php_error_code;

/* NOTE: Callers should check if len is greater than 4GB - simdjson will always return a non zero error code for those */

/**
 * Parses the given string into a return code, using the default singleton parser.
 *
 * This must be called after simdjson's request initialization phase and before simdjson's request shutdown phase.
 * (e.g. PECLs should not use this during module or request initialization/shutdown)
 */
PHP_SIMDJSON_API simdjson_php_error_code php_simdjson_parse_default(const char *json, size_t len, zval *return_value, bool associative, size_t depth);

/**
 * Checks if the given JSON is valid, using the default singleton parser.
 * Returns 0 if it is valid.
 */
PHP_SIMDJSON_API simdjson_php_error_code php_simdjson_validate_default(const char *json, size_t len, size_t depth);

/**
 * Returns or creates the singleton parser used internally by simdjson (e.g. for the `php_simdjson_*_default()` methods).
 * (Thread-local in ZTS builds of PHP)
 *
 * Callers must NOT free this.
 */
PHP_SIMDJSON_API struct simdjson_php_parser *php_simdjson_get_default_singleton_parser(void);

/* FIXME add php_simdjson_get_default_singleton_parser api */
/* FIXME add php_simdjson_decode_with_default_singleton_parser(return_value, json, len, bool associative) */

/**
 * Returns the error message corresponding to a given error code returned by a call to simdjson_php.
 */
PHP_SIMDJSON_API const char* php_simdjson_error_msg(simdjson_php_error_code code);
/**
 * Throw a SimdJsonException with the provided error code and the corresponding error message.
 */
PHP_SIMDJSON_API void php_simdjson_throw_jsonexception(simdjson_php_error_code code);
/**
 * Create a brand new parser instance.
 *
 * The caller must free it with php_simdjson_free_parser once it is no longer used.
 *
 * Callers may use this instead of the shared singleton parser when memory usage is a concern
 * (e.g. the PECLs are likely to be used load a string that's megabytes long in a long-lived php process)
 *
 * Callers should free this parser before or during the request shutdown phase.
 */
PHP_SIMDJSON_API struct simdjson_php_parser* php_simdjson_create_parser(void);
/**
 * Release a parser **constructed by php_simdjson_create_parser** and all associated buffers.
 */
PHP_SIMDJSON_API void php_simdjson_free_parser(struct simdjson_php_parser* parser);
/**
 * Returns 0 if the given json string is valid
 */
PHP_SIMDJSON_API simdjson_php_error_code php_simdjson_validate(struct simdjson_php_parser* parser, const char *json, size_t len, size_t depth);
/**
 * Parses the given string into a return code.
 *
 * If the returned error code is 0, then return_value contains the parsed value.
 * If the returned error code is non-0, then return_value will not be initialized.
 */
PHP_SIMDJSON_API simdjson_php_error_code php_simdjson_parse(struct simdjson_php_parser* parser, const char *json, size_t len, zval *return_value, bool associative, size_t depth);
/**
 * Parses the part of the given string at the json pointer 'key' into a PHP value at return_value
 *
 * If the returned error code is 0, then return_value contains the parsed value (or null).
 * If the returned error code is non-0, then return_value will not be initialized.
 *
 * - SIMDJSON_ERR_NO_SUCH_FIELD is returned if the json pointer 'key' is not found
 * - Other errors are returned for invalid json, etc.
 *
 * @see https://www.rfc-editor.org/rfc/rfc6901.html
 */
PHP_SIMDJSON_API simdjson_php_error_code php_simdjson_key_value(struct simdjson_php_parser* parser, const char *json, size_t len, const char *key, zval *return_value, bool associative, size_t depth);
/**
 * Checks if the json pointer 'key' exists in the given json string.
 *
 * - 0 if the key exists
 * - NO_SUCH_FIELD if a field does not exist in an object
 * - INDEX_OUT_OF_BOUNDS if an array index is larger than an array length
 * - INCORRECT_TYPE if a non-integer is used to access an array
 * - INVALID_JSON_POINTER if the JSON pointer is invalid and cannot be parsed
 *
 * @see https://www.rfc-editor.org/rfc/rfc6901.html
 */
PHP_SIMDJSON_API uint8_t php_simdjson_key_exists(struct simdjson_php_parser* parser, const char *json, size_t len, const char *key, size_t depth);
/**
 * Count the keys of the given array/object at json pointer 'key' exists in the given json string.
 *
 * If the returned error code is 0, then the zval in return_value is overwritten with the key count using ZVAL_LONG.
 * - For arrays, this is the array size
 * - For objects, this is the object size
 * - For other values, this is 0 (when fail_if_uncountable is false)
 *
 * @see https://www.rfc-editor.org/rfc/rfc6901.html
 */
PHP_SIMDJSON_API simdjson_php_error_code php_simdjson_key_count(struct simdjson_php_parser* parser, const char *json, size_t len, const char *key, zval *return_value, size_t depth, bool fail_if_uncountable);

END_EXTERN_C()

#endif
