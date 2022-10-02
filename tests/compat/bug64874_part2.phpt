--TEST--
Case-sensitivity part of bug #64874 compat ("json_decode handles whitespace and case-sensitivity incorrectly")
--FILE--
<?php
function decode($json) {
    try {
        var_dump(simdjson_decode($json));
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
Caught RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
array(1) {
  [0]=>
  bool(true)
}
Caught RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.

bool(false)
Caught RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
array(1) {
  [0]=>
  bool(false)
}
Caught RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.

NULL
Caught RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
array(1) {
  [0]=>
  NULL
}
Caught RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.

Done