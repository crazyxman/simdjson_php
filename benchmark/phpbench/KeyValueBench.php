<?php

declare(strict_types=1);

namespace SimdjsonBench;

use PhpBench\Benchmark\Metadata\Annotations\Subject;

if (!extension_loaded('simdjson')) {
        exit;
}

/**
 * @Revs(5)
 * @Iterations(5)
 * @Warmup(3)
 * @OutputTimeUnit("milliseconds", precision=5)
 * @BeforeMethods({"init"})
 * @Groups({"perf"})
 */
class KeyValueBench
{

    /**
     * @var string
     */
    private $json;

    public function init(): void
    {
        $this->json = <<<EOF
{ 
  "result" : [ 
    { 
      "_key" : "70614", 
      "_id" : "products/70614", 
      "_rev" : "_al3hU1K---", 
      "Hello3" : "World3" 
    }, 
    { 
      "_key" : "70616", 
      "_id" : "products/70616", 
      "_rev" : "_al3hU1K--A", 
      "Hello4" : "World4" 
    } 
  ], 
  "hasMore" : false, 
  "count" : 2, 
  "cached" : false, 
  "extra" : { 
    "stats" : { 
      "writesExecuted" : 0, 
      "writesIgnored" : 0, 
      "scannedFull" : 4, 
      "scannedIndex" : 0, 
      "filtered" : 0, 
      "httpRequests" : 0, 
      "executionTime" : 0.00014734268188476562, 
      "peakMemoryUsage" : 2558 
    }, 
    "warnings" : [ ] 
  }, 
  "error" : false, 
  "code" : 201 
}
EOF;
    }

    /**
     * @Subject()
     */
    public function jsonDecode(): void
    {
        $data = json_decode($this->json, true);

        if ('World3' !== $data['result'][0]['Hello3']) {
            throw new \RuntimeException('error');
        }
    }

    /**
     * @Subject()
     */
    public function simdjson()
    {
        $value = \simdjson_key_value($this->json, "result\t0\tHello3", true);

        if ('World3' !== $value) {
            throw new \RuntimeException('error');
        }
    }
}
