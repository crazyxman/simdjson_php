--TEST--
Bug #41067 compat test (json_encode() problem with UTF-16 input)
--FILE--
<?php
$json = '["\ud834\udd00"]';
$json_decoded = simdjson_decode($json, true);
// print $json_decoded[0] . "\n";
print bin2hex($json_decoded[0]) . "\n";
print "END\n";
?>
--EXPECT--
f09d8480
END
