<?php

declare(strict_types=1);

namespace SimdjsonBench;

use PhpBench\Benchmark\Metadata\Annotations\Subject;

if (!extension_loaded('simdjson')) {
        exit;
}

/**
 * This tests decoding an array with 100 objects where keys are single-char strings and empty strings.
 *
 * In simdjson, those are optimized for memory usage in php 7.2+ by using php's interned string constants instead of brand new strings.
 * (json_decode doesn't do that but could - when using the simdjson C module it's quicker since the length is known before we need to allocate memory for the internal PHP string representation (zend_string))
 *
 * Note that in this benchmark, PHP garbage collecting the array returned by simdjson_decode can skip over the array keys,
 * because none of the keys were reference counted.
 *
 * @Revs(5)
 * @Iterations(5)
 * @Warmup(3)
 * @OutputTimeUnit("milliseconds", precision=5)
 * @BeforeMethods({"init"})
 * @Groups({"decode"})
 */
class SingleCharStringsBench
{

    /**
     * @var string
     */
    private $json;

    public function init(): void
    {
        $raw = [];
        for ($i = 0; $i < 100; $i++) {
            $raw[] = ['x' => chr(48 + $i % 10), 'y' => chr(48 + $i % 8), 's' => ''];
        }
        $this->json = \json_encode($raw);
    }

    /**
     * @Subject()
     */
    public function jsonDecodeAssoc(): void
    {
        $data = \json_decode($this->json, true);

        if ('0' !== $data[0]['x']) {
            throw new \RuntimeException('error');
        }
    }

    /**
     * @Subject()
     */
    public function jsonDecode(): void
    {
        $data = \json_decode($this->json, false);

        if ('0' !== $data[0]->x) {
            throw new \RuntimeException('error');
        }
    }

    /**
     * @Subject()
     */
    public function simdjsonDecodeAssoc()
    {
        $data = \simdjson_decode($this->json, true);

        if ('0' !== $data[0]['x']) {
            throw new \RuntimeException('error');
        }
    }

    /**
     * @Subject()
     */
    public function simdjsonDecode()
    {
        $data = \simdjson_decode($this->json, false);

        if ('0' !== $data[0]->x) {
            throw new \RuntimeException('error');
        }
    }

}
