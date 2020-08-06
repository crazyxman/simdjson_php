dnl $Id$
dnl config.m4 for extension simdjson

sinclude(./autoconf/pecl.m4)
sinclude(./autoconf/php-executable.m4)

PECL_INIT([simdjson])

PHP_ARG_ENABLE(simdjson, whether to enable simdjson, [ --enable-simdjson   Enable simdjson])

if test "$PHP_SIMDJSON" != "no"; then

  CXXFLAGS=" -march=native -O2 -std=c++17 "
  PHP_REQUIRE_CXX()

  AC_MSG_CHECKING([PHP version])

  PHP_SIMDJSON_PHP_VERSION=$PHP_VERSION
  PHP_SIMDJSON_PHP_VERSION_ID=$PHP_VERSION_ID

  if test -z "$PHP_SIMDJSON_PHP_VERSION"; then
    if test -z "$PHP_CONFIG"; then
      AC_MSG_ERROR([php-config not found])
    fi
    PHP_SIMDJSON_PHP_VERSION=`${PHP_CONFIG} --version`
    PHP_SIMDJSON_PHP_VERSION_ID=`echo "${PHP_SIMDJSON_PHP_VERSION}" | $AWK 'BEGIN { FS = "."; } { printf "%d", ([$]1 * 100 + [$]2) * 100 + [$]3;}'`
  fi

  AC_MSG_RESULT($PHP_SIMDJSON_PHP_VERSION)
  if test "$PHP_SIMDJSON_PHP_VERSION_ID" -lt "73000"; then
    AC_MSG_ERROR([not supported. Need a PHP version >= 7.3.0 (found $PHP_SIMDJSON_PHP_VERSION)])
  fi

  AC_DEFINE(HAVE_SIMDJSON, 1, [whether simdjson is enabled])
  PHP_NEW_EXTENSION(simdjson,
      simdjson.cpp                        \
      src/bindings.cpp                    \
      src/simdjson.cpp,
    $ext_shared)

  PHP_ADD_MAKEFILE_FRAGMENT
  PHP_INSTALL_HEADERS([ext/simdjson], [php_simdjson.h])
fi
