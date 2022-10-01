--TEST--
Bug #62010 compat (json_decode produces invalid byte-sequences)
--FILE--
<?php

try {
    var_dump(json_decode('"\ud834"'));
} catch (RuntimeException $e) {
    printf("Caught %s: %s\n", get_class($e), $e->getMessage());
}
?>
--EXPECT--
NULL
bool(true)
string(50) "Single unpaired UTF-16 surrogate in unicode escape"
