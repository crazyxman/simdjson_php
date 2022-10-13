--TEST--
JSON compat (http://www.crockford.com/JSON/JSON_checker/test/fail*.json)
--FILE--
<?php

$tests = array(
    '"A JSON payload should be an object or array, not a string."',
    '["Unclosed array"',
    '{unquoted_key: "keys must be quoted}',
    '["extra comma",]',
    '["double extra comma",,]',
    '[   , "<-- missing value"]',
    '["Comma after the close"],',
    '["Extra close"]]',
    '{"Extra comma": true,}',
    '{"Extra value after close": true} "misplaced quoted value"',
    '{"Illegal expression": 1 + 2}',
    '{"Illegal invocation": alert()}',
    '{"Numbers cannot have leading zeroes": 013}',
    '{"Numbers cannot be hex": 0x14}',
    '["Illegal backslash escape: \\x15"]',
    '["Illegal backslash escape: \\\'"]',
    '["Illegal backslash escape: \\017"]',
    '[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[["Too deep"]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]',
    '{"Missing colon" null}',
    '{"Double colon":: null}',
    '{"Comma instead of colon", null}',
    '["Colon instead of comma": false]',
    '["Bad value", truth]',
    "['single quote']"
);

foreach ($tests as $test) {
    echo 'Testing: ' . $test . "\n";
    echo "AS OBJECT\n";
    try {
        var_dump(simdjson_decode($test));
    } catch (SimdJsonException $e) {
        printf("Caught %s: %s\n", get_class($e), $e->getMessage());
    }
    echo "AS ARRAY\n";
    try {
        var_dump(simdjson_decode($test, true));
    } catch (SimdJsonException $e) {
        printf("Caught %s: %s\n", get_class($e), $e->getMessage());
    }
}

?>
--EXPECT--
Testing: "A JSON payload should be an object or array, not a string."
AS OBJECT
string(58) "A JSON payload should be an object or array, not a string."
AS ARRAY
string(58) "A JSON payload should be an object or array, not a string."
Testing: ["Unclosed array"
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: {unquoted_key: "keys must be quoted}
AS OBJECT
Caught SimdJsonException: A string is opened, but never closed.
AS ARRAY
Caught SimdJsonException: A string is opened, but never closed.
Testing: ["extra comma",]
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: ["double extra comma",,]
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: [   , "<-- missing value"]
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: ["Comma after the close"],
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: ["Extra close"]]
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: {"Extra comma": true,}
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: {"Extra value after close": true} "misplaced quoted value"
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: {"Illegal expression": 1 + 2}
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: {"Illegal invocation": alert()}
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: {"Numbers cannot have leading zeroes": 013}
AS OBJECT
Caught SimdJsonException: Problem while parsing a number
AS ARRAY
Caught SimdJsonException: Problem while parsing a number
Testing: {"Numbers cannot be hex": 0x14}
AS OBJECT
Caught SimdJsonException: Problem while parsing a number
AS ARRAY
Caught SimdJsonException: Problem while parsing a number
Testing: ["Illegal backslash escape: \x15"]
AS OBJECT
Caught SimdJsonException: Problem while parsing a string
AS ARRAY
Caught SimdJsonException: Problem while parsing a string
Testing: ["Illegal backslash escape: \'"]
AS OBJECT
Caught SimdJsonException: Problem while parsing a string
AS ARRAY
Caught SimdJsonException: Problem while parsing a string
Testing: ["Illegal backslash escape: \017"]
AS OBJECT
Caught SimdJsonException: Problem while parsing a string
AS ARRAY
Caught SimdJsonException: Problem while parsing a string
Testing: [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[["Too deep"]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
AS OBJECT
Caught SimdJsonException: The JSON document was too deep (too many nested objects and arrays)
AS ARRAY
Caught SimdJsonException: The JSON document was too deep (too many nested objects and arrays)
Testing: {"Missing colon" null}
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: {"Double colon":: null}
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: {"Comma instead of colon", null}
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: ["Colon instead of comma": false]
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
Testing: ["Bad value", truth]
AS OBJECT
Caught SimdJsonException: Problem while parsing an atom starting with the letter 't'
AS ARRAY
Caught SimdJsonException: Problem while parsing an atom starting with the letter 't'
Testing: ['single quote']
AS OBJECT
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.
AS ARRAY
Caught SimdJsonException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.