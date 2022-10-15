--TEST--
simdjson_is_valid edge cases test
--INI--
error_reporting=E_ALL
display_errors=stderr
--FILE--
<?php
require_once __DIR__ . '/dump.inc';
dump(function () { return simdjson_is_valid('[]', -1); });
dump(function () { return simdjson_is_valid('[[]]', 2); });
dump(function () { return simdjson_is_valid('[[]]', 1024); });
dump(function () { return simdjson_is_valid('[[]]', 1); });
dump(function () { return simdjson_is_valid('[[]]'); });
echo "test deeply nested\n";
$long = str_repeat('[', 3000) . str_repeat(']', 3000);
dump(function () use ($long) { return simdjson_is_valid($long); });
dump(function () use ($long) { return simdjson_is_valid($long, 2999); });
dump(function () use ($long) { return simdjson_is_valid($long, 3000); });
?>
--EXPECTF--
Caught SimdJsonValueError: simdjson_is_valid(): Argument #2 ($depth) must be greater than zero
bool(true)
bool(true)
bool(false)
bool(true)
test deeply nested
bool(false)
bool(false)
bool(true)
