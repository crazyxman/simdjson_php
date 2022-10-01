--TEST--
Test simdjson_decode() function : error conditions
--FILE--
<?php
echo "*** Testing simdjson_decode() : error conditions ***\n";

echo "\n-- Testing simdjson_decode() function with depth below 0 --\n";

try {
    var_dump(simdjson_decode('"abc"', true, -1));
} catch (Throwable $e) {
    // TODO: Change in a future release?
    printf("simdjson_decode(): %s: %s\n", get_class($e), $e->getMessage());
}
?>
--EXPECT--
*** Testing simdjson_decode() : error conditions ***

-- Testing simdjson_decode() function with depth below 0 --
simdjson_decode(): Argument #3 ($depth) must be greater than 0
