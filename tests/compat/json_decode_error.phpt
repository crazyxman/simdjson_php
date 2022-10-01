--TEST--
Test simdjson_decode() function : error conditions
--FILE--
<?php
try {
    var_dump(simdjson_decode('"abc"', true, -1));
} catch (Throwable $e) {
    // TODO: Change in a future release?
    printf("simdjson_decode(): %s: %s\n", get_class($e), $e->getMessage());
}
?>
--EXPECTF--
Warning: simdjson_decode(): Depth must be greater than zero in %s on line 3
NULL
