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
 * Put all of the publicly visible functionality and macros into the same header file
 * (On windows, the include paths used by the c compiler may be different)
 */
#include "Zend/zend.h"
#include "Zend/zend_portability.h"

// All code in this header file should be changed to go within BEGIN_EXTERN_C/END_EXTERN_C macros (both header definitions, and C++ declarations, including function implementations),
// so that pecls written in C can use this functionality without separate C++ files to load bindings.h.
// BEGIN_EXTERN_C is needed for symbols to be mangled using C rules instead of C++ rules in all includers.
// When I add ZEND_API to declarations and possibly definitions, I get linker errors.
BEGIN_EXTERN_C()

extern zend_module_entry simdjson_module_entry;
#define phpext_simdjson_ptr &simdjson_module_entry

#define PHP_SIMDJSON_VERSION                  "2.2.0dev"
#define SIMDJSON_SUPPORT_URL                  "https://github.com/crazyxman/simdjson_php"
#define SIMDJSON_PARSE_FAIL                   0
#define SIMDJSON_PARSE_SUCCESS                1
#define SIMDJSON_PARSE_KEY_EXISTS             2
#define SIMDJSON_PARSE_KEY_NOEXISTS           3

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

/*
 * This module defines utilities and helper functions used elsewhere in simdjson.
 */
#ifdef PHP_WIN32
#	define PHP_SIMDJSON_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SIMDJSON_API __attribute__ ((visibility("default")))
#else
#	define PHP_SIMDJSON_API /* nothing special */
#endif

extern zend_class_entry *simdjson_exception_ce;

// NOTE: Namespaces and references(&) are C++ only functionality.
// To expose this functionality to other C PECLs,
// switch to a forward class declaration of a class that only wraps simdjson::dom::parser
typedef uint8_t simdjson_php_error_code;

/* NOTE: Callers should check if len is greater than 4GB - simdjson will always return a non zero error code for those */

/* FIXME add cplus_simdjson_get_default_singleton_parser api */
/* FIXME add cplus_simdjson_decode_with_default_singleton_parser(return_value, json, len, bool assoc) */

PHP_SIMDJSON_API const char* cplus_simdjson_error_msg(simdjson_php_error_code);
PHP_SIMDJSON_API void cplus_simdjson_throw_jsonexception(simdjson_php_error_code);
PHP_SIMDJSON_API struct simdjson_php_parser* cplus_simdjson_create_parser(void);
PHP_SIMDJSON_API void cplus_simdjson_free_parser(struct simdjson_php_parser* parser);
PHP_SIMDJSON_API bool cplus_simdjson_is_valid(struct simdjson_php_parser* parser, const char *json, size_t len, size_t depth);
PHP_SIMDJSON_API simdjson_php_error_code cplus_simdjson_parse(struct simdjson_php_parser* parser, const char *json, size_t len, zval *return_value, bool assoc, size_t depth);
PHP_SIMDJSON_API simdjson_php_error_code cplus_simdjson_key_value(struct simdjson_php_parser* parser, const char *json, size_t len, const char *key, zval *return_value, bool assoc, size_t depth);
PHP_SIMDJSON_API u_short cplus_simdjson_key_exists(struct simdjson_php_parser* parser, const char *json, size_t len, const char *key, size_t depth);
PHP_SIMDJSON_API simdjson_php_error_code cplus_simdjson_key_count(struct simdjson_php_parser* parser, const char *json, size_t len, const char *key, zval *return_value, size_t depth);

END_EXTERN_C()

#endif
