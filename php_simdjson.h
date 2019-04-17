    
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 王进喜
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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