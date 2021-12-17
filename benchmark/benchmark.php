<?php
/**
 * Created by PhpStorm.
 * User: wangjinxi
 * Date: 2019/4/16
 * Time: 3:32 AM
 */

$title = "filename|json_decode|simdjson_decode|simdjson_is_valid\n---|:--:|---:|---:\n";
foreach (glob(__DIR__.'/../jsonexamples/*.json') as $key=>$item) {

    $jsonString = file_get_contents($item);

    $stime = hrtime(true);
    simdjson_decode($jsonString, true);
    $etime = hrtime(true);
    $simdd_time = $etime - $stime;


    $stime = hrtime(true);
    simdjson_is_valid($jsonString);
    $etime = hrtime(true);
    $simdi_time = $etime - $stime;


    $stime = hrtime(true);
    json_decode($jsonString, true);
    $etime = hrtime(true);
    $jsond_time = $etime - $stime;


    $title.= basename($item)."|{$jsond_time}|{$simdd_time}|$simdi_time\n";

}

echo $title;


if (!function_exists('hrtime')) {
    function hrtime(bool $as_number = false)
    {
        return microtime($as_number);
    }
}
