/*
  +----------------------------------------------------------------------+
  | simdjson_php                                                         |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.0 of the Apache license,    |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.apache.org/licenses/LICENSE-2.0.html                      |
  +----------------------------------------------------------------------+
  | Author: Tyson Andre <tandre@php.net>                                 |
  +----------------------------------------------------------------------+
*/
#ifndef SIMDJSON_PHP_BINDINGS_IMPL_H
#define SIMDJSON_PHP_BINDINGS_IMPL_H

#include "simdjson.h"

// NOTE: Namespaces are C++ only functionality.
// To expose this functionality to other C PECLs,
// bindings.h exposes a forward class declaration of a class that only wraps simdjson::dom::parser
struct simdjson_php_parser {
public:
    simdjson::dom::parser parser;
};

#endif
