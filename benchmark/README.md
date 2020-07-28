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

\SimdjsonBench\KeyValueBench

    jsonDecode..............................R2 I3 [μ Mo]/r: 0.00960 0.00949 (ms) [μSD μRSD]/r: 0.000ms 1.86%
    simdjson................................R1 I2 [μ Mo]/r: 0.00360 0.00360 (ms) [μSD μRSD]/r: 0.000ms 0.00%

2 subjects, 10 iterations, 10 revs, 0 rejects, 0 failures, 0 warnings
(best [mean mode] worst) = 3.600 [6.600 6.543] 3.600 (μs)
⅀T: 66.000μs μSD/r 0.089μs μRSD/r: 0.932%
suite: 1343d187ed9001f1c88f28338b0ad07bc4fc1b44, date: 2020-07-28, stime: 21:10:10
+---------------+------------+----------+-----------+-----------+-------+
| benchmark     | subject    | mem_peak | mean      | best      | diff  |
+---------------+------------+----------+-----------+-----------+-------+
| KeyValueBench | simdjson   | 877,352b | 0.00360ms | 0.00360ms | 1.00x |
| KeyValueBench | jsonDecode | 877,392b | 0.00960ms | 0.00940ms | 2.67x |
+---------------+------------+----------+-----------+-----------+-------+
```