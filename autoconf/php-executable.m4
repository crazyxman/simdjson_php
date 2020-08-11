dnl
dnl Generate run-php bash script
dnl
AC_CONFIG_COMMANDS_POST([
  ln -s "$PHP_EXECUTABLE" build/php
])
