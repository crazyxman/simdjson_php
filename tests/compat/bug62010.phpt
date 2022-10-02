--TEST--
Bug #62010 compat (json_decode produces invalid byte-sequences)
--FILE--
<?php

try {
    var_dump(simdjson_decode('"\ud834"'));
} catch (RuntimeException $e) {
    printf("Caught %s: %s\n", get_class($e), $e->getMessage());
}
?>
--EXPECT--
Caught SimdJsonException: Problem while parsing a string