--TEST--
simdjson_decode args test
--FILE--
<?php
$reflection = new \ReflectionFunction('\simdjson_decode');
echo $reflection;

?>
--EXPECTF--
Function [ <internal:simdjson> function simdjson_decode ] {

  - Parameters [3] {
    Parameter #0 [ <required> string $json ]
    Parameter #1 [ <optional> bool%S $assoc%S ]
    Parameter #2 [ <optional> int%S $depth%S ]
  }
}