--TEST--
Bug #68546 compat (json_decode() Fatal error: Cannot access property started with '\0')
--SKIPIF--
<?php if (!function_exists('json_encode')) echo "skip output test requires json_encode\n"; ?>
--FILE--
<?php

try {
    var_dump(simdjson_decode('{"key": {"\u0000": "aa"}}'));
} catch (RuntimeException $e) {
    printf("Caught %s: %d %s\n", get_class($e), $e->getCode(), $e->getMessage());
}
try {
    var_dump(simdjson_decode('[{"key1": 0, "\u0000": 1}]'));
} catch (RuntimeException $e) {
    printf("Caught %s: %d %s\n", get_class($e), $e->getCode(), $e->getMessage());
}
echo "Arrays allow empty keys\n";
// var_export has trailing whitespace and var_dump emits a raw null byte.
// Just use json for plain text, it's enabled in CI
echo json_encode(simdjson_decode('{"key": {"\u0000": "aa"}}', true)), "\n";

echo "Done\n";
?>
--EXPECT--
Caught SimdJsonException: 255 Invalid property name
Caught SimdJsonException: 255 Invalid property name
Arrays allow empty keys
{"key":{"\u0000":"aa"}}
Done