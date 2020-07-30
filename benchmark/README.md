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

7 subjects, 35 iterations, 35 revs, 0 rejects, 0 failures, 0 warnings
(best [mean mode] worst) = 3.000 [4.394 4.390] 3.000 (μs)
⅀T: 153.800μs μSD/r 0.082μs μRSD/r: 1.806%
suite: 1343d1aa6c27e67a8914ed5d1a02238284fcfce5, date: 2020-07-30, stime: 14:53:00
+---------------+-----------------------------+----------+-----------+-----------+-------+
| benchmark     | subject                     | mem_peak | mean      | best      | diff  |
+---------------+-----------------------------+----------+-----------+-----------+-------+
| KeyValueBench | simdjsonIntViaObject        | 884,552b | 0.00300ms | 0.00300ms | 1.00x |
| KeyValueBench | simdjsonIntViaArray         | 884,552b | 0.00300ms | 0.00300ms | 1.00x |
| KeyValueBench | simdjsonDeepStringViaObject | 884,560b | 0.00308ms | 0.00300ms | 1.03x |
| KeyValueBench | simdjsonDeepStringViaArray  | 884,560b | 0.00312ms | 0.00300ms | 1.04x |
| KeyValueBench | simdjsonArray               | 884,544b | 0.00408ms | 0.00400ms | 1.36x |
| KeyValueBench | simdjsonObject              | 884,544b | 0.00468ms | 0.00460ms | 1.56x |
| KeyValueBench | jsonDecode                  | 884,544b | 0.00980ms | 0.00960ms | 3.27x |
+---------------+-----------------------------+----------+-----------+-----------+-------+
```