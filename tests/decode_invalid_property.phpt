--TEST--
simdjson_decode throws exception if json object has invalid property
--SKIPIF--
<?php if (PHP_VERSION_ID < 70100) { echo "skip empty string not allowed as property in php 7.0\n"; } ?>
--FILE--
<?php
$json = '{"key":[1],"\u0000*\u0000protected": "test"}';
$jsonRepeatedInvalid = '{"key":{"\u0000*\u0000protected": "test"}, "\u0000*\u0000protected2": "test"}';
$jsonWithEmptyKey = '{"":true}';

// Convert null bytes to octal "\000" for readability
function dump($value) {
    ob_start();
    var_dump($value);
    echo addcslashes(ob_get_clean(), "\\\0");
}
// allowed for arrays
dump(simdjson_decode($json, true));

try {
    simdjson_decode($json);
} catch (\RuntimeException $e) {
    var_dump($e->getMessage());
}

// allowed for arrays
dump(simdjson_decode($jsonRepeatedInvalid, true));

try {
    simdjson_decode($jsonRepeatedInvalid);
} catch (\RuntimeException $e) {
    var_dump($e->getMessage());
}
// The empty key is allowed.
var_dump(simdjson_decode($jsonWithEmptyKey));
var_dump(simdjson_decode($jsonWithEmptyKey, true));

?>
--EXPECT--
array(2) {
  ["key"]=>
  array(1) {
    [0]=>
    int(1)
  }
  ["\000*\000protected"]=>
  string(4) "test"
}
string(21) "Invalid property name"
array(2) {
  ["key"]=>
  array(1) {
    ["\000*\000protected"]=>
    string(4) "test"
  }
  ["\000*\000protected2"]=>
  string(4) "test"
}
string(21) "Invalid property name"
object(stdClass)#1 (1) {
  [""]=>
  bool(true)
}
array(1) {
  [""]=>
  bool(true)
}