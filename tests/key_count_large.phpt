--TEST--
simdjson_key_count large value test
--SKIPIF--
<?php
if (!function_exists('json_encode')) { echo "skip json_encode not available\n"; }
if (PHP_INT_SIZE < 8) { echo "skip maybe not enough continuous memory for buffers to decode 200MB of json\n"; }
if (!($_ENV['SIMDJSON_HIGH_MEMORY_TESTS'] ?? null)) { echo "skip requires SIMDJSONHIGH_MEMORY_TESTS=1\n"; }
?>
--INI--
memory_limit=2G
--FILE--
<?php
function my_array_repeat($value, int $n): array {
    // This is too large for array_pad chunk size.
    // Here, we choose a sma
    $result = [];
    for ($i = 0; $i < $n; $i++) {
        $result[] = $value;
    }
    return $result;
}
function dump_count(array $x, int $flags = 0) {
    // To reduce the amount of memory needed by this test:
    // 1. Use the smallest possible value for the repeated array
    // 2. Free the last reference to the array before using simdjson
    $str = json_encode(['x' => $x], $flags);
    unset($x);
    var_dump(simdjson_key_count($str, "x"));
}
dump_count([]);
dump_count([1 => '2']);
dump_count(my_array_repeat(1, 0x1000000), JSON_FORCE_OBJECT);
dump_count(my_array_repeat(1, 0x1000000));
dump_count(my_array_repeat(1, 0x0FFFFFF), JSON_FORCE_OBJECT);
dump_count(my_array_repeat(1, 0x0FFFFFF));
?>
--EXPECTF--
int(0)
int(1)
int(16777216)
int(16777216)
int(16777215)
int(16777215)
