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

#ifdef __AVX2__
#include "simdjson.h"
#include "php.h"

namespace simdjsonphp {
  bool isValid(std::string_view p);
  void parse(std::string_view p, zval* return_value, u_short assoc);
  zval makeArray(ParsedJson::iterator & pj);
  zval makeObject(ParsedJson::iterator & pj);
}

#ifdef __cplusplus
extern "C"{
#endif
	bool cplus_isValid(const char *json);
    void cplus_parse(const char *json, zval* return_value, u_short assoc);
#ifdef __cplusplus
}
#endif

#endif
