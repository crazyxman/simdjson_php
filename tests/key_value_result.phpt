--TEST--
simdjson_key_value with deep string test
--FILE--
<?php
$json = file_get_contents(__DIR__ . DIRECTORY_SEPARATOR . '_files' . DIRECTORY_SEPARATOR . 'result.json');
$value = \simdjson_key_value($json, "result", false);
var_dump($value);

$value = \simdjson_key_value($json, "result", true);
var_dump($value);

?>
--EXPECTF--
array(2) {
  [0]=>
  object(stdClass)#1 (4) {
    ["_key"]=>
    string(5) "70614"
    ["_id"]=>
    string(14) "products/70614"
    ["_rev"]=>
    string(11) "_al3hU1K---"
    ["Hello3"]=>
    string(6) "World3"
  }
  [1]=>
  object(stdClass)#2 (4) {
    ["_key"]=>
    string(5) "70616"
    ["_id"]=>
    string(14) "products/70616"
    ["_rev"]=>
    string(11) "_al3hU1K--A"
    ["Hello4"]=>
    string(6) "World4"
  }
}
array(2) {
  [0]=>
  array(4) {
    ["_key"]=>
    string(5) "70614"
    ["_id"]=>
    string(14) "products/70614"
    ["_rev"]=>
    string(11) "_al3hU1K---"
    ["Hello3"]=>
    string(6) "World3"
  }
  [1]=>
  array(4) {
    ["_key"]=>
    string(5) "70616"
    ["_id"]=>
    string(14) "products/70616"
    ["_rev"]=>
    string(11) "_al3hU1K--A"
    ["Hello4"]=>
    string(6) "World4"
  }
}