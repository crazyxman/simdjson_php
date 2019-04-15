/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Jinxi Wang <1054636713@qq.com>                                                             |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_LYCITEA_SRC_HELPERS_H
#define PHP_LYCITEA_SRC_HELPERS_H


#ifdef __cplusplus
extern "C"{
#endif

int add_next_index_zval(zval *arg, zval *value);
int add_assoc_zval_ex(zval *arg, const char *key, size_t key_len, zval *value);

#ifdef __cplusplus
}
#endif

#endif
