<?php
/**
 * This is used with php-src's build/gen_stubs.php to generate argument info compatible with php 7.0 and above.
 *
 * See README.md for function documentation.
 *
 * @generate-class-entries
 */

class SimdJsonException extends RuntimeException {
}

/**
 * Thrown for invalid depths, with similar behavior to php 8.0.
 *
 * NOTE: https://www.php.net/valueerror was added in php 8.0.
 * In older php versions, this extends Error instead.
 */
class SimdJsonValueError extends ValueError {
}
