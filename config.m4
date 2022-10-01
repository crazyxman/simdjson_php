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

  dnl Mark symbols hidden by default if the compiler (for example, gcc >= 4)
  dnl supports it. This can help reduce the binary size and startup time.
  AX_CHECK_COMPILE_FLAG([-fvisibility=hidden],
                        [CXXFLAGS="$CXXFLAGS -fvisibility=hidden"])

  AC_DEFINE(HAVE_SIMDJSON, 1, [whether simdjson is enabled])
  dnl Disable exceptions because PHP is written in C and loads this C++ module, handle errors manually.
  dnl Disable development checks of C simdjson library in php debug builds (can manually override)
  PHP_NEW_EXTENSION(simdjson, [
      php_simdjson.cpp                    \
      src/bindings.cpp                    \
      src/simdjson.cpp],
    $ext_shared,, "-std=c++17 -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -DSIMDJSON_EXCEPTIONS=0 -DSIMDJSON_DEVELOPMENT_CHECKS=0", cxx)

  PHP_INSTALL_HEADERS([ext/simdjson], [php_simdjson.h, src/bindings.h src/bindings_impl.h])
  PHP_ADD_MAKEFILE_FRAGMENT
  PHP_ADD_BUILD_DIR(src, 1)
fi
