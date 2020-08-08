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

  tmp_version=$PHP_VERSION
  if test -z "$tmp_version"; then
    if test -z "$PHP_CONFIG"; then
      AC_MSG_ERROR([php-config not found])
    fi
    php_version=`$PHP_CONFIG --version 2>/dev/null|head -n 1|sed -e 's#\([0-9]\.[0-9]*\.[0-9]*\)\(.*\)#\1#'`
  else
    php_version=`echo "$tmp_version"|sed -e 's#\([0-9]\.[0-9]*\.[0-9]*\)\(.*\)#\1#'`
  fi

  if test -z "$php_version"; then
    AC_MSG_ERROR([failed to detect PHP version, please report])
  fi

  ac_IFS=$IFS
  IFS="."
  set $php_version
  IFS=$ac_IFS
  simdjson_php_version=`expr [$]1 \* 1000000 + [$]2 \* 1000 + [$]3`

  if test "$simdjson_php_version" -le "7000000"; then
    AC_MSG_ERROR([You need at least PHP 7.0.0 to be able to use this version of simdjson. PHP $php_version found])
  else
    AC_MSG_RESULT([$php_version, ok])
  fi

  PHP_SUBST(SIMDJSON_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, SIMDJSON_SHARED_LIBADD)

  AC_DEFINE(HAVE_SIMDJSON, 1, [whether simdjson is enabled])
  PHP_NEW_EXTENSION(simdjson,
      simdjson.cpp                        \
      src/bindings.cpp                    \
      src/simdjson.cpp,
    $ext_shared)

  PHP_ADD_MAKEFILE_FRAGMENT
  PHP_INSTALL_HEADERS([ext/simdjson], [php_simdjson.h])
fi
