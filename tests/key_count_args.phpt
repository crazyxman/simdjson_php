--TEST--
simdjson_key_count args test

--SKIPIF--
<?php ?>

--FILE--
<?php
$reflection = new \ReflectionFunction('\simdjson_key_count');
var_dump((string) $reflection);

?>
--EXPECTF--
string(205) "Function [ <internal:simdjson> function simdjson_key_count ] {

  - Parameters [3] {
    Parameter #0 [ <required> $json ]
    Parameter #1 [ <required> $key ]
    Parameter #2 [ <required> $depth ]
  }
}
"