# simdjson_php
php bindings for the simdjson project. https://arxiv.org/abs/1902.08318

### Requirement
* PHP 7.0 +
* required C++17 support
* it is required AVX2 support in order to use all of its powers. You may want to check whether your OS/processor supports it
  - OS X: `sysctl -a | grep machdep.cpu.leaf7_features`
  - Linux: `grep avx2 /proc/cpuinfo`

### Compile simdjson_php in Linux
```
$/path/to/phpize
$./configure --with-php-config=/path/to/php-config
$make && make install
```

### simdjson_php Usage
```
//Check if a JSON string is valid:
$isValid = simdjson_isvalid($jsonString);

//Parsing a JSON string
$parsedJSON = simdjson_decode($jsonString);
```

### Benchmarks 
