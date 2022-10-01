--TEST--
Compat simdjson_decode() tests
--FILE--
<?php

var_dump(simdjson_decode(""));
var_dump(simdjson_decode("", 1));
var_dump(simdjson_decode("", 0));
var_dump(simdjson_decode(".", 1));
var_dump(simdjson_decode(".", 0));
var_dump(simdjson_decode("<?>"));
var_dump(simdjson_decode(";"));
var_dump(simdjson_decode("руссиш"));
var_dump(simdjson_decode("blah"));
var_dump(simdjson_decode('{ "test": { "foo": "bar" } }'));
var_dump(simdjson_decode('{ "test": { "foo": "" } }'));
var_dump(simdjson_decode('{ "": { "foo": "" } }'));
var_dump(simdjson_decode('{ "": { "": "" } }'));
var_dump(simdjson_decode('{ "": { "": "" }'));
var_dump(simdjson_decode('{ "": "": "" } }'));

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
