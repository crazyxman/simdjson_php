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
        $lasterr = 1;
    }
}
var_dump(compat_decode(FALSE));
var_dump($lasterr);
var_dump(compat_decode(""));
var_dump($lasterr);

var_dump(compat_decode(0));
var_dump($lasterr);
var_dump(compat_decode(1));
var_dump($lasterr);
var_dump(compat_decode(TRUE));
var_dump($lasterr);

compat_decode("\xED\xA0\xB4");
var_dump($lasterr);

compat_decode("\x00");
var_dump($lasterr);

compat_decode("\"\xED\xA0\xB4\"");
var_dump($lasterr);

compat_decode("\"\x00\"");
var_dump($lasterr);
?>
--EXPECT--
NULL
int(4)
NULL
int(4)
int(0)
int(0)
int(1)
int(0)
int(1)
int(0)
int(5)
int(3)
int(5)
int(3)
