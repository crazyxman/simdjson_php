--TEST--
simdjson_decode test all types
--FILE--
<?php
// Test all types that work both on 32-bit and 64-bit PECL builds.
$json = '[{}, {"":"b"}, "", "x", 0, -1, 0.5, null, false, true, []]';
$value = \simdjson_decode($json, false);
var_dump($value);

$value = \simdjson_decode($json, true);
var_dump($value);

?>
--EXPECT--
array(11) {
  [0]=>
  object(stdClass)#1 (0) {
  }
  [1]=>
  object(stdClass)#2 (1) {
    [""]=>
    string(1) "b"
  }
  [2]=>
  string(0) ""
  [3]=>
  string(1) "x"
  [4]=>
  int(0)
  [5]=>
  int(-1)
  [6]=>
  float(0.5)
  [7]=>
  NULL
  [8]=>
  bool(false)
  [9]=>
  bool(true)
  [10]=>
  array(0) {
  }
}
array(11) {
  [0]=>
  array(0) {
  }
  [1]=>
  array(1) {
    [""]=>
    string(1) "b"
  }
  [2]=>
  string(0) ""
  [3]=>
  string(1) "x"
  [4]=>
  int(0)
  [5]=>
  int(-1)
  [6]=>
  float(0.5)
  [7]=>
  NULL
  [8]=>
  bool(false)
  [9]=>
  bool(true)
  [10]=>
  array(0) {
  }
}
