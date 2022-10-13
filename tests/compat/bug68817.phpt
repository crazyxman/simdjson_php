--TEST--
Bug #68817 compat (Null pointer deference)
--FILE--
<?php

var_dump(simdjson_decode('[""]'));

?>
--EXPECT--
array(1) {
  [0]=>
  string(0) ""
}
