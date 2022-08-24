--TEST--
simdjson_decode reject max depth
--FILE--
<?php
declare(strict_types=1);
ini_set('error_reporting', (string)E_ALL);
ini_set('display_errors', 'stderr');

foreach ([1024, PHP_INT_MAX >> 1] as $depth) {
    $value = \simdjson_decode('[]', true, $depth);
    var_dump($value);
    $value = \simdjson_key_count('{"a":"b"}', 'a', $depth);
    var_dump($value);
    $value = \simdjson_key_value('{"a":{}}', 'a', true, $depth);
    var_dump($value);
    $value = \simdjson_is_valid('{}', $depth);
    var_dump($value);
}
?>
--EXPECTF--
array(0) {
}
int(0)
array(0) {
}
bool(true)
Warning: simdjson_decode(): Depth exceeds maximum allowed value of %d in %s on line 7
NULL
Warning: simdjson_key_count(): Depth exceeds maximum allowed value of %d in %s on line 9
NULL
Warning: simdjson_key_value(): Depth exceeds maximum allowed value of %d in %s on line 11
NULL
Warning: simdjson_is_valid(): Depth exceeds maximum allowed value of %d in %s on line 13
NULL
