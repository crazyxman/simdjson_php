--TEST--
simdjson_is_valid test
--FILE--
<?php
$json = file_get_contents(__DIR__ . DIRECTORY_SEPARATOR . '_files' . DIRECTORY_SEPARATOR . 'result.json');
$value = \simdjson_is_valid($json);
var_dump($value);

$value = \simdjson_is_valid('{"corrupt": true,');
var_dump($value);

$value = \simdjson_is_valid('{"corrupt" true}');
var_dump($value);

$value = \simdjson_is_valid('Invalid JSON string');
var_dump($value);

?>
--EXPECTF--
bool(true)
bool(false)
bool(false)
bool(false)