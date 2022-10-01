--TEST--
simdjson_decode throws for integer syntax out of signed/unsigned 64-bit range due to C simdjson library
--SKIPIF--
<?php if (PHP_INT_SIZE < 8) echo "skip 64-bit test only\n"; ?>
--INI--
; in php 8.0 var_dump started using serialize_precision instead of precision
serialize_precision=20
precision=20
--FILE--
<?php
// https://github.com/simdjson/simdjson/blob/master/doc/basics.md#standard-compliance
// > - The specification allows implementations to set limits on the range and precision of numbers accepted.  We support 64-bit floating-point numbers as well as integer values.
// >   - We parse integers and floating-point numbers as separate types which allows us to support all signed (two's complement) 64-bit integers, like a Java `long` or a C/C++ `long long` and all 64-bit unsigned integers. When we cannot represent exactly an integer as a signed or unsigned 64-bit value, we reject the JSON document.
// >   - We support the full range of 64-bit floating-point numbers (binary64). The values range from `std::numeric_limits<double>::lowest()`  to `std::numeric_limits<double>::max()`, so from -1.7976e308 all the way to 1.7975e308. Extreme values (less or equal to -1e308, greater or equal to 1e308) are rejected: we refuse to parse the input document. Numbers are parsed with a perfect accuracy (ULP 0): the nearest floating-point value is chosen, rounding to even when needed. If you serialized your floating-point numbers with 17 significant digits in a standard compliant manner, the simdjson library is guaranteed to recover the same numbers, exactly.
function dump_result(string $x) {
    echo "Testing " . var_export($x, true) . "\n";
    try {
        var_dump(simdjson_decode($x));
    } catch (Exception $e) {
        printf("Caught %s: %s\n", get_class($e), $e->getMessage());
    }
}
dump_result('18446744073709551615');
dump_result('18446744073709551615.0');
dump_result('18446744073709551615E0');
dump_result('18446744073709551616'); // simdjson_decode throws but json_decode doesn't.
dump_result('18446744073709551616.0');
dump_result('-9223372036854775808');
dump_result('-9223372036854775809');
dump_result('-9223372036854775809.0');
dump_result('1e307');
dump_result('1e309');
?>
--EXPECT--
Testing '18446744073709551615'
float(18446744073709551616)
Testing '18446744073709551615.0'
float(18446744073709551616)
Testing '18446744073709551615E0'
float(18446744073709551616)
Testing '18446744073709551616'
float(18446744073709551616)
Testing '18446744073709551616.0'
float(18446744073709551616)
Testing '-9223372036854775808'
int(-9223372036854775808)
Testing '-9223372036854775809'
float(-9223372036854775808)
Testing '-9223372036854775809.0'
float(-9223372036854775808)
Testing '1e307'
float(9.9999999999999998603E+306)
Testing '1e309'
float(INF)
