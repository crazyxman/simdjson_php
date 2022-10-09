--TEST--
simdjson_key_count test
--FILE--
<?php
$json = file_get_contents(__DIR__ . DIRECTORY_SEPARATOR . '_files' . DIRECTORY_SEPARATOR . 'result.json');
$value = \simdjson_key_count($json, "result");
var_dump($value);

$value = \simdjson_key_count($json, "extra/stats");
var_dump($value);

?>
--EXPECTF--
int(2)
int(8)