--TEST--
Bug #69187 compat json_last_error return BC in PHP7
--FILE--
<?php
function compat_decode($value) {
    global $lasterr;
    $lasterr = 0;
    try {
        return simdjson_decode($value);
    } catch (RuntimeException $e) {
        return sprintf("%s: %s", get_class($e), $e->getMessage());
    }
}
var_dump(compat_decode(FALSE));
var_dump(compat_decode(""));

var_dump(compat_decode(0));
var_dump(compat_decode(1));
var_dump(compat_decode(TRUE));

compat_decode("\xED\xA0\xB4");

compat_decode("\x00");
var_dump($lasterr);

compat_decode("\"\xED\xA0\xB4\"");
var_dump($lasterr);

compat_decode("\"\x00\"");
var_dump($lasterr);
?>
--EXPECT--
string(38) "RuntimeException: Empty: no JSON found"
string(38) "RuntimeException: Empty: no JSON found"
int(0)
int(1)
int(1)
int(0)
int(0)
int(0)