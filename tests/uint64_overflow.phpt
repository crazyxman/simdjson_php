--TEST--
simdjson_decode uint64_t overflow test
--FILE--
<?php
ini_set('serialize_precision', '20');
var_export(simdjson_decode('9223372036854775808'));
echo "\n";
var_export(simdjson_decode('18446744073709551615'));
echo "\n";
try {
    var_export(simdjson_decode('18446744073709551616'));
} catch (Exception $e) {
    printf("%s: %s\n", get_class($e), $e->getMessage());
}
?>
--EXPECT--
9223372036854775808.0
18446744073709551616.0
18446744073709551616.0
