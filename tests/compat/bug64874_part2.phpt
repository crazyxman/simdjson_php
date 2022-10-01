--TEST--
Case-sensitivity part of bug #64874 compat ("json_decode handles whitespace and case-sensitivity incorrectly")
--FILE--
<?php
function decode($json) {
    try {
        var_dump(json_decode($json));
    } catch (RuntimeException $e) {
        printf("Caught %s: %s\n", get_class($e), $e->getMessage());
    }
}

// Only lowercase should work
decode('true');
decode('True');
decode('[true]');
decode('[True]');
echo PHP_EOL;

decode('false');
decode('False');
decode('[false]');
decode('[False]');
echo PHP_EOL;

decode('null');
decode('Null');
decode('[null]');
decode('[Null]');
echo PHP_EOL;

echo "Done\n";
?>
--EXPECT--
bool(true)
SUCCESS
NULL
ERROR
array(1) {
  [0]=>
  bool(true)
}
SUCCESS
NULL
ERROR

bool(false)
SUCCESS
NULL
ERROR
array(1) {
  [0]=>
  bool(false)
}
SUCCESS
NULL
ERROR

NULL
SUCCESS
NULL
ERROR
array(1) {
  [0]=>
  NULL
}
SUCCESS
NULL
ERROR

Done
