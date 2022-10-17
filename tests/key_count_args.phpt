--TEST--
simdjson_key_count args test
--FILE--
<?php
$reflection = new \ReflectionFunction('\simdjson_key_count');
echo $reflection;

?>
--EXPECTF--
Function [ <internal:simdjson> function simdjson_key_count ] {

  - Parameters [4] {
    Parameter #0 [ <required> string $json ]
    Parameter #1 [ <required> string $key ]
    Parameter #2 [ <optional> int%S $depth%S ]
    Parameter #3 [ <optional> bool%S $throw_if_uncountable%S ]
  }
  - Return [ int%S ]
}