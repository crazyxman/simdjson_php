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

You may also run a simpler standalone benchmark script on the JSON files in [`jsonexamples`](../jsonexamples) by running the commands:

```
php -d extension=modules/simdjson.so benchmark/benchmark.php
```

For decoding functions, the benchmark includes both the time to decode the data and the time to garbage collect/free the decoded data.

The output should look like this

```
filename|json_decode|simdjson_decode|simdjson_is_valid|relative_decode|relative_is_valid
---|:--:|---:|---:|---:|--:
apache_builds.json|529445|252983|57314|0.48x|0.11x
canada.json|37480906|9114440|3594688|0.24x|0.10x
citm_catalog.json|6211875|3034887|992156|0.49x|0.16x
github_events.json|274541|101767|30350|0.37x|0.11x
gsoc-2018.json|13174491|4000738|1771096|0.30x|0.13x
instruments.json|1051116|396716|133227|0.38x|0.13x
marine_ik.json|25511155|13070490|4951457|0.51x|0.19x
mesh.json|6063492|2246089|1191093|0.37x|0.20x
mesh.pretty.json|10617360|2659494|1634062|0.25x|0.15x
numbers.json|1017109|328689|213022|0.32x|0.21x
random.json|3158095|1526622|402568|0.48x|0.13x
stringifiedphp.json|591071|99104|86034|0.17x|0.15x
twitter.json|2589313|997667|354375|0.39x|0.14x
twitterescaped.json|3089506|1156811|553984|0.37x|0.18x
update-center.json|3084217|1227891|336303|0.40x|0.11x
```

- `canada.json` is an example of a string with a lot of floats (polygon for a map of canada).
  Same for the `mesh*.json` files and `numbers.json` file
- `stringifiedphp.json` is an example of a single long JSON encoded string (representation of php file with newlines and quotes).
- `twitter.json` is an example of decoding data with a mix of types with a lot of non-ascii codepoints.
  `twitterescaped.json` is the same data with `"\uXXXX"` escaping and no whitespace.
- `random.json` is a large object with a lot of short keys, small objects/arrays, and short string/integer values, with some non-ASCII values.
  `apache_builds.json` contains a lot of whitespace and relatively small objects, string keys, and mostly string values.
