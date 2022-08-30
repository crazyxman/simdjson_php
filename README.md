# simdjson_php
simdjson_php bindings for the [simdjson project](https://github.com/lemire/simdjson).

[![Build Status](https://github.com/crazyxman/simdjson_php/actions/workflows/integration.yml/badge.svg?branch=master)](https://github.com/crazyxman/simdjson_php/actions/workflows/integration.yml?query=branch%3Amaster)

## Requirement
- PHP 7 +
- We support platforms like Linux or macOS
- Prerequisites: g++ (version 7 or better) or clang++ (version 6 or better), and a 64-bit system with a command-line shell (e.g., Linux, macOS, freeBSD). We also support programming environments like Visual Studio and Xcode, but different steps are needed

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

//Check if a JSON string is valid:
$isValid = simdjson_is_valid($jsonString); //return bool

//Parsing a JSON string. similar to the json_decode() function but without the fourth argument
$parsedJSON = simdjson_decode($jsonString, true, 512); //return array|object|null. "null" string is not a standard json

/*
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
*/

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

## Benchmarks
See the [benchmark](./benchmark) folder for more benchmarks.
