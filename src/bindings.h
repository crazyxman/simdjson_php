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

#ifndef SIMDJSON_PHP_BINDINGS_H
#define SIMDJSON_PHP_BINDINGS_H

#include "Zend/zend.h"
#include "Zend/zend_portability.h"

// NOTE: All code in this header file should be made compatible with C, so that pecls written in C can use this functionality.
// BEGIN_EXTERN_C is needed for symbols to be mangled using C rules instead of C++ rules in all includers.
BEGIN_EXTERN_C()
extern ZEND_API zend_class_entry *simdjson_exception_ce;

// NOTE: Namespaces and references(&) are C++ only functionality.
// To expose this functionality to other C PECLs,
// switch to a forward class declaration of a class that only wraps simdjson::dom::parser
class simdjson_php_parser;
typedef uint8_t simdjson_php_error_code;

ZEND_API void cplus_simdjson_throw_jsonexception(simdjson_php_error_code);
ZEND_API simdjson_php_parser* cplus_simdjson_create_parser(void);
ZEND_API void cplus_simdjson_free_parser(simdjson_php_parser* parser);
ZEND_API bool cplus_simdjson_is_valid(simdjson_php_parser* parser, const char *json, size_t len, size_t depth);
ZEND_API simdjson_php_error_code cplus_simdjson_parse(simdjson_php_parser* parser, const char *json, size_t len, zval *return_value, unsigned char assoc, size_t depth);
ZEND_API simdjson_php_error_code cplus_simdjson_key_value(simdjson_php_parser* parser, const char *json, size_t len, const char *key, zval *return_value, unsigned char assoc, size_t depth);
ZEND_API u_short cplus_simdjson_key_exists(simdjson_php_parser* parser, const char *json, size_t len, const char *key, size_t depth);
ZEND_API simdjson_php_error_code cplus_simdjson_key_count(simdjson_php_parser* parser, const char *json, size_t len, const char *key, zval *return_value, size_t depth);

END_EXTERN_C()

#endif
