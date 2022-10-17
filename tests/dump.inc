<?php
function dump(Closure $c) {
    try {
        var_dump($c());
    } catch (Throwable $e) {
        printf("Caught %s: %s\n", get_class($e), $e->getMessage());
    }
}
