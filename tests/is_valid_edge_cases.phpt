--TEST--
simdjson_is_valid edge cases test
--INI--
error_reporting=E_ALL
display_errors=stderr
--FILE--
<?php
var_dump(\simdjson_is_valid('[]', -1));
var_dump(\simdjson_is_valid('[[]]', 2));
var_dump(\simdjson_is_valid('[[]]', 1024));
var_dump(\simdjson_is_valid('[[]]', 1));
var_dump(\simdjson_is_valid('[[]]'));
echo "test deeply nested\n";
$long = str_repeat('[', 3000) . str_repeat(']', 3000);
var_dump(\simdjson_is_valid($long));
var_dump(\simdjson_is_valid($long, 2999));
var_dump(\simdjson_is_valid($long, 3000));
?>
--EXPECTF--
Warning: simdjson_is_valid(): Depth must be greater than zero in %sis_valid_edge_cases.php on line 2
NULL
bool(true)
bool(true)
bool(false)
bool(true)
test deeply nested
bool(false)
bool(false)
bool(true)
