--TEST--
simdjson_key_value args test
--FILE--
<?php
$reflection = new \ReflectionFunction('\simdjson_key_value');
echo $reflection;

?>
--EXPECTF--
Function [ <internal:simdjson> function simdjson_key_value ] {

  - Parameters [4] {
    Parameter #0 [ <required> string $json ]
    Parameter #1 [ <required> string $key ]
    Parameter #2 [ <optional> bool%S $assoc%S ]
    Parameter #3 [ <optional> int%S $depth%S ]
  }
}