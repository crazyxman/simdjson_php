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
#define PHP_SIMDJSON_API
#define SIMDJSON_G(v) (simdjson_globals.v)

#define PHP_SIMDJSON_VERSION                  "1.0.0"


#define SIMDJSON_STARTUP_FUNCTION(module)    ZEND_MINIT_FUNCTION(simdjson_##module)
#define SIMDJSON_RINIT_FUNCTION(module)        ZEND_RINIT_FUNCTION(simdjson_##module)
#define SIMDJSON_STARTUP(module)                ZEND_MODULE_STARTUP_N(simdjson_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define SIMDJSON_SHUTDOWN_FUNCTION(module)    ZEND_MSHUTDOWN_FUNCTION(simdjson_##module)
#define SIMDJSON_SHUTDOWN(module)            ZEND_MODULE_SHUTDOWN_N(simdjson_##module)(INIT_FUNC_ARGS_PASSTHRU)
extern PHPAPI void php_var_dump(zval **struc, int level);
extern PHPAPI void php_debug_zval_dump(zval **struc, int level);

ZEND_BEGIN_MODULE_GLOBALS(simdjson)

ZEND_END_MODULE_GLOBALS(simdjson)

PHP_MINIT_FUNCTION(simdjson);
PHP_MSHUTDOWN_FUNCTION(simdjson);
PHP_RINIT_FUNCTION(simdjson);
PHP_RSHUTDOWN_FUNCTION(simdjson);
PHP_MINFO_FUNCTION(simdjson);

extern ZEND_DECLARE_MODULE_GLOBALS(simdjson);

#endif
