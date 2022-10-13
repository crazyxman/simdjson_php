--TEST--
Bug #68546 compat (json_decode() Fatal error: Cannot access property started with '\0')
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
var_export(simdjson_decode('{"key": {"\u0000": "aa"}}', true));
echo "\n";

echo "Done\n";
?>
--EXPECT--
Caught SimdJsonException: 255 Invalid property name
Caught SimdJsonException: 255 Invalid property name
Arrays allow empty keys
array (
  'key' =>
  array (
    '' . "\0" . '' => 'aa',
  ),
)
Done