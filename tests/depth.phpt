--TEST--
simdjson_decode depth checked
--INI--
display_errors=stderr
error_reporting=E_ALL
--FILE--
<?php
var_dump(simdjson_decode('[]', true, 0));
var_dump(simdjson_key_value('[]', '', true, 0));
var_dump(simdjson_key_count('[]', '', 1));
var_dump(simdjson_decode('[]', true, 1));
echo "Test '[1]'\n";
try {
    var_dump(simdjson_decode('[1]', true, 1));
} catch (RuntimeException $e) {
    echo "Caught for [1]: {$e->getMessage()}\n";
}
var_dump(simdjson_decode('[1]', true, 2));
// XXX there's a difference between simdjson_decode and json_decode.
// In json_decode, an array with a scalar has the same depth as an array with no elements.
// In simdjson_decode, an array with a scalar is deeper than an array with no elements.
// For typical use cases, this shouldn't matter.
try {
    var_dump(simdjson_decode('[[1]]', true, 2));
} catch (RuntimeException $e) {
    echo "Caught for [[1]]: {$e->getMessage()}\n";
}
var_dump(simdjson_decode('[[]]', true, 2));
var_dump(simdjson_decode('[[1]]', true, 3));
?>
--EXPECTF--
Warning: simdjson_decode(): Depth must be greater than zero in %s on line 2
NULL
Warning: simdjson_key_value(): Depth must be greater than zero in %s on line 3
NULL
int(0)
array(0) {
}
Test '[1]'
Caught for [1]: The JSON document was too deep (too many nested objects and arrays)
array(1) {
  [0]=>
  int(1)
}
Caught for [[1]]: The JSON document was too deep (too many nested objects and arrays)
array(1) {
  [0]=>
  array(0) {
  }
}
array(1) {
  [0]=>
  array(1) {
    [0]=>
    int(1)
  }
}