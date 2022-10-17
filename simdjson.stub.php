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
 * Thrown for error conditions on fields such as $depth that are not expected to be
 * from user-provided JSON, with similar behavior to php 8.0.
 *
 * NOTE: https://www.php.net/valueerror was added in php 8.0.
 * In older php versions, this extends Error instead.
 *
 * When support for php 8.0 is dropped completely,
 * a major release of simdjson will likely switch to a standard ValueError.
 */
class SimdJsonValueError extends ValueError {
}
