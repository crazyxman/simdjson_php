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

extern zend_module_entry simdjson_module_entry;
#define phpext_simdjson_ptr &simdjson_module_entry

#define PHP_SIMDJSON_VERSION                  "2.1.0"
#define SIMDJSON_SUPPORT_URL                  "https://github.com/crazyxman/simdjson_php"
#define SIMDJSON_PARSE_FAIL                   0
#define SIMDJSON_PARSE_SUCCESS                1
#define SIMDJSON_PARSE_KEY_EXISTS             2
#define SIMDJSON_PARSE_KEY_NOEXISTS           3

#define SIMDJSON_PARSE_DEFAULT_DEPTH          512

/*
 * NOTE: Namespaces and references(&) are C++ only functionality.
 * To expose this functionality to other C PECLs,
 * switch to a forward class declaration of a class that only wraps simdjson::dom::parser
 */
class simdjson_php_parser;

ZEND_BEGIN_MODULE_GLOBALS(simdjson)
    /*
     * php::simdjson::parser pointer, constructed on first use with request-scope lifetime.
     * Note that in ZTS builds, the thread for each request will deliberately have different instances for each concurrently running request.
     * (The simdjson library is not thread safe)
     */
    simdjson_php_parser *parser;
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
# define PHP_SIMDJSON_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_SIMDJSON_API __attribute__ ((visibility("default")))
#else
# define PHP_SIMDJSON_API
#endif

#endif
