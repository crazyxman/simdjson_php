dnl config.m4 for extension simdjson

PHP_ARG_ENABLE(simdjson, whether to enable simdjson, [ --enable-simdjson   Enable simdjson])

if test "$PHP_SIMDJSON" != "no"; then

  PHP_REQUIRE_CXX()

  AC_MSG_CHECKING([PHP version])

  if test -z "$PHP_CONFIG"; then
    AC_MSG_ERROR([php-config not found])
  fi
  php_version=`$PHP_CONFIG --vernum`

  if test -z "$php_version"; then
    AC_MSG_ERROR([failed to detect PHP version, please report])
  fi

  if test "$php_version" -lt "70000"; then
    AC_MSG_ERROR([You need at least PHP 7.0.0 to be able to use this version of simdjson. PHP $php_version found])
  else
    AC_MSG_RESULT([$php_version, ok])
  fi

  AC_DEFINE(HAVE_SIMDJSON, 1, [whether simdjson is enabled])
  PHP_NEW_EXTENSION(simdjson, [
      simdjson.cpp                        \
      src/bindings.cpp                    \
      src/simdjson.cpp],
    $ext_shared,, "-std=c++17")

  PHP_INSTALL_HEADERS([ext/simdjson], [php_simdjson.h])
  PHP_ADD_MAKEFILE_FRAGMENT
  PHP_ADD_BUILD_DIR(src, 1)
fi
