--TEST--
simdjson_key_value with int test
--FILE--
<?php
$json = file_get_contents(__DIR__ . DIRECTORY_SEPARATOR . '_files' . DIRECTORY_SEPARATOR . 'result.json');
$value = \simdjson_key_value($json, "code", false);
var_dump($value);

$value = \simdjson_key_value($json, "code", true);
var_dump($value);

?>
--EXPECTF--
int(201)
int(201)
