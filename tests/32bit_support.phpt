--TEST--
simdjson_decode test 32-bit
--INI--
serialize_precision=-1
--SKIPIF--
<?php if (PHP_INT_SIZE >= 8) echo "skip 32-bit platforms only\n"; ?>
--FILE--
<?php
// 32-bit performance is terrible and 32-bit builds don't take advantage of SIMD instructions,
// but this represent integers outside of the range PHP_INT_MIN..PHP_INT_MAX reasonably.
var_dump(simdjson_decode('[2147483647, 2147483648, -2147483648, -2147483649, 9223372036854775807, -9223372036854775808]'));
?>
--EXPECT--
array(6) {
  [0]=>
  int(2147483647)
  [1]=>
  float(2147483648)
  [2]=>
  int(-2147483648)
  [3]=>
  float(-2147483649)
  [4]=>
  float(9.223372036854776E+18)
  [5]=>
  float(-9.223372036854776E+18)
}
