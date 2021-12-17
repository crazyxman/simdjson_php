--TEST--
simdjson_decode args test

--SKIPIF--
<?php ?>

--FILE--
<?php
$reflection = new \ReflectionFunction('\simdjson_decode');
echo $reflection;

?>
--EXPECTF--
Function [ <internal:simdjson> function simdjson_decode ] {

  - Parameters [3] {
    Parameter #0 [ <required> $json ]
    Parameter #1 [ <optional> $assoc%S ]
    Parameter #2 [ <optional> $depth%S ]
  }
}