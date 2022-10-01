--TEST--
simdjson_decode uses smaller depth than max_depth when safe to do so
--FILE--
<?php
declare(strict_types=1);
ini_set('error_reporting', (string)E_ALL);
ini_set('display_errors', 'stderr');

// This should only be allocating a few megabytes of memory, not gigabytes,
// due to internally choosing a smaller depth that behaves equivalently for excessively large requested depths.
foreach ([1024, 1 << 27] as $depth) {
    echo "Test depth=$depth:\n";
    $value = simdjson_decode('[]', true, $depth);
    var_dump($value);
    $value = simdjson_key_count('{"a":"b"}', 'a', $depth);
    var_dump($value);
    try {
        simdjson_decode(str_repeat('[', 200000) . str_repeat(']', 199999), true, $depth);
        echo "should be invalid\n";
    } catch (Exception $e) {
        printf("Caught %s: %s\n", get_class($e), $e->getMessage());
    }
}
?>
--EXPECT--
Test depth=1024:
array(0) {
}
int(0)
Caught SimdJsonException: The JSON document was too deep (too many nested objects and arrays)
Test depth=134217728:
array(0) {
}
int(0)
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
