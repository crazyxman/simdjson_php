#ifdef __AVX2__
#include "simdjson.h"
#include "php.h"

namespace simdjsonphp {
  bool isValid(std::string_view p);
  void parse(std::string_view p, zval* return_value);
  zval makeArray(ParsedJson::iterator & pj);
}

#ifdef __cplusplus
extern "C"{
#endif
	bool cplus_isValid(const char *json);
    void cplus_parse(const char *json, zval* return_value);
#ifdef __cplusplus
}
#endif

#endif