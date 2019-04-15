# simdjson_php
php bindings for the simdjson project. https://arxiv.org/abs/1902.08318

### Requirement
* PHP 7.0 +
it is required AVX2 support in order to use all of its powers. You may want to check whether your OS/processor supports it
- OS X: `sysctl -a | grep machdep.cpu.leaf7_features`
- Linux: `grep avx2 /proc/cpuinfo`

### Compile simdjson_php in Linux
```
$/path/to/phpize
$./configure --with-php-config=/path/to/php-config
$make && make install
```

### Router Usage
Here's a basic usage example:
```
$/path/to/phpize
$./configure --with-php-config=/path/to/php-config
$make && make install
```
