--TEST--
simdjson_key_exists test
--FILE--
<?php
$json = file_get_contents(__DIR__ . DIRECTORY_SEPARATOR . '_files' . DIRECTORY_SEPARATOR . 'result.json');
$value = \simdjson_key_exists($json, "result");
var_dump($value);

$value = \simdjson_key_exists($json, "result/0/Hello3");
var_dump($value);

$value = \simdjson_key_exists($json, "unknown");
var_dump($value);

$value = \simdjson_key_exists($json, "unknown/deep/nested");
var_dump($value);

$value = \simdjson_key_exists($json, "result/10");
var_dump($value);

?>
--EXPECTF--
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)