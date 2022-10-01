--TEST--
Bug #45791 compat (json_decode() does not handle number 0e0)
--FILE--
<?php

var_dump(simdjson_decode('{"zero": 0e0}'));

?>
--EXPECT--
object(stdClass)#1 (1) {
  ["zero"]=>
  float(0)
}
