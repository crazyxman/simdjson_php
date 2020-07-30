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

#define SIMDJSON_NODE_TYPE_STRING       '"'
#define SIMDJSON_NODE_TYPE_DOUBLE       'd'
#define SIMDJSON_NODE_TYPE_FALSE        'f'
#define SIMDJSON_NODE_TYPE_LONG         'l'
#define SIMDJSON_NODE_TYPE_NULL         'n'
#define SIMDJSON_NODE_TYPE_TRUE         't'
#define SIMDJSON_NODE_TYPE_ARRAY        '['
#define SIMDJSON_NODE_TYPE_OBJECT       '{'


namespace simdjsonphp {

    static bool is_valid(std::string p);

    static void parse(std::string p, zval *return_value, unsigned char assoc, u_short depth);

//    static zval make_array(simdjson::simdjson_result<simdjson::dom::element> result);
    static zval make_array(simdjson::dom::element element);

//    static zval make_object(simdjson::simdjson_result<simdjson::dom::element> element);
    static zval make_object(simdjson::dom::element element);
}

#ifdef __cplusplus
extern "C" {
#endif

bool cplus_simdjson_isvalid(const char *json);

void cplus_simdjson_parse(const char *json, zval *return_value, unsigned char assoc, u_short depth);

void
cplus_simdjson_key_value(const char *json, const char *key, zval *return_value, unsigned char assoc, u_short depth);

void cplus_simdjson_key_value_pjh(void *pjh, const char *key, zval *return_value, unsigned char assoc);

u_short cplus_simdjson_key_exists(const char *json, const char *key, u_short depth);

u_short cplus_simdjson_key_exists_pjh(void *pjh, const char *key);

void *cplus_simdjson_resource(const char *json, void *pj, u_short depth);

void cplus_simdjson_dtor(void *handle, u_short type);

#ifdef __cplusplus
}
#endif



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */