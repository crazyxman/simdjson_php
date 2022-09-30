<?php

function compare_decode_result(string $filename) {
    echo "$filename: ";
    $contents = file_get_contents($filename);
    try {
        $jsonResult = json_decode($contents, true, 512, JSON_THROW_ON_ERROR);
    } catch (Exception $e) {
        $jsonError = $e;
    }
    try {
        $simdjsonResult = simdjson_decode($contents, true);
    } catch (Exception $e) {
        $simdjsonError = $e;
    }
    if (isset($jsonError)) {
        if (isset($simdjsonError)) {
            return sprintf('same(both throw): JSON(%s: %s) SIMDJSON(%s: %s)',
                get_class($jsonError), $jsonError->getMessage(),
                get_class($simdjsonError), $simdjsonError->getMessage());
        }
        return sprintf('ERROR: json threw(%s %s) but simdjson did not throw',
                get_class($jsonError), $jsonError->getMessage());
    }

    if (isset($simdjsonError)) {
        return sprintf('STRICTER: simdjson threw(%s %s) but json did not throw',
                get_class($simdjsonError), $simdjsonError->getMessage());
    }

    if ($simdjsonResult === $jsonResult) {
        return "same result";
    }
    return sprintf("ERROR: different result: SIMDJSON(%s) !== JSON(%s)", var_export($simdjsonResult, true), var_export($jsonResult, true));
}

