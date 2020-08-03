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

namespace simdjsonphp {

    static bool is_valid(std::string p);

    static void parse(const std::string& p, zval *return_value, unsigned char assoc, u_short depth);

    static zval make_array(simdjson::dom::element element);

    static zval make_object(simdjson::dom::element element);
}

#ifdef __cplusplus
extern "C" {
#endif

bool cplus_simdjson_isvalid(const char *json);

void cplus_simdjson_parse(const char *json, zval *return_value, unsigned char assoc, u_short depth);

void cplus_simdjson_key_value(const char *json, const char *key, zval *return_value, unsigned char assoc, u_short depth);

u_short cplus_simdjson_key_exists(const char *json, const char *key, u_short depth);

void cplus_simdjson_key_count(const char *json, const char *key, zval *return_value, u_short depth);

#ifdef __cplusplus
}
#endif