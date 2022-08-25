--TEST--
simdjson_decode uses strict types
--FILE--
<?php
declare(strict_types = 1);

try {
    var_dump(simdjson_decode(null));
} catch (Error $e) {
    printf("Caught %s: %s\n", get_class($e), $e->getMessage());
}
try {
    var_dump(simdjson_key_exists('{}', null));
} catch (Error $e) {
    printf("Caught %s: %s\n", get_class($e), $e->getMessage());
}
?>
--EXPECTF--
Caught TypeError: %Ssimdjson_decode()%S must be of %Stype string, null given
Caught TypeError: %Ssimdjson_key_exists()%S must be of %Stype string, null given
