--TEST--
simdjson_key_value with int test
--SKIPIF--
<?php if (PHP_VERSION_ID < 70100) echo "skip requires php 7.1+ for empty string as object key\n";
--FILE--
<?php
require_once __DIR__ . '/dump.inc';
$json = '{"":true,"code":123,"values":[{}],"obj":{"z": false}}';
$pointers = [
    '/code',
    '/',
    '',
    '/code',
    '/code/0',
    '/code/x',
    '/values',
    '/values/0',
    '/values/1',
    '/values/-1',
    '/obj',
    '/obj/z',
];
foreach ($pointers as $pointer) {
    echo "Test ", var_export($pointer, true), "\n";
    dump(function () use ($json, $pointer) { return simdjson_key_value($json, $pointer, false); });
    dump(function () use ($json, $pointer) { return simdjson_key_value($json, $pointer, true); });
}

?>
--EXPECT--
Test '/code'
int(123)
int(123)
Test '/'
bool(true)
bool(true)
Test ''
object(stdClass)#2 (4) {
  [""]=>
  bool(true)
  ["code"]=>
  int(123)
  ["values"]=>
  array(1) {
    [0]=>
    object(stdClass)#3 (0) {
    }
  }
  ["obj"]=>
  object(stdClass)#4 (1) {
    ["z"]=>
    bool(false)
  }
}
array(4) {
  [""]=>
  bool(true)
  ["code"]=>
  int(123)
  ["values"]=>
  array(1) {
    [0]=>
    array(0) {
    }
  }
  ["obj"]=>
  array(1) {
    ["z"]=>
    bool(false)
  }
}
Test '/code'
int(123)
int(123)
Test '/code/0'
Caught SimdJsonException: Invalid JSON pointer syntax.
Caught SimdJsonException: Invalid JSON pointer syntax.
Test '/code/x'
Caught SimdJsonException: Invalid JSON pointer syntax.
Caught SimdJsonException: Invalid JSON pointer syntax.
Test '/values'
array(1) {
  [0]=>
  object(stdClass)#2 (0) {
  }
}
array(1) {
  [0]=>
  array(0) {
  }
}
Test '/values/0'
object(stdClass)#2 (0) {
}
array(0) {
}
Test '/values/1'
Caught SimdJsonException: Attempted to access an element of a JSON array that is beyond its length.
Caught SimdJsonException: Attempted to access an element of a JSON array that is beyond its length.
Test '/values/-1'
Caught SimdJsonException: The JSON element does not have the requested type.
Caught SimdJsonException: The JSON element does not have the requested type.
Test '/obj'
object(stdClass)#2 (1) {
  ["z"]=>
  bool(false)
}
array(1) {
  ["z"]=>
  bool(false)
}
Test '/obj/z'
bool(false)
bool(false)
