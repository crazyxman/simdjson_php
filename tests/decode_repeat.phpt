--TEST--
simdjson_decode repeated keys use the last value
--FILE--
<?php
// Repeated
$json = '{"a":"b","a":"c","0":[1],"0":[2],"foo":"bar","foo":"baz"}';
$value = \simdjson_decode($json, false);
var_dump($value);

$value = \simdjson_decode($json, true);
var_dump($value);
?>
--EXPECT--
object(stdClass)#1 (3) {
  ["a"]=>
  string(1) "c"
  ["0"]=>
  array(1) {
    [0]=>
    int(2)
  }
  ["foo"]=>
  string(3) "baz"
}
array(3) {
  ["a"]=>
  string(1) "c"
  [0]=>
  array(1) {
    [0]=>
    int(2)
  }
  ["foo"]=>
  string(3) "baz"
}
