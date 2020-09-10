--TEST--
simdjson_key_value args test

--SKIPIF--
<?php ?>

--FILE--
<?php
$reflection = new \ReflectionFunction('\simdjson_key_value');
var_dump((string) $reflection);

?>
--EXPECTF--
string(244) "Function [ <internal:simdjson> function simdjson_key_value ] {

  - Parameters [4] {
    Parameter #0 [ <required> $json ]
    Parameter #1 [ <required> $key ]
    Parameter #2 [ <required> $assoc ]
    Parameter #3 [ <required> $depth ]
  }
}
"