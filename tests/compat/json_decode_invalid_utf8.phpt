--TEST--
simdjson_decode() invalid UTF8
--FILE--
<?php
function simdjson_decode_invalid_utf8($str) {
    try {
        var_dump(simdjson_decode($str));
        // no other flags
    } catch (Exception $e) {
        printf("Caught %s: %s\n", get_class($e), $e->getMessage());
    }
}
simdjson_decode_invalid_utf8("\"a\xb0b\"");
simdjson_decode_invalid_utf8("\"a\xd0\xf2b\"");
simdjson_decode_invalid_utf8("\"\x61\xf0\x80\x80\x41\"");
simdjson_decode_invalid_utf8("[\"\xc1\xc1\",\"a\"]");
echo "Done\n";
?>
--EXPECT--
Caught RuntimeException: The input is not valid UTF-8
Caught RuntimeException: The input is not valid UTF-8
Caught RuntimeException: The input is not valid UTF-8
Caught RuntimeException: The input is not valid UTF-8
Done