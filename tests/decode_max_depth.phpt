--TEST--
simdjson_decode reject max depth
--FILE--
<?php
declare(strict_types=1);
ini_set('error_reporting', (string)E_ALL);
ini_set('display_errors', 'stderr');
require_once __DIR__ . '/dump.inc';

foreach ([0, PHP_INT_MIN, 1024, PHP_INT_MAX >> 1, PHP_INT_MAX] as $depth) {
    dump(function () use ($depth) { return simdjson_decode('[]', true, $depth); });
    dump(function () use ($depth) { return simdjson_key_count('{"a":"b"}', 'a', $depth); });
    dump(function () use ($depth) { return simdjson_key_value('{"a":{}}', 'a', true, $depth); });
    dump(function () use ($depth) { return simdjson_key_exists('{"a":{}}', 'a', $depth); });
    dump(function () use ($depth) { return simdjson_is_valid('{}', $depth); });
}
?>
--EXPECTF--
Caught SimdJsonValueError: simdjson_decode(): Argument #2 ($depth) must be greater than zero
Caught SimdJsonValueError: simdjson_key_count(): Argument #4 ($depth) must be greater than zero
Caught SimdJsonValueError: simdjson_key_value(): Argument #4 ($depth) must be greater than zero
Caught SimdJsonValueError: simdjson_key_exists(): Argument #3 ($depth) must be greater than zero
Caught SimdJsonValueError: simdjson_is_valid(): Argument #2 ($depth) must be greater than zero
Caught SimdJsonValueError: simdjson_decode(): Argument #2 ($depth) must be greater than zero
Caught SimdJsonValueError: simdjson_key_count(): Argument #4 ($depth) must be greater than zero
Caught SimdJsonValueError: simdjson_key_value(): Argument #4 ($depth) must be greater than zero
Caught SimdJsonValueError: simdjson_key_exists(): Argument #3 ($depth) must be greater than zero
Caught SimdJsonValueError: simdjson_is_valid(): Argument #2 ($depth) must be greater than zero
array(0) {
}
int(0)
array(0) {
}
bool(true)
bool(true)
Caught SimdJsonValueError: simdjson_decode(): Argument #2 ($depth) exceeds maximum allowed value of %d
Caught SimdJsonValueError: simdjson_key_count(): Argument #4 ($depth) exceeds maximum allowed value of %d
Caught SimdJsonValueError: simdjson_key_value(): Argument #4 ($depth) exceeds maximum allowed value of %d
Caught SimdJsonValueError: simdjson_key_exists(): Argument #3 ($depth) exceeds maximum allowed value of %d
Caught SimdJsonValueError: simdjson_is_valid(): Argument #2 ($depth) exceeds maximum allowed value of %d
Caught SimdJsonValueError: simdjson_decode(): Argument #2 ($depth) exceeds maximum allowed value of %d
Caught SimdJsonValueError: simdjson_key_count(): Argument #4 ($depth) exceeds maximum allowed value of %d
Caught SimdJsonValueError: simdjson_key_value(): Argument #4 ($depth) exceeds maximum allowed value of %d
Caught SimdJsonValueError: simdjson_key_exists(): Argument #3 ($depth) exceeds maximum allowed value of %d
Caught SimdJsonValueError: simdjson_is_valid(): Argument #2 ($depth) exceeds maximum allowed value of %d
