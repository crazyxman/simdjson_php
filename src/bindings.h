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

#include "simdjson.h"

extern zend_class_entry *simdjson_exception_ce;

simdjson::dom::parser* cplus_simdjson_create_parser(void);
void cplus_simdjson_free_parser(simdjson::dom::parser* parser);
bool cplus_simdjson_is_valid(const char *json, size_t len);
void cplus_simdjson_parse(const char *json, size_t len, zval *return_value, unsigned char assoc, size_t depth);
void cplus_simdjson_key_value(const char *json, size_t len, const char *key, zval *return_value, unsigned char assoc, size_t depth);
u_short cplus_simdjson_key_exists(const char *json, size_t len, const char *key, size_t depth);
void cplus_simdjson_key_count(const char *json, size_t len, const char *key, zval *return_value, size_t depth);
