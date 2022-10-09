--TEST--
simdjson_decode throws exception if json is invalid
--FILE--
<?php
$json = "Invalid JSON string";

try {
    \simdjson_decode($json, true);
} catch (\RuntimeException $e) {
    var_dump($e->getMessage());
}

?>
--EXPECTF--
string(102) "The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc."