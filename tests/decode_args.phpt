--TEST--
simdjson_decode args test

--SKIPIF--
<?php ?>

--FILE--
<?php
$reflection = new \ReflectionFunction('\simdjson_decode');
var_dump((string) $reflection);

?>
--EXPECTF--
string(204) "Function [ <internal:simdjson> function simdjson_decode ] {

  - Parameters [3] {
    Parameter #0 [ <required> $json ]
    Parameter #1 [ <required> $assoc ]
    Parameter #2 [ <required> $depth ]
  }
}
"