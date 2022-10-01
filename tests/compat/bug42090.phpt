--TEST--
Bug #42090 compat (json_decode causes segmentation fault)
--FILE--
<?php
var_dump(
    simdjson_decode('""'),
    simdjson_decode('"..".'),
    simdjson_decode('"'),
    simdjson_decode('""""'),
    simdjson_decode(json_encode('"')),
    simdjson_decode(json_encode('""'))
);
?>
--EXPECT--
string(0) ""
NULL
NULL
NULL
string(1) """
string(2) """"
