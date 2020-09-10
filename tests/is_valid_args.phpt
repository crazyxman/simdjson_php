--TEST--
simdjson_is_valid args test

--SKIPIF--
<?php ?>

--FILE--
<?php
$reflection = new \ReflectionFunction('\simdjson_is_valid');
var_dump((string) $reflection);

?>
--EXPECTF--
string(128) "Function [ <internal:simdjson> function simdjson_is_valid ] {

  - Parameters [1] {
    Parameter #0 [ <required> $json ]
  }
}
"