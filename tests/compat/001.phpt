--TEST--
Compat simdjson_decode() tests
--FILE--
<?php
function simdjson_compat_decode($v) {
    try {
        return simdjson_decode($v);
    } catch (Exception $e) {
        return null;
    }
}

var_dump(simdjson_compat_decode(""));
var_dump(simdjson_compat_decode("", 1));
var_dump(simdjson_compat_decode("", 0));
var_dump(simdjson_compat_decode(".", 1));
var_dump(simdjson_compat_decode(".", 0));
var_dump(simdjson_compat_decode("<?>"));
var_dump(simdjson_compat_decode(";"));
var_dump(simdjson_compat_decode("руссиш"));
var_dump(simdjson_compat_decode("blah"));
var_dump(simdjson_compat_decode('{ "test": { "foo": "bar" } }'));
var_dump(simdjson_compat_decode('{ "test": { "foo": "" } }'));
var_dump(simdjson_compat_decode('{ "": { "foo": "" } }'));
var_dump(simdjson_compat_decode('{ "": { "": "" } }'));
var_dump(simdjson_compat_decode('{ "": { "": "" }'));
var_dump(simdjson_compat_decode('{ "": "": "" } }'));

?>
--EXPECTF--
NULL
NULL
NULL
NULL
NULL
NULL
NULL
NULL
NULL
object(stdClass)#%d (1) {
  ["test"]=>
  object(stdClass)#%d (1) {
    ["foo"]=>
    string(3) "bar"
  }
}
object(stdClass)#%d (1) {
  ["test"]=>
  object(stdClass)#%d (1) {
    ["foo"]=>
    string(0) ""
  }
}
object(stdClass)#%d (1) {
  [""]=>
  object(stdClass)#%d (1) {
    ["foo"]=>
    string(0) ""
  }
}
object(stdClass)#%d (1) {
  [""]=>
  object(stdClass)#%d (1) {
    [""]=>
    string(0) ""
  }
}
NULL
NULL
