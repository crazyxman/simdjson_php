--TEST--
simdjson_key_count throws exception if key was not found test
--FILE--
<?php
$json = file_get_contents(__DIR__ . DIRECTORY_SEPARATOR . '_files' . DIRECTORY_SEPARATOR . 'result.json');

try {
    \simdjson_key_count($json, "unknown", 1);
} catch (\RuntimeException $e) {
    var_dump($e->getMessage());
}

try {
    \simdjson_key_count($json, "unknown");
} catch (\RuntimeException $e) {
    var_dump($e->getMessage());
}

?>
--EXPECT--
string(67) "The JSON document was too deep (too many nested objects and arrays)"
string(56) "The JSON field referenced does not exist in this object."