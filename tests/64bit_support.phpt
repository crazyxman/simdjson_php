--TEST--
simdjson_decode test 64-bit
--SKIPIF--
<?php if (PHP_INT_SIZE < 8) echo "skip 64-bit platforms only\n"; ?>
--FILE--
<?php
var_dump(simdjson_decode('[2147483647, 2147483648, -2147483648, -2147483649, 9223372036854775807, -9223372036854775808]'));
?>
--EXPECT--
array(6) {
  [0]=>
  int(2147483647)
  [1]=>
  int(2147483648)
  [2]=>
  int(-2147483648)
  [3]=>
  int(-2147483649)
  [4]=>
  int(9223372036854775807)
  [5]=>
  int(-9223372036854775808)
}
