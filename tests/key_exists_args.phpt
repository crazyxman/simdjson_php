--TEST--
simdjson_key_exists args test

--SKIPIF--
<?php ?>

--FILE--
<?php
$reflection = new \ReflectionFunction('\simdjson_key_exists');
var_dump((string) $reflection);

?>
--EXPECTF--
string(206) "Function [ <internal:simdjson> function simdjson_key_exists ] {

  - Parameters [3] {
    Parameter #0 [ <required> $json ]
    Parameter #1 [ <required> $key ]
    Parameter #2 [ <required> $depth ]
  }
}
"