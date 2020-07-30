# Benchmark

## Build project

``` 
phpize
./configure
make
make test
```

## Install PHP Composer dependecies

[Install Composer](https://getcomposer.org/download/) if not already done and execute:

```
composer install
```

## Run benchmark

Execute from project root folder:

```
php benchmark/vendor/bin/phpbench run --report=table
```

The output should look like this:

``` 
$ php benchmark/vendor/bin/phpbench run --report=table
PhpBench @git_tag@. Running benchmarks.
Using configuration file: /home/skeil/data/sources/sandrokeil/simdjson_php/phpbench.json.dist

11 subjects, 55 iterations, 55 revs, 0 rejects, 0 failures, 0 warnings
(best [mean mode] worst) = 2.200 [5.396 5.406] 2.200 (μs)
⅀T: 296.800μs μSD/r 0.055μs μRSD/r: 0.810%
suite: 1343d1aa9acc97af8673633f932bfa31f7e5b0df, date: 2020-07-30, stime: 20:07:58
+---------------+-------------------------+-----------+----------+-----------+-----------+-------+
| benchmark     | subject                 | groups    | mem_peak | mean      | best      | diff  |
+---------------+-------------------------+-----------+----------+-----------+-----------+-------+
| DecodeBench   | simdjsonDecodeAssoc     | decode    | 880,864b | 0.00596ms | 0.00580ms | 2.71x |
| DecodeBench   | simdjsonDecode          | decode    | 880,856b | 0.00672ms | 0.00660ms | 3.05x |
| DecodeBench   | jsonDecodeAssoc         | decode    | 880,856b | 0.00972ms | 0.00960ms | 4.42x |
| DecodeBench   | jsonDecode              | decode    | 880,856b | 0.01044ms | 0.01040ms | 4.75x |
| KeyValueBench | simdjsonInt             | key_value | 884,496b | 0.00220ms | 0.00220ms | 1.00x |
| KeyValueBench | simdjsonIntAssoc        | key_value | 884,504b | 0.00220ms | 0.00220ms | 1.00x |
| KeyValueBench | simdjsonDeepString      | key_value | 884,504b | 0.00240ms | 0.00240ms | 1.09x |
| KeyValueBench | simdjsonDeepStringAssoc | key_value | 884,504b | 0.00240ms | 0.00240ms | 1.09x |
| KeyValueBench | simdjsonArray           | key_value | 884,496b | 0.00348ms | 0.00340ms | 1.58x |
| KeyValueBench | simdjsonObject          | key_value | 884,496b | 0.00400ms | 0.00400ms | 1.82x |
| KeyValueBench | jsonDecode              | key_value | 884,496b | 0.00984ms | 0.00960ms | 4.47x |
+---------------+-------------------------+-----------+----------+-----------+-----------+-------+
```