chdir(dirname(__DIR__) . '/jsonexamples/test_parsing');
$files = glob('*.json');
sort($files);
foreach ($files as $filename) {
    $result = compare_decode_result($filename);
    echo "$filename:\n=> $result\n";
}
echo "Done comparing json to see if simdjson_decode() returns any values json_decode() would return or throw for\n";
/*
i_number_double_huge_neg_exp.json: i_number_double_huge_neg_exp.json:
=> same result
i_number_huge_exp.json: i_number_huge_exp.json:
=> STRICTER: simdjson threw(RuntimeException Problem while parsing a number) but json did not throw
i_number_neg_int_huge_exp.json: i_number_neg_int_huge_exp.json:
=> STRICTER: simdjson threw(RuntimeException Problem while parsing a number) but json did not throw
i_number_pos_double_huge_exp.json: i_number_pos_double_huge_exp.json:
=> STRICTER: simdjson threw(RuntimeException Problem while parsing a number) but json did not throw
i_number_real_neg_overflow.json: i_number_real_neg_overflow.json:
=> STRICTER: simdjson threw(RuntimeException Problem while parsing a number) but json did not throw
i_number_real_pos_overflow.json: i_number_real_pos_overflow.json:
=> STRICTER: simdjson threw(RuntimeException Problem while parsing a number) but json did not throw
i_number_real_underflow.json: i_number_real_underflow.json:
=> same result
i_number_too_big_neg_int.json: i_number_too_big_neg_int.json:
=> STRICTER: simdjson threw(RuntimeException Problem while parsing a number) but json did not throw
i_number_too_big_pos_int.json: i_number_too_big_pos_int.json:
=> STRICTER: simdjson threw(RuntimeException Problem while parsing a number) but json did not throw
i_number_very_big_negative_int.json: i_number_very_big_negative_int.json:
=> STRICTER: simdjson threw(RuntimeException Problem while parsing a number) but json did not throw
i_object_key_lone_2nd_surrogate.json: i_object_key_lone_2nd_surrogate.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
i_string_1st_surrogate_but_2nd_missing.json: i_string_1st_surrogate_but_2nd_missing.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
i_string_1st_valid_surrogate_2nd_invalid.json: i_string_1st_valid_surrogate_2nd_invalid.json:
=> ERROR: json threw(JsonException Single unpaired UTF-16 surrogate in unicode escape) but simdjson did not throw
i_string_UTF-16LE_with_BOM.json: i_string_UTF-16LE_with_BOM.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: Within strings, some characters must be escaped, we found unescaped characters)
i_string_UTF-8_invalid_sequence.json: i_string_UTF-8_invalid_sequence.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_UTF8_surrogate_U+D800.json: i_string_UTF8_surrogate_U+D800.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_incomplete_surrogate_and_escape_valid.json: i_string_incomplete_surrogate_and_escape_valid.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
i_string_incomplete_surrogate_pair.json: i_string_incomplete_surrogate_pair.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
i_string_incomplete_surrogates_escape_valid.json: i_string_incomplete_surrogates_escape_valid.json:
=> ERROR: json threw(JsonException Single unpaired UTF-16 surrogate in unicode escape) but simdjson did not throw
i_string_invalid_lonely_surrogate.json: i_string_invalid_lonely_surrogate.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
i_string_invalid_surrogate.json: i_string_invalid_surrogate.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
i_string_invalid_utf-8.json: i_string_invalid_utf-8.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_inverted_surrogates_U+1D11E.json: i_string_inverted_surrogates_U+1D11E.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
i_string_iso_latin_1.json: i_string_iso_latin_1.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_lone_second_surrogate.json: i_string_lone_second_surrogate.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
i_string_lone_utf8_continuation_byte.json: i_string_lone_utf8_continuation_byte.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_not_in_unicode_range.json: i_string_not_in_unicode_range.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_overlong_sequence_2_bytes.json: i_string_overlong_sequence_2_bytes.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_overlong_sequence_6_bytes.json: i_string_overlong_sequence_6_bytes.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_overlong_sequence_6_bytes_null.json: i_string_overlong_sequence_6_bytes_null.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_truncated-utf-8.json: i_string_truncated-utf-8.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
i_string_utf16BE_no_BOM.json: i_string_utf16BE_no_BOM.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: Within strings, some characters must be escaped, we found unescaped characters)
i_string_utf16LE_no_BOM.json: i_string_utf16LE_no_BOM.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: Within strings, some characters must be escaped, we found unescaped characters)
i_structure_500_nested_arrays.json: i_structure_500_nested_arrays.json:
=> same result
i_structure_UTF-8_BOM_empty_object.json: i_structure_UTF-8_BOM_empty_object.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_1_true_without_comma.json: n_array_1_true_without_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_a_invalid_utf8.json: n_array_a_invalid_utf8.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_array_colon_instead_of_comma.json: n_array_colon_instead_of_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_comma_after_close.json: n_array_comma_after_close.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_comma_and_number.json: n_array_comma_and_number.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_double_comma.json: n_array_double_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_double_extra_comma.json: n_array_double_extra_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_extra_close.json: n_array_extra_close.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_extra_comma.json: n_array_extra_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_incomplete.json: n_array_incomplete.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_incomplete_invalid_value.json: n_array_incomplete_invalid_value.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_inner_array_no_comma.json: n_array_inner_array_no_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_invalid_utf8.json: n_array_invalid_utf8.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_array_items_separated_by_semicolon.json: n_array_items_separated_by_semicolon.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_just_comma.json: n_array_just_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_just_minus.json: n_array_just_minus.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_array_missing_value.json: n_array_missing_value.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_newlines_unclosed.json: n_array_newlines_unclosed.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_number_and_comma.json: n_array_number_and_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_number_and_several_commas.json: n_array_number_and_several_commas.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_spaces_vertical_tab_formfeed.json: n_array_spaces_vertical_tab_formfeed.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: Within strings, some characters must be escaped, we found unescaped characters)
n_array_star_inside.json: n_array_star_inside.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_unclosed.json: n_array_unclosed.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_unclosed_trailing_comma.json: n_array_unclosed_trailing_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_unclosed_with_new_lines.json: n_array_unclosed_with_new_lines.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_array_unclosed_with_object_inside.json: n_array_unclosed_with_object_inside.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_incomplete_false.json: n_incomplete_false.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing an atom starting with the letter 'f')
n_incomplete_null.json: n_incomplete_null.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing an atom starting with the letter 'n')
n_incomplete_true.json: n_incomplete_true.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing an atom starting with the letter 't')
n_multidigit_number_then_00.json: n_multidigit_number_then_00.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_++.json: n_number_++.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_+1.json: n_number_+1.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_+Inf.json: n_number_+Inf.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_-01.json: n_number_-01.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_-1.0..json: n_number_-1.0..json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_-2..json: n_number_-2..json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_-NaN.json: n_number_-NaN.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_.-1.json: n_number_.-1.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_.2e-3.json: n_number_.2e-3.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_0.1.2.json: n_number_0.1.2.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_0.3e+.json: n_number_0.3e+.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_0.3e.json: n_number_0.3e.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_0.e1.json: n_number_0.e1.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_0_capital_E+.json: n_number_0_capital_E+.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_0_capital_E.json: n_number_0_capital_E.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_0e+.json: n_number_0e+.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_0e.json: n_number_0e.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_1.0e+.json: n_number_1.0e+.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_1.0e-.json: n_number_1.0e-.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_1.0e.json: n_number_1.0e.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_1_000.json: n_number_1_000.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_1eE2.json: n_number_1eE2.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_2.e+3.json: n_number_2.e+3.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_2.e-3.json: n_number_2.e-3.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_2.e3.json: n_number_2.e3.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_9.e+.json: n_number_9.e+.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_Inf.json: n_number_Inf.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_NaN.json: n_number_NaN.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_U+FF11_fullwidth_digit_one.json: n_number_U+FF11_fullwidth_digit_one.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_expression.json: n_number_expression.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_hex_1_digit.json: n_number_hex_1_digit.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_hex_2_digits.json: n_number_hex_2_digits.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_infinity.json: n_number_infinity.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_invalid+-.json: n_number_invalid+-.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_invalid-negative-real.json: n_number_invalid-negative-real.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_invalid-utf-8-in-bigger-int.json: n_number_invalid-utf-8-in-bigger-int.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_number_invalid-utf-8-in-exponent.json: n_number_invalid-utf-8-in-exponent.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_number_invalid-utf-8-in-int.json: n_number_invalid-utf-8-in-int.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_number_minus_infinity.json: n_number_minus_infinity.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_minus_sign_with_trailing_garbage.json: n_number_minus_sign_with_trailing_garbage.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_minus_space_1.json: n_number_minus_space_1.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_neg_int_starting_with_zero.json: n_number_neg_int_starting_with_zero.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_neg_real_without_int_part.json: n_number_neg_real_without_int_part.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_neg_with_garbage_at_end.json: n_number_neg_with_garbage_at_end.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_real_garbage_after_e.json: n_number_real_garbage_after_e.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_real_with_invalid_utf8_after_e.json: n_number_real_with_invalid_utf8_after_e.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_number_real_without_fractional_part.json: n_number_real_without_fractional_part.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_starting_with_dot.json: n_number_starting_with_dot.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_number_with_alpha.json: n_number_with_alpha.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_with_alpha_char.json: n_number_with_alpha_char.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_number_with_leading_zero.json: n_number_with_leading_zero.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_object_bad_value.json: n_object_bad_value.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing an atom starting with the letter 't')
n_object_bracket_key.json: n_object_bracket_key.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_comma_instead_of_colon.json: n_object_comma_instead_of_colon.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_double_colon.json: n_object_double_colon.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_emoji.json: n_object_emoji.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_garbage_at_end.json: n_object_garbage_at_end.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_key_with_single_quotes.json: n_object_key_with_single_quotes.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_lone_continuation_byte_in_key_and_trailing_comma.json: n_object_lone_continuation_byte_in_key_and_trailing_comma.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_object_missing_colon.json: n_object_missing_colon.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_missing_key.json: n_object_missing_key.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_missing_semicolon.json: n_object_missing_semicolon.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_missing_value.json: n_object_missing_value.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_no-colon.json: n_object_no-colon.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_non_string_key.json: n_object_non_string_key.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_non_string_key_but_huge_number_instead.json: n_object_non_string_key_but_huge_number_instead.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_repeated_null_null.json: n_object_repeated_null_null.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_several_trailing_commas.json: n_object_several_trailing_commas.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_single_quote.json: n_object_single_quote.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_trailing_comma.json: n_object_trailing_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_trailing_comment.json: n_object_trailing_comment.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_trailing_comment_open.json: n_object_trailing_comment_open.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_trailing_comment_slash_open.json: n_object_trailing_comment_slash_open.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_trailing_comment_slash_open_incomplete.json: n_object_trailing_comment_slash_open_incomplete.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_two_commas_in_a_row.json: n_object_two_commas_in_a_row.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_unquoted_key.json: n_object_unquoted_key.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_unterminated-value.json: n_object_unterminated-value.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: A string is opened, but never closed.)
n_object_with_single_string.json: n_object_with_single_string.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_object_with_trailing_garbage.json: n_object_with_trailing_garbage.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_single_space.json: n_single_space.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Empty: no JSON found)
n_string_1_surrogate_then_escape.json: n_string_1_surrogate_then_escape.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: A string is opened, but never closed.)
n_string_1_surrogate_then_escape_u.json: n_string_1_surrogate_then_escape_u.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_1_surrogate_then_escape_u1.json: n_string_1_surrogate_then_escape_u1.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_1_surrogate_then_escape_u1x.json: n_string_1_surrogate_then_escape_u1x.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_accentuated_char_no_quotes.json: n_string_accentuated_char_no_quotes.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_string_backslash_00.json: n_string_backslash_00.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Within strings, some characters must be escaped, we found unescaped characters)
n_string_escape_x.json: n_string_escape_x.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_escaped_backslash_bad.json: n_string_escaped_backslash_bad.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: A string is opened, but never closed.)
n_string_escaped_ctrl_char_tab.json: n_string_escaped_ctrl_char_tab.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Within strings, some characters must be escaped, we found unescaped characters)
n_string_escaped_emoji.json: n_string_escaped_emoji.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_incomplete_escape.json: n_string_incomplete_escape.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: A string is opened, but never closed.)
n_string_incomplete_escaped_character.json: n_string_incomplete_escaped_character.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_incomplete_surrogate.json: n_string_incomplete_surrogate.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_incomplete_surrogate_escape_invalid.json: n_string_incomplete_surrogate_escape_invalid.json:
=> same(both throw): JSON(JsonException: Single unpaired UTF-16 surrogate in unicode escape) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_invalid-utf-8-in-escape.json: n_string_invalid-utf-8-in-escape.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_string_invalid_backslash_esc.json: n_string_invalid_backslash_esc.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_invalid_unicode_escape.json: n_string_invalid_unicode_escape.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_invalid_utf8_after_escape.json: n_string_invalid_utf8_after_escape.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_string_leading_uescaped_thinspace.json: n_string_leading_uescaped_thinspace.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_string_no_quotes_with_bad_escape.json: n_string_no_quotes_with_bad_escape.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_string_single_doublequote.json: n_string_single_doublequote.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: A string is opened, but never closed.)
n_string_single_quote.json: n_string_single_quote.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_string_single_string_no_double_quotes.json: n_string_single_string_no_double_quotes.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_string_start_escape_unclosed.json: n_string_start_escape_unclosed.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: A string is opened, but never closed.)
n_string_unescaped_crtl_char.json: n_string_unescaped_crtl_char.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: Within strings, some characters must be escaped, we found unescaped characters)
n_string_unescaped_newline.json: n_string_unescaped_newline.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: Within strings, some characters must be escaped, we found unescaped characters)
n_string_unescaped_tab.json: n_string_unescaped_tab.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: Within strings, some characters must be escaped, we found unescaped characters)
n_string_unicode_CapitalU.json: n_string_unicode_CapitalU.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a string)
n_string_with_trailing_garbage.json: n_string_with_trailing_garbage.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_100000_opening_arrays.json: n_structure_100000_opening_arrays.json:
=> same(both throw): JSON(JsonException: Maximum stack depth exceeded) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_U+2060_word_joined.json: n_structure_U+2060_word_joined.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_UTF8_BOM_no_data.json: n_structure_UTF8_BOM_no_data.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_angle_bracket_..json: n_structure_angle_bracket_..json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_angle_bracket_null.json: n_structure_angle_bracket_null.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_array_trailing_garbage.json: n_structure_array_trailing_garbage.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_array_with_extra_array_close.json: n_structure_array_with_extra_array_close.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_array_with_unclosed_string.json: n_structure_array_with_unclosed_string.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: A string is opened, but never closed.)
n_structure_ascii-unicode-identifier.json: n_structure_ascii-unicode-identifier.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_capitalized_True.json: n_structure_capitalized_True.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_close_unopened_array.json: n_structure_close_unopened_array.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_comma_instead_of_closing_brace.json: n_structure_comma_instead_of_closing_brace.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_double_array.json: n_structure_double_array.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_end_array.json: n_structure_end_array.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_incomplete_UTF8_BOM.json: n_structure_incomplete_UTF8_BOM.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_structure_lone-invalid-utf-8.json: n_structure_lone-invalid-utf-8.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_structure_lone-open-bracket.json: n_structure_lone-open-bracket.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_no_data.json: n_structure_no_data.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Empty: no JSON found)
n_structure_null-byte-outside-string.json: n_structure_null-byte-outside-string.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_number_with_trailing_garbage.json: n_structure_number_with_trailing_garbage.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: Problem while parsing a number)
n_structure_object_followed_by_closing_object.json: n_structure_object_followed_by_closing_object.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_object_unclosed_no_value.json: n_structure_object_unclosed_no_value.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_object_with_comment.json: n_structure_object_with_comment.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_object_with_trailing_garbage.json: n_structure_object_with_trailing_garbage.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_array_apostrophe.json: n_structure_open_array_apostrophe.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_array_comma.json: n_structure_open_array_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_array_object.json: n_structure_open_array_object.json:
=> same(both throw): JSON(JsonException: Maximum stack depth exceeded) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_array_open_object.json: n_structure_open_array_open_object.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_array_open_string.json: n_structure_open_array_open_string.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: A string is opened, but never closed.)
n_structure_open_array_string.json: n_structure_open_array_string.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_object.json: n_structure_open_object.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_object_close_array.json: n_structure_open_object_close_array.json:
=> same(both throw): JSON(JsonException: State mismatch (invalid or malformed JSON)) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_object_comma.json: n_structure_open_object_comma.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_object_open_array.json: n_structure_open_object_open_array.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_object_open_string.json: n_structure_open_object_open_string.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: A string is opened, but never closed.)
n_structure_open_object_string_with_apostrophes.json: n_structure_open_object_string_with_apostrophes.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_open_open.json: n_structure_open_open.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_single_eacute.json: n_structure_single_eacute.json:
=> same(both throw): JSON(JsonException: Malformed UTF-8 characters, possibly incorrectly encoded) SIMDJSON(RuntimeException: The input is not valid UTF-8)
n_structure_single_star.json: n_structure_single_star.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_trailing_#.json: n_structure_trailing_#.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_uescaped_LF_before_string.json: n_structure_uescaped_LF_before_string.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_unclosed_array.json: n_structure_unclosed_array.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_unclosed_array_partial_null.json: n_structure_unclosed_array_partial_null.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_unclosed_array_unfinished_false.json: n_structure_unclosed_array_unfinished_false.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_unclosed_array_unfinished_true.json: n_structure_unclosed_array_unfinished_true.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_unclosed_object.json: n_structure_unclosed_object.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_unicode-identifier.json: n_structure_unicode-identifier.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_whitespace_U+2060_word_joiner.json: n_structure_whitespace_U+2060_word_joiner.json:
=> same(both throw): JSON(JsonException: Syntax error) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
n_structure_whitespace_formfeed.json: n_structure_whitespace_formfeed.json:
=> same(both throw): JSON(JsonException: Control character error, possibly incorrectly encoded) SIMDJSON(RuntimeException: The JSON document has an improper structure: missing or superfluous commas, braces, missing keys, etc.)
y_array_arraysWithSpaces.json: y_array_arraysWithSpaces.json:
=> same result
y_array_empty-string.json: y_array_empty-string.json:
=> same result
y_array_empty.json: y_array_empty.json:
=> same result
y_array_ending_with_newline.json: y_array_ending_with_newline.json:
=> same result
y_array_false.json: y_array_false.json:
=> same result
y_array_heterogeneous.json: y_array_heterogeneous.json:
=> same result
y_array_null.json: y_array_null.json:
=> same result
y_array_with_1_and_newline.json: y_array_with_1_and_newline.json:
=> same result
y_array_with_leading_space.json: y_array_with_leading_space.json:
=> same result
y_array_with_several_null.json: y_array_with_several_null.json:
=> same result
y_array_with_trailing_space.json: y_array_with_trailing_space.json:
=> same result
y_number.json: y_number.json:
=> same result
y_number_0e+1.json: y_number_0e+1.json:
=> same result
y_number_0e1.json: y_number_0e1.json:
=> same result
y_number_after_space.json: y_number_after_space.json:
=> same result
y_number_double_close_to_zero.json: y_number_double_close_to_zero.json:
=> same result
y_number_int_with_exp.json: y_number_int_with_exp.json:
=> same result
y_number_minus_zero.json: y_number_minus_zero.json:
=> same result
y_number_negative_int.json: y_number_negative_int.json:
=> same result
y_number_negative_one.json: y_number_negative_one.json:
=> same result
y_number_negative_zero.json: y_number_negative_zero.json:
=> same result
y_number_real_capital_e.json: y_number_real_capital_e.json:
=> same result
y_number_real_capital_e_neg_exp.json: y_number_real_capital_e_neg_exp.json:
=> same result
y_number_real_capital_e_pos_exp.json: y_number_real_capital_e_pos_exp.json:
=> same result
y_number_real_exponent.json: y_number_real_exponent.json:
=> same result
y_number_real_fraction_exponent.json: y_number_real_fraction_exponent.json:
=> same result
y_number_real_neg_exp.json: y_number_real_neg_exp.json:
=> same result
y_number_real_pos_exponent.json: y_number_real_pos_exponent.json:
=> same result
y_number_simple_int.json: y_number_simple_int.json:
=> same result
y_number_simple_real.json: y_number_simple_real.json:
=> same result
y_object.json: y_object.json:
=> same result
y_object_basic.json: y_object_basic.json:
=> same result
y_object_duplicated_key.json: y_object_duplicated_key.json:
=> same result
y_object_duplicated_key_and_value.json: y_object_duplicated_key_and_value.json:
=> same result
y_object_empty.json: y_object_empty.json:
=> same result
y_object_empty_key.json: y_object_empty_key.json:
=> same result
y_object_escaped_null_in_key.json: y_object_escaped_null_in_key.json:
=> same result
y_object_extreme_numbers.json: y_object_extreme_numbers.json:
=> same result
y_object_long_strings.json: y_object_long_strings.json:
=> same result
y_object_simple.json: y_object_simple.json:
=> same result
y_object_string_unicode.json: y_object_string_unicode.json:
=> same result
y_object_with_newlines.json: y_object_with_newlines.json:
=> same result
y_string_1_2_3_bytes_UTF-8_sequences.json: y_string_1_2_3_bytes_UTF-8_sequences.json:
=> same result
y_string_accepted_surrogate_pair.json: y_string_accepted_surrogate_pair.json:
=> same result
y_string_accepted_surrogate_pairs.json: y_string_accepted_surrogate_pairs.json:
=> same result
y_string_allowed_escapes.json: y_string_allowed_escapes.json:
=> same result
y_string_backslash_and_u_escaped_zero.json: y_string_backslash_and_u_escaped_zero.json:
=> same result
y_string_backslash_doublequotes.json: y_string_backslash_doublequotes.json:
=> same result
y_string_comments.json: y_string_comments.json:
=> same result
y_string_double_escape_a.json: y_string_double_escape_a.json:
=> same result
y_string_double_escape_n.json: y_string_double_escape_n.json:
=> same result
y_string_escaped_control_character.json: y_string_escaped_control_character.json:
=> same result
y_string_escaped_noncharacter.json: y_string_escaped_noncharacter.json:
=> same result
y_string_in_array.json: y_string_in_array.json:
=> same result
y_string_in_array_with_leading_space.json: y_string_in_array_with_leading_space.json:
=> same result
y_string_last_surrogates_1_and_2.json: y_string_last_surrogates_1_and_2.json:
=> same result
y_string_nbsp_uescaped.json: y_string_nbsp_uescaped.json:
=> same result
y_string_nonCharacterInUTF-8_U+10FFFF.json: y_string_nonCharacterInUTF-8_U+10FFFF.json:
=> same result
y_string_nonCharacterInUTF-8_U+FFFF.json: y_string_nonCharacterInUTF-8_U+FFFF.json:
=> same result
y_string_null_escape.json: y_string_null_escape.json:
=> same result
y_string_one-byte-utf-8.json: y_string_one-byte-utf-8.json:
=> same result
y_string_pi.json: y_string_pi.json:
=> same result
y_string_reservedCharacterInUTF-8_U+1BFFF.json: y_string_reservedCharacterInUTF-8_U+1BFFF.json:
=> same result
y_string_simple_ascii.json: y_string_simple_ascii.json:
=> same result
y_string_space.json: y_string_space.json:
=> same result
y_string_surrogates_U+1D11E_MUSICAL_SYMBOL_G_CLEF.json: y_string_surrogates_U+1D11E_MUSICAL_SYMBOL_G_CLEF.json:
=> same result
y_string_three-byte-utf-8.json: y_string_three-byte-utf-8.json:
=> same result
y_string_two-byte-utf-8.json: y_string_two-byte-utf-8.json:
=> same result
y_string_u+2028_line_sep.json: y_string_u+2028_line_sep.json:
=> same result
y_string_u+2029_par_sep.json: y_string_u+2029_par_sep.json:
=> same result
y_string_uEscape.json: y_string_uEscape.json:
=> same result
y_string_uescaped_newline.json: y_string_uescaped_newline.json:
=> same result
y_string_unescaped_char_delete.json: y_string_unescaped_char_delete.json:
=> same result
y_string_unicode.json: y_string_unicode.json:
=> same result
y_string_unicodeEscapedBackslash.json: y_string_unicodeEscapedBackslash.json:
=> same result
y_string_unicode_2.json: y_string_unicode_2.json:
=> same result
y_string_unicode_U+10FFFE_nonchar.json: y_string_unicode_U+10FFFE_nonchar.json:
=> same result
y_string_unicode_U+1FFFE_nonchar.json: y_string_unicode_U+1FFFE_nonchar.json:
=> same result
y_string_unicode_U+200B_ZERO_WIDTH_SPACE.json: y_string_unicode_U+200B_ZERO_WIDTH_SPACE.json:
=> same result
y_string_unicode_U+2064_invisible_plus.json: y_string_unicode_U+2064_invisible_plus.json:
=> same result
y_string_unicode_U+FDD0_nonchar.json: y_string_unicode_U+FDD0_nonchar.json:
=> same result
y_string_unicode_U+FFFE_nonchar.json: y_string_unicode_U+FFFE_nonchar.json:
=> same result
y_string_unicode_escaped_double_quote.json: y_string_unicode_escaped_double_quote.json:
=> same result
y_string_utf8.json: y_string_utf8.json:
=> same result
y_string_with_del_character.json: y_string_with_del_character.json:
=> same result
y_structure_lonely_false.json: y_structure_lonely_false.json:
=> same result
y_structure_lonely_int.json: y_structure_lonely_int.json:
=> same result
y_structure_lonely_negative_real.json: y_structure_lonely_negative_real.json:
=> same result
y_structure_lonely_null.json: y_structure_lonely_null.json:
=> same result
y_structure_lonely_string.json: y_structure_lonely_string.json:
=> same result
y_structure_lonely_true.json: y_structure_lonely_true.json:
=> same result
y_structure_string_empty.json: y_structure_string_empty.json:
=> same result
y_structure_trailing_newline.json: y_structure_trailing_newline.json:
=> same result
y_structure_true_in_array.json: y_structure_true_in_array.json:
=> same result
y_structure_whitespace_array.json: y_structure_whitespace_array.json:
=> same result
Done comparing json to see if simdjson_decode() returns any values json_decode() would return or throw for
*/
