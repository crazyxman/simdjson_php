# Benchmark

## Build project

Build the project from the project root folder:

```
phpize
./configure
make
make test
```

## Install PHP Composer dependencies

[Install Composer](https://getcomposer.org/download/) if not already done and execute it in the benchmark folder:

```
composer install
```

## Run PHPBench benchmark

Execute from project root folder:

```
php benchmark/vendor/bin/phpbench run --report=table --group decode
```

The output should look like this (Example output is using an optimized php 7.4 NTS build with the simdjson Intel/AMD AVX2 implementation):

```
\SimdjsonBench\DecodeBench

    jsonDecodeAssoc.........................R1 I1 [μ Mo]/r: 0.00388 0.00381 (ms) [μSD μRSD]/r: 0.000ms 2.53%
    jsonDecode..............................R1 I4 [μ Mo]/r: 0.00412 0.00419 (ms) [μSD μRSD]/r: 0.000ms 2.38%
    simdjsonDecodeAssoc.....................R1 I4 [μ Mo]/r: 0.00160 0.00160 (ms) [μSD μRSD]/r: 0.000ms 0.00%
    simdjsonDecode..........................R5 I4 [μ Mo]/r: 0.00200 0.00200 (ms) [μSD μRSD]/r: 0.000ms 0.00%

4 subjects, 20 iterations, 20 revs, 0 rejects, 0 failures, 0 warnings
(best [mean mode] worst) = 1.600 [2.900 2.900] 1.600 (μs)
⅀T: 58.000μs μSD/r 0.049μs μRSD/r: 1.226%
suite: 1348b91b9c795a97081586821cb7ad40fcf92c64, date: 2022-08-17, stime: 00:01:20
+-------------+---------------------+--------+----------+-----------+-----------+-------+
| benchmark   | subject             | groups | mem_peak | mean      | best      | diff  |
+-------------+---------------------+--------+----------+-----------+-----------+-------+
| DecodeBench | simdjsonDecodeAssoc | decode | 597,696b | 0.00160ms | 0.00160ms | 1.00x |
| DecodeBench | simdjsonDecode      | decode | 597,664b | 0.00200ms | 0.00200ms | 1.25x |
| DecodeBench | jsonDecodeAssoc     | decode | 597,664b | 0.00388ms | 0.00380ms | 2.43x |
| DecodeBench | jsonDecode          | decode | 597,664b | 0.00412ms | 0.00400ms | 2.58x |
+-------------+---------------------+--------+----------+-----------+-----------+-------+
```

```
php benchmark/vendor/bin/phpbench run --report=table --group key_value
```

The output should look like this:

```
\SimdjsonBench\KeyValueBench

    jsonDecode..............................R1 I0 [μ Mo]/r: 0.00400 0.00400 (ms) [μSD μRSD]/r: 0.000ms 0.00%
    simdjsonDeepString......................R5 I4 [μ Mo]/r: 0.00080 0.00080 (ms) [μSD μRSD]/r: 0.000ms 0.00%
    simdjsonDeepStringAssoc.................R2 I1 [μ Mo]/r: 0.00080 0.00080 (ms) [μSD μRSD]/r: 0.000ms 0.00%
    simdjsonInt.............................R2 I2 [μ Mo]/r: 0.00060 0.00060 (ms) [μSD μRSD]/r: 0.000ms 0.00%
    simdjsonIntAssoc........................R5 I4 [μ Mo]/r: 0.00080 0.00080 (ms) [μSD μRSD]/r: 0.000ms 0.00%
    simdjsonArray...........................R1 I3 [μ Mo]/r: 0.00100 0.00100 (ms) [μSD μRSD]/r: 0.000ms 0.00%
    simdjsonObject..........................R5 I4 [μ Mo]/r: 0.00100 0.00100 (ms) [μSD μRSD]/r: 0.000ms 0.00%

7 subjects, 35 iterations, 35 revs, 0 rejects, 0 failures, 0 warnings
(best [mean mode] worst) = 0.600 [1.286 1.286] 0.600 (μs)
⅀T: 45.000μs μSD/r 0.000μs μRSD/r: 0.000%
suite: 1348b91bf021f090195aa64f9e32bb9787e2aba7, date: 2022-08-17, stime: 00:00:39
+---------------+-------------------------+-----------+----------+-----------+-----------+-------+
| benchmark     | subject                 | groups    | mem_peak | mean      | best      | diff  |
+---------------+-------------------------+-----------+----------+-----------+-----------+-------+
| KeyValueBench | simdjsonInt             | key_value | 597,680b | 0.00060ms | 0.00060ms | 1.00x |
| KeyValueBench | simdjsonDeepString      | key_value | 597,712b | 0.00080ms | 0.00080ms | 1.33x |
| KeyValueBench | simdjsonDeepStringAssoc | key_value | 597,712b | 0.00080ms | 0.00080ms | 1.33x |
| KeyValueBench | simdjsonIntAssoc        | key_value | 597,712b | 0.00080ms | 0.00080ms | 1.33x |
| KeyValueBench | simdjsonArray           | key_value | 597,680b | 0.00100ms | 0.00100ms | 1.67x |
| KeyValueBench | simdjsonObject          | key_value | 597,680b | 0.00100ms | 0.00100ms | 1.67x |
| KeyValueBench | jsonDecode              | key_value | 597,680b | 0.00400ms | 0.00400ms | 6.67x |
+---------------+-------------------------+-----------+----------+-----------+-----------+-------+
```

```
php benchmark/vendor/bin/phpbench run --report=table --group multiple
```

The output should look like this:

```
\SimdjsonBench\MultipleAccessBench

    simdjsonMultipleAccessSameDocument......R5 I4 [μ Mo]/r: 0.00424 0.00420 (ms) [μSD μRSD]/r: 0.000ms 1.89%
    simdjsonMultipleAccessDifferentDocument.R2 I4 [μ Mo]/r: 0.00472 0.00479 (ms) [μSD μRSD]/r: 0.000ms 2.08%

2 subjects, 10 iterations, 10 revs, 0 rejects, 0 failures, 0 warnings
(best [mean mode] worst) = 4.200 [4.480 4.497] 4.400 (μs)
⅀T: 44.800μs μSD/r 0.089μs μRSD/r: 1.981%
suite: 1348b906f3a3db7db83e25ab05aeb9a8b3091a84, date: 2022-08-16, stime: 23:59:25
+---------------------+-----------------------------------------+----------+----------+-----------+-----------+-------+
| benchmark           | subject                                 | groups   | mem_peak | mean      | best      | diff  |
+---------------------+-----------------------------------------+----------+----------+-----------+-----------+-------+
| MultipleAccessBench | simdjsonMultipleAccessSameDocument      | multiple | 597,784b | 0.00424ms | 0.00420ms | 1.00x |
| MultipleAccessBench | simdjsonMultipleAccessDifferentDocument | multiple | 597,784b | 0.00472ms | 0.00460ms | 1.11x |
+---------------------+-----------------------------------------+----------+----------+-----------+-----------+-------+
```

## Run benchmark

You may also run a simpler standalone benchmark script on the JSON files in `jsonexamples` by running the commands:

```
php -d extension=modules/simdjson.so benchmark/benchmark.php
```

The output should look like this

```
filename|json_decode|simdjson_decode|simdjson_is_valid|relative_decode|relative_is_valid
---|:--:|---:|---:|---:|--:
apache_builds.json|494947|254120|56543|0.51x|0.11x
canada.json|37145417|10838062|3320384|0.29x|0.09x
citm_catalog.json|5874904|2776749|919151|0.47x|0.16x
github_events.json|259100|97268|27053|0.38x|0.10x
gsoc-2018.json|12669856|3819161|1622433|0.30x|0.13x
instruments.json|926302|379006|127161|0.41x|0.14x
marine_ik.json|25719210|13434756|4342874|0.52x|0.17x
mesh.json|6093213|2881514|1037859|0.47x|0.17x
mesh.pretty.json|10560292|2920351|1460824|0.28x|0.14x
numbers.json|940632|293774|191690|0.31x|0.20x
random.json|2962083|1420896|361135|0.48x|0.12x
twitter.json|2398937|961927|323916|0.40x|0.14x
twitterescaped.json|2730841|1077194|498268|0.39x|0.18x
update-center.json|2875567|1128182|293914|0.39x|0.10x
```
