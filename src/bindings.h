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

#define SIMDJSON_NODE_TYPE_STRING       '"'
#define SIMDJSON_NODE_TYPE_DOUBLE       'd'
#define SIMDJSON_NODE_TYPE_FALSE        'f'
#define SIMDJSON_NODE_TYPE_LONG         'l'
#define SIMDJSON_NODE_TYPE_NULL         'n'
#define SIMDJSON_NODE_TYPE_TRUE         't'
#define SIMDJSON_NODE_TYPE_ARRAY        '['
#define SIMDJSON_NODE_TYPE_OBJECT       '{'

namespace simdjsonphp {

    static bool isvalid(std::string_view p);

    static void parse(std::string_view p, zval *return_value, u_short assoc);

    static zval make_array(ParsedJson::iterator &pj);

    static zval make_object(ParsedJson::iterator &pj);
}

#ifdef __cplusplus
extern "C" {
#endif
bool cplus_isvalid(const char *json);
void cplus_parse(const char *json, zval *return_value, u_short assoc);
#ifdef __cplusplus
}
#endif

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */