# simdjson_php
simdjson_php bindings for the [simdjson project](https://github.com/lemire/simdjson).

[![Build Status](https://github.com/crazyxman/simdjson_php/actions/workflows/integration.yml/badge.svg?branch=master)](https://github.com/crazyxman/simdjson_php/actions/workflows/integration.yml?query=branch%3Amaster)
[![Build Status (Windows)](https://ci.appveyor.com/api/projects/status/github/crazyxman/simdjson_php?svg=true)](https://ci.appveyor.com/project/crazyxman/simdjson-php)

## Requirement
- PHP 7 +
- We support platforms like Linux or macOS
- Prerequisites: g++ (version 7 or better) or clang++ (version 6 or better), and a 64-bit system with a command-line shell (e.g., Linux, macOS, freeBSD). We also support programming environments like Visual Studio and Xcode, but different steps are needed

## Installing

### Linux

simdjson may also be installed with the command `pecl install simdjson` (You will need to enable simdjson in php.ini)

Alternately, you may wish to [build from source](#compile-simdjson_php-in-linux).

### MacOS

`pecl install simdjson` is the recommended installation method (You will need to enable simdjson in php.ini)

Alternately, you may wish to [build from source](#compile-simdjson_php-in-linux).

### Installing on Windows

Prebuilt DLLs can be [downloaded from PECL](https://pecl.php.net/package/simdjson) once the [PHP for Windows team fixes hardware issues](https://windows.php.net/).

See https://wiki.php.net/internals/windows/stepbystepbuild_sdk_2#building_pecl_extensions and .appveyor.yml for how to build this, in the meantime.

## Compile simdjson_php in Linux
```
$ phpize
$ ./configure
$ make
$ make test
$ make install
```

Add the following line to your php.ini

```
extension=simdjson.so
```

## simdjson_php Usage
```php
$jsonString = <<<'JSON'
{
  "Image": {
    "Width":  800,
    "Height": 600,
    "Title":  "View from 15th Floor",
    "Thumbnail": {
      "Url":    "http://www.example.com/image/481989943",
      "Height": 125,
      "Width":  100
    },
    "Animated" : false,
    "IDs": [116, 943, 234, 38793, {"p": "30"}]
  }
}
JSON;

//Check if a JSON string is valid:
$isValid = simdjson_is_valid($jsonString); //return bool
var_dump($isValid);  // true

//Parsing a JSON string. similar to the json_decode() function but without the fourth argument
try {
    $parsedJSON = simdjson_decode($jsonString, true, 512); //return array|object|null. "null" string is not a standard json
    var_dump($parsedJSON); // PHP array
} catch (RuntimeException $e) {
    echo "Failed to parse $jsonString: {$e->getMessage()}\n";
}

//note. "/" is a separator. Can be used as the "key" of the object and the "index" of the array
//E.g. "Image/Thumbnail/Url" is ok.


//get the value of a "key" in a json string
$value = simdjson_key_value($jsonString, "Image/Thumbnail/Url");
var_dump($value); // string(38) "http://www.example.com/image/481989943"

$value = simdjson_key_value($jsonString, "Image/IDs/4", true);
var_dump($value);
/*
array(1) {
  ["p"]=>
  string(2) "30"
}
*/

//check if the key exists. return true|false|null. "true" exists, "false" does not exist, "null" string is not a standard json
$res = simdjson_key_exists($jsonString, "Image/IDs/1");
var_dump($res) //bool(true)

// count the values
$res = simdjson_key_count($jsonString, "Image/IDs");
var_dump($res) //int(5)

```

## simdjson_php API

```php
<?php
/**
 * Similar to json_decode()
 *
 * @returns array|stdClass|string|float|int|bool|null
 * @throws RuntimeException for invalid JSON (or document over 4GB, or out of range integer/float)
 */
function simdjson_decode(string $json, bool $assoc = false, int $depth = 512) {}

/**
 * Returns true if json is valid.
 *
 * @returns ?bool (null if depth is invalid)
 */
function simdjson_is_valid(string $json, int $depth = 512) : ?bool {}

/**
 * Parses $json and returns the number of keys in $json matching the JSON pointer $key
 *
 * @returns ?bool (null if depth is invalid)
 */
function simdjson_key_count(string $json, string $key, int $depth = 512) : ?int {}

/**
 * Returns true if the JSON pointer $key could be found.
 *
 * @returns ?bool (null if depth is invalid, false if json is invalid or key is not found)
 */
function simdjson_key_exists(string $json, string $key, int $depth = 512) : ?bool {}

/**
 * Returns the value at $key
 *
 * @returns array|stdClass|string|float|int|bool|null the value at $key
 * @throws RuntimeException for invalid JSON (or document over 4GB, or out of range integer/float)
 */
function simdjson_key_value(string $json, string $key, bool $assoc = unknown, int $depth = unknown) {}
```

## Edge cases

There are some differences from `json_decode()` due to the implementation of the underlying simdjson library. This will throw a RuntimeException if simdjson rejects the JSON.

1) **Until simdjson 2.1.0,** `simdjson_decode()` differed in how out of range 64-bit integers and floats are handled.

See https://github.com/simdjson/simdjson/blob/master/doc/basics.md#standard-compliance

> - The specification allows implementations to set limits on the range and precision of numbers accepted.  We support 64-bit floating-point numbers as well as integer values.
>   - We parse integers and floating-point numbers as separate types which allows us to support all signed (two's complement) 64-bit integers, like a Java `long` or a C/C++ `long long` and all 64-bit unsigned integers. When we cannot represent exactly an integer as a signed or unsigned 64-bit value, we reject the JSON document.
>   - We support the full range of 64-bit floating-point numbers (binary64). The values range from `std::numeric_limits<double>::lowest()`  to `std::numeric_limits<double>::max()`, so from -1.7976e308 all the way to 1.7975e308. Extreme values (less or equal to -1e308, greater or equal to 1e308) are rejected: we refuse to parse the input document. Numbers are parsed with a perfect accuracy (ULP 0): the nearest floating-point value is chosen, rounding to even when needed. If you serialized your floating-point numbers with 17 significant digits in a standard compliant manner, the simdjson library is guaranteed to recover the same numbers, exactly.

2) The maximum string length that can be passed to `simdjson_decode()` is 4GiB (4294967295 bytes).
`json_decode()` can decode longer strings.

3) The handling of max depth is counted slightly differently for empty vs non-empty objects/arrays.
In `json_decode`, an array with a scalar has the same depth as an array with no elements.
In `simdjson_decode`, an array with a scalar is one level deeper than an array with no elements.
For typical use cases, this shouldn't matter.
(e.g. `simdjson_decode('[[]]', true, 2)` will succeed but `json_decode('[[]]', true, 2)` and `simdjson_decode('[[1]]', true, 2)` will fail.)

## Benchmarks
See the [benchmark](./benchmark) folder for more benchmarks.
