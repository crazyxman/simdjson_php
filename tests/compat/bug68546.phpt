--TEST--
Bug #68546 compat (json_decode() Fatal error: Cannot access property started with '\0')
--FILE--
<?php

try {
    var_dump(json_decode('{"key": {"\u0000": "aa"}}'));
} catch (RuntimeException $e) {
    printf("Caught %s: %s\n", get_class($e), $e->getMessage());
}
try {
    var_dump(json_decode('[{"key1": 0, "\u0000": 1}]'));
} catch (RuntimeException $e) {
    printf("Caught %s: %s\n", get_class($e), $e->getMessage());
}

echo "Done\n";
?>
--EXPECT--
NULL
bool(true)
NULL
bool(true)
string(36) "The decoded property name is invalid"
Done
