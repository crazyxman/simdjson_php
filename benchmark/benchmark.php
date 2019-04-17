<?php
/**
 * Created by PhpStorm.
 * User: wangjinxi
 * Date: 2019/4/16
 * Time: 3:32 AM
 */

$title = "filename|json_decode|simd_json_decode|simd_json_valid\n---|:--:|---:|---:\n";

foreach (glob(__DIR__ . '/data/*.json') as $key=>$item) {

    $jsonString = file_get_contents($item);

    $stime = microtime_float();
    simd_json_decode($jsonString);
    $etime = microtime_float();
    $simdd_time = bcsub($etime, $stime, 8);


    $stime = microtime_float();
    simd_json_valid($jsonString);
    $etime = microtime_float();
    $simdi_time = bcsub($etime, $stime, 8);


    $stime = microtime_float();
    json_decode($jsonString);
    $etime = microtime_float();
    $jsond_time = bcsub($etime, $stime, 8);


    $title.= basename($item)."|{$jsond_time}|{$simdd_time}|$simdi_time\n";

}

echo $title;


function microtime_float()
{
    list($usec, $sec) = explode(" ", microtime());
    return bcadd((float)$usec, (float)$sec, 8);
}