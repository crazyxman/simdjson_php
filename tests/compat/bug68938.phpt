--TEST--
Bug #68938 compat (json_decode() decodes empty string without indicating error)
--FILE--
<?php
try {
    simdjson_decode("");
} catch (RuntimeException $e) {
    printf("Caught %s: %s\n", get_class($e), $e->getMessage());
}
?>
--EXPECT--
Caught RuntimeException: Empty: no JSON found
