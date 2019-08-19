# simdjson_php
simdjson_php bindings for the simdjson project. https://github.com/lemire/simdjson

### Requirement
- PHP 7.0 +
- We support platforms like Linux or macOS
- A processor with 
  - AVX2 (i.e., Intel processors starting with the Haswell microarchitecture released 2013 and AMD processors starting with the Zen microarchitecture released 2017), 
  - or SSE 4.2 and CLMUL (i.e., Intel processors going back to Westmere released in 2010 or AMD processors starting with the Jaguar used in the PS4 and XBox One)
  - or a 64-bit ARM processor (ARMv8-A): this covers a wide range of mobile processors, including all Apple processors currently available for sale, going back as far back as the iPhone 5s (2013).
- A recent C++ compiler (e.g., GNU GCC or LLVM CLANG ), we assume C++17. GNU GCC 7 or better or LLVM's clang 6 or better.
- Some benchmark scripts assume bash and other common utilities, but they are optional.
- You may want to check whether your OS/processor supports it
  - OS X: `sysctl -a | grep machdep.cpu.leaf7_features`
  - Linux: `grep avx2 /proc/cpuinfo`

### Compile simdjson_php in Linux
```
$/path/to/phpize
$./configure --with-php-config=/path/to/php-config
$make && make install
```

### simdjson_php Usage
```php

//Check if a JSON string is valid:
$isValid = simdjson_isvalid($jsonString); //return bool

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

//note. "\t" is a separator. It must be a control character. Can be used as the "key" of the object and the "index" of the array
//E.g. "Image\tThumbnail\tUrl" is ok. 'Image\tThumbnail\tUrl' is wrong


//get the value of a "key" in a json string
$value = simdjson_key_value($jsonString, "Image\tThumbnail\tUrl");
var_dump($value); // string(38) "http://www.example.com/image/481989943"

$value = simdjson_key_value($jsonString, "Image\tIDs\t4", true);
var_dump($value); 
/*
array(1) {
  ["p"]=>
  string(2) "30"
}
*/

//Get the parsed json resource
$resource = simdjson_resource($jsonString);
//get the value of a "key" in 'resource'
$value = simdjson_key_value($resource, "Image\tThumbnail\tUrl");
var_dump($value); // string(38) "http://www.example.com/image/481989943"

$value = simdjson_key_value($resource, "Image\tIDs\t4", true);
var_dump($value); 
/*
array(1) {
  ["p"]=>
  string(2) "30"
}
*/

//check if the key exists. return true|false|null. "true" exists, "false" does not exist, "null" string is not a standard json
$res = simdjson_key_exists($jsonString, "Image\tIDs\t1");
var_dump($res) //bool(true)

```

### Benchmarks (sec)
filename|json_decode|simdjson_decode|simdjson_isvalid
---|:--:|---:|---:
apache_builds.json|0.00307300|0.00225200|0.00018100
canada.json|0.13955000|0.02773900|0.00358300
citm_catalog.json|0.03030900|0.01334000|0.00117000
github_events.json|0.00294100|0.00090400|0.00008500
gsoc-2018.json|0.04292500|0.01112000|0.00186700
instruments.json|0.00509700|0.00231800|0.00017500
marine_ik.json|0.09833600|0.04417500|0.00463400
mesh.json|0.01869200|0.00722600|0.00114800
mesh.pretty.json|0.03576200|0.00738100|0.00163400
numbers.json|0.00263600|0.00069900|0.00018200
random.json|0.01713500|0.00973900|0.00063000
twitter.json|0.01258600|0.00618400|0.00057400
twitterescaped.json|0.01435900|0.00650400|0.00074300
update-center.json|0.01506000|0.00869100|0.00047800

You may run the benchmarks by running the commands:
* php benchmark/benchmark.php
