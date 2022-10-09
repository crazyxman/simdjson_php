--TEST--
simdjson_key_value with deep string test
--FILE--
<?php
$json = file_get_contents(__DIR__ . DIRECTORY_SEPARATOR . '_files' . DIRECTORY_SEPARATOR . 'result.json');
$value = \simdjson_key_value($json, "result/0/Hello3", false);
var_dump($value);

$value = \simdjson_key_value($json, "result/0/Hello3", true);
var_dump($value);

?>
--EXPECTF--
string(6) "World3"
string(6) "World3"