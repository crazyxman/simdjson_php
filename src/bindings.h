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

#ifndef PHP_SIMDJSON_BINDINGS
#define PHP_SIMDJSON_BINDINGS

#include "simdjson.h"

#define SIMDJSON_DEPTH_CHECK_THRESHOLD     100000
#define SIMDJSON_CAPACITY_RECLAIM_THRESHOLD 1000000

#define SIMDJSON_SHOULD_REUSE_PARSER(strlen, depth) (EXPECTED((strlen) <= SIMDJSON_DEPTH_CHECK_THRESHOLD && (depth) <= SIMDJSON_DEPTH_CHECK_THRESHOLD))

ZEND_API simdjson::dom::parser* cplus_simdjson_create_parser(void);
ZEND_API void cplus_simdjson_free_parser(simdjson::dom::parser* parser);
ZEND_API bool cplus_simdjson_is_valid(simdjson::dom::parser &parser, const char *json, size_t len, size_t depth);
ZEND_API void cplus_simdjson_parse(simdjson::dom::parser &parser, const char *json, size_t len, zval *return_value, unsigned char assoc, size_t depth);
ZEND_API void cplus_simdjson_key_value(simdjson::dom::parser &parser, const char *json, size_t len, const char *key, zval *return_value, unsigned char assoc, size_t depth);
u_short cplus_simdjson_key_exists(simdjson::dom::parser &parser, const char *json, size_t len, const char *key, size_t depth);
ZEND_API void cplus_simdjson_key_count(simdjson::dom::parser &parser, const char *json, size_t len, const char *key, zval *return_value, size_t depth);

// Emit out of memory errors to make behavior of json_is_valid consistent with other PHP APIs if it returns. https://github.com/crazyxman/simdjson_php/issues/80
#define SIMDJSON_OUT_OF_MEMORY_ERROR_NORETURN() zend_error_noreturn(E_ERROR, "simdjson_php: Out of memory")
#define SIMDJSON_TRY_ALLOCATE(parser, capacity, depth) { \
        auto error = (parser).allocate((capacity), (depth)); \
        if (UNEXPECTED(error)) { SIMDJSON_OUT_OF_MEMORY_ERROR_NORETURN(); } \
    }

#endif /* PHP_SIMDJSON_BINDINGS */
