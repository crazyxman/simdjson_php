<?php
/**
 * Created by PhpStorm.
 * User: wangjinxi
 * Date: 2019/4/16
 * Time: 3:32 AM
 */

if (!function_exists('hrtime')) {
    function hrtime(bool $as_number = false)
    {
        return microtime($as_number);
    }
}

// Repeat the operation to make benchmark results less random.
const ITERATIONS = 4;

// Print the amount of nanoseconds taken on average for the functions, as well as the relative amount of time taken compared to json_decode
$result = "filename|json_decode|simdjson_decode|simdjson_is_valid|relative_decode|relative_is_valid\n---|:--:|---:|---:|---:|--:\n";
foreach (glob(__DIR__.'/../jsonexamples/*.json') as $item) {

    $jsonString = file_get_contents($item);

    $stime = hrtime(true);
    for ($i = 0; $i < ITERATIONS; $i++) { simdjson_decode($jsonString, true); }
    $etime = hrtime(true);
    $simdd_time = (int)(($etime - $stime) / ITERATIONS);

    $stime = hrtime(true);
    for ($i = 0; $i < ITERATIONS; $i++) { simdjson_is_valid($jsonString); }
    $etime = hrtime(true);
    $simdi_time = (int)(($etime - $stime) / ITERATIONS);

    $stime = hrtime(true);
    for ($i = 0; $i < ITERATIONS; $i++) { json_decode($jsonString, true); }
    $etime = hrtime(true);
    $jsond_time = (int)(($etime - $stime) / ITERATIONS);

    $relative_decode = sprintf('%.2fx', $simdd_time / $jsond_time);
    $relative_is_valid = sprintf('%.2fx', $simdi_time / $jsond_time);
    $result.= basename($item)."|{$jsond_time}|{$simdd_time}|$simdi_time|$relative_decode|$relative_is_valid\n";
}

echo $result;
