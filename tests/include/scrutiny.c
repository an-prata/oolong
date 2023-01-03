/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "scrutiny.h"

#define SCRUTINY_TEXT_NORMAL "\033[0m"
#define SCRUTINY_TEXT_BOLD "\033[1m"
#define SCRUTINY_TEXT_ITALIC "\033[3m"
#define SCRUTINY_TEXT_GREEN "\033[0;32m"
#define SCRUTINY_TEXT_BLUE "\033[0;44m"
#define SCRUTINY_TEXT_RED "\033[0;31m"

static size_t failed_cases = 0;
static size_t passed_cases = 0;
static size_t failed_tests_length = 0;
static size_t passed_tests_length = 0;
static const char** failed_tests;
static const char** passed_tests;
static size_t test_files_length = 0;
static const char** test_files;

static bool compare_null_terminated_strings(const char* str0, const char* str1)
{
    for (size_t i = 0;; i++)
    {
        if (str0[i] == '\0' && str1[i] == '\0')
            return true;
        if (str0[i] == '\0' && str1[i] == '\0')
            return false;
        if (str0[i] != str1[i])
            return false;
    }
}

static void succeeded_test_expand_and_add(const char* succeeded_test)
{
    passed_cases++;
    
    for (size_t i = 0; i < passed_tests_length; i++)
        if (compare_null_terminated_strings(passed_tests[i], succeeded_test))
            return;

    /* If the test function has failed previously dont add it to the succeeded list. */
    for (size_t i = 0; i < failed_tests_length; i++)
        if (compare_null_terminated_strings(failed_tests[i], succeeded_test))
            return;
    
    passed_tests_length++;
    passed_tests = realloc(passed_tests, passed_tests_length * sizeof(char*));
    passed_tests[passed_tests_length - 1] = succeeded_test;
}

static void succeeded_test_contract_and_remove(const char* test)
{
    for (size_t i = 0; i < passed_tests_length; i++)
    {
        if (compare_null_terminated_strings(passed_tests[i], test))
        {
            if (i != passed_tests_length - 1)
                passed_tests[i] = passed_tests[passed_tests_length - 1];

            passed_tests_length--;
            return;
        }
    }
}

static void failed_test_expand_and_add(const char* failed_test)
{
    failed_cases++;
    
    for (size_t i = 0; i < failed_tests_length; i++)
        if (compare_null_terminated_strings(failed_tests[i], failed_test))
            return;

    failed_tests_length++;
    failed_tests = realloc(failed_tests, failed_tests_length * sizeof(char*));
    failed_tests[failed_tests_length - 1] = failed_test;
}

static void test_file_expand_and_add(const char* test_file)
{
    for (size_t i = 0; i < test_files_length; i++)
        if (compare_null_terminated_strings(test_files[i], test_file))
            return;

    test_files_length++;
    test_files = realloc(test_files, test_files_length * sizeof(char*));
    test_files[test_files_length - 1] = test_file;
}

static void failed_test_print_failure(const char* expected, const char* actual, const char* file, const char* function, size_t line, const char* assert)
{
    printf(SCRUTINY_TEXT_RED SCRUTINY_TEXT_BOLD "\nTEST FAILED" SCRUTINY_TEXT_NORMAL " (%s): " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " failed " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " on line %zu\n\n", file, function, assert, line);
    printf("\tExpected:  %s\n", expected);
    printf("\tActual:    %s\n", actual);
}

static void failed_test_print_failure_unsigned_integer(uint64_t expected, uint64_t actual, const char* file, const char* function, size_t line, const char* assert)
{
    printf(SCRUTINY_TEXT_RED SCRUTINY_TEXT_BOLD "\nTEST FAILED" SCRUTINY_TEXT_NORMAL " (%s): " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " failed " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " on line %zu\n\n", file, function, assert, line);
    printf("\tExpected:  %zu\n", expected);
    printf("\tActual:    %zu\n", actual);
}

static void failed_test_print_failure_signed_integer(int64_t expected, int64_t actual, const char* file, const char* function, size_t line, const char* assert)
{
    printf(SCRUTINY_TEXT_RED SCRUTINY_TEXT_BOLD "\nTEST FAILED" SCRUTINY_TEXT_NORMAL " (%s): " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " failed " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " on line %zu\n\n", file, function, assert, line);
    printf("\tExpected:  %zi\n", expected);
    printf("\tActual:    %zi\n", actual);
}

void scrutiny_run_tests(scrutiny_unit_test_t* scrutiny_unit_tests)
{
    size_t tests_count;
    for (tests_count = 0; scrutiny_unit_tests[tests_count] != NULL; tests_count++);
    printf("Running %zu scrutiny unit tests...\n", tests_count);
    
    for (size_t test = 0; test < tests_count; test++)
        scrutiny_unit_tests[test]();
}

int scrutiny_output_test_results(file_t* out_file)
{
    if (out_file == NULL)
        return 1;

    long double percent_passed = ((long double)passed_tests_length / (long double)(passed_tests_length + failed_tests_length)) * 100.0;
    long double percent_failed = ((long double)failed_tests_length / (long double)(passed_tests_length + failed_tests_length)) * 100.0;
    long double percent_cases_passed = ((long double)passed_cases / (long double)(passed_cases + failed_cases)) * 100.0;
    long double percent_cases_failed = ((long double)failed_cases / (long double)(passed_cases + failed_cases)) * 100.0;

    fprintf(out_file, "Scrutiny ran %zu test cases, from %zu tests, in %zu files.\n\n", failed_cases + passed_cases, failed_tests_length + passed_tests_length, test_files_length);

    if (failed_tests_length > 0)
        fprintf(out_file, "Failed tests:\n");

    for (size_t i = 0; i < failed_tests_length; i++)
        fprintf(out_file, SCRUTINY_TEXT_ITALIC "\t%s\n" SCRUTINY_TEXT_NORMAL, failed_tests[i]);

    if (passed_cases > 0)
    {
        fprintf(out_file, "\n(" SCRUTINY_TEXT_GREEN "" SCRUTINY_TEXT_NORMAL ") %zu of %zu tests passed (%2.1Lf%%).", passed_tests_length, passed_tests_length + failed_tests_length, percent_passed);
        fprintf(out_file, "\t(" SCRUTINY_TEXT_GREEN "" SCRUTINY_TEXT_NORMAL ") %zu of %zu test cases passed (%2.1Lf%%).\n", passed_cases, passed_cases + failed_cases, percent_cases_passed);
    }

    if (failed_cases > 0)
    {
        fprintf(out_file, "(" SCRUTINY_TEXT_RED "x" SCRUTINY_TEXT_NORMAL ") %zu of %zu tests failed (%2.1Lf%%).", failed_tests_length, passed_tests_length + failed_tests_length, percent_failed);
        fprintf(out_file, "\t(" SCRUTINY_TEXT_RED "x" SCRUTINY_TEXT_NORMAL ") %zu of %zu test cases failed (%2.1Lf%%).\n", failed_cases, failed_cases + passed_cases, percent_cases_failed);
    }

    fflush(out_file);

    if (ferror(out_file))
        return -1;
    
    return 0;
}

int scrutiny_output_test_results_parsable(file_t* out_file)
{
    if (out_file == NULL)
        return 1;

    long double percent_passed = ((long double)passed_tests_length / (long double)(passed_tests_length + failed_tests_length)) * 100.0;
    long double percent_failed = ((long double)failed_tests_length / (long double)(passed_tests_length + failed_tests_length)) * 100.0;
    long double percent_cases_passed = ((long double)passed_cases / (long double)(passed_cases + failed_cases)) * 100.0;
    long double percent_cases_failed = ((long double)failed_cases / (long double)(passed_cases + failed_cases)) * 100.0;

    fprintf(out_file, "ran %zu cases, %zu tests, %zu files\n\n", failed_cases + passed_cases, failed_tests_length + passed_tests_length, test_files_length);

    fprintf(out_file, "cases:\n");
    fprintf(out_file, "%zu/%zu passed (%2.1Lf%%)\n", passed_cases, passed_cases + failed_cases, percent_cases_passed);
    fprintf(out_file, "%zu/%zu failed (%2.1Lf%%)\n\n", failed_cases, failed_cases + passed_cases, percent_cases_failed);

    fprintf(out_file, "tests:\n");
    fprintf(out_file, "%zu/%zu passed (%2.1Lf%%)\n", passed_tests_length, passed_tests_length + failed_tests_length, percent_passed);
    fprintf(out_file, "%zu/%zu failed (%2.1Lf%%)\n\n", failed_tests_length, passed_tests_length + failed_tests_length, percent_failed);

    fprintf(out_file, "failed tests:\n");

    for (size_t i = 0; i < failed_tests_length; i++)
        fprintf(out_file, "%s\n", failed_tests[i]);

    fflush(out_file);

    if (ferror(out_file))
        return -1;
    
    return 0;
}

void scrutiny_report_assert_pass(const char* file, const char* function)
{
    test_file_expand_and_add(file);
    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_fail(const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);
    succeeded_test_contract_and_remove(function);
    failed_test_expand_and_add(function);
    printf(SCRUTINY_TEXT_RED SCRUTINY_TEXT_BOLD "\nTEST FAILED" SCRUTINY_TEXT_NORMAL " (%s): " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " failed " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " on line %zu\n\n", file, function, __func__, line);
}

void scrutiny_report_assert_true(bool expression, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (!expression)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        printf(SCRUTINY_TEXT_RED SCRUTINY_TEXT_BOLD "\nTEST FAILED" SCRUTINY_TEXT_NORMAL " (%s): " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " failed " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " on line %zu\n\n", file, function, __func__, line);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_false(bool expression, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expression)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        printf(SCRUTINY_TEXT_RED SCRUTINY_TEXT_BOLD "\nTEST FAILED" SCRUTINY_TEXT_NORMAL " (%s): " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " failed " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " on line %zu\n\n", file, function, __func__, line);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_char(char expected, char actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_unsigned_char(unsigned char expected, unsigned char actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_short(short expected, short actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int(int expected, int actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_long(long expected, long actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_long_long(long long expected, long long actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_unsigned_short(unsigned short expected, unsigned short actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_unsigned_int(unsigned int expected, unsigned int actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_unsigned_long(unsigned long expected, unsigned long actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_unsigned_long_long(unsigned long long expected, unsigned long long actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_float(float expected, float actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_double(double expected, double actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_long_double(long double expected, long double actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int8_t(int8_t expected, int8_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int16_t(int16_t expected, int16_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int32_t(int32_t expected, int32_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int64_t(int64_t expected, int64_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int_fast8_t(int_fast8_t expected, int_fast8_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int_fast16_t(int_fast16_t expected, int_fast16_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int_fast32_t(int_fast32_t expected, int_fast32_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int_fast64_t(int_fast64_t expected, int_fast64_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int_least8_t(int_least8_t expected, int_least8_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int_least16_t(int_least16_t expected, int_least16_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int_least32_t(int_least32_t expected, int_least32_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_int_least64_t(int_least64_t expected, int_least64_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_signed_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint8_t(uint8_t expected, uint8_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint16_t(uint16_t expected, uint16_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint32_t(uint32_t expected, uint32_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint64_t(uint64_t expected, uint64_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint_fast8_t(uint_fast8_t expected, uint_fast8_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint_fast16_t(uint_fast16_t expected, uint_fast16_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint_fast32_t(uint_fast32_t expected, uint_fast32_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint_fast64_t(uint_fast64_t expected, uint_fast64_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint_least8_t(uint_least8_t expected, uint_least8_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint_least16_t(uint_least16_t expected, uint_least16_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint_least32_t(uint_least32_t expected, uint_least32_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_uint_least64_t(uint_least64_t expected, uint_least64_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_intptr_t(intptr_t expected, intptr_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_intmax_t(intmax_t expected, intmax_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_enum(enum_value_t expected, enum_value_t actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_ptr(void* expected, void* actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (expected != actual)
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure_unsigned_integer((uint64_t)expected, (uint64_t)actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_ptr_data(void* expected, void* actual, size_t struct_size, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    uint8_t* expected_bytes = (uint8_t*)expected;
    uint8_t* actual_bytes = (uint8_t*)actual;

    for (size_t i = 0; i < struct_size; i++)
    {
        if (expected_bytes[i] != actual_bytes[i])
        {
            succeeded_test_contract_and_remove(function);
            failed_test_expand_and_add(function);
            printf(SCRUTINY_TEXT_RED SCRUTINY_TEXT_BOLD "\nTEST FAILED" SCRUTINY_TEXT_NORMAL " (%s): " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " failed " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " on line %zu\n\n", file, function, __func__, line);
            printf("\tExpected:  %u at byte %zu\n", expected_bytes[i], i);
            printf("\tActual:    %u\n", actual_bytes[i]);
            return;
        }
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_array(void* expected, void* actual, size_t sizeof_type, size_t array_length, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    uint8_t* expected_bytes = (uint8_t*)expected;
    uint8_t* actual_bytes = (uint8_t*)actual;

    for (size_t byte = 0; byte < sizeof_type * array_length; byte++)
    {
        if (expected_bytes[byte] != actual_bytes[byte])
        {
            succeeded_test_contract_and_remove(function);
            failed_test_expand_and_add(function);
            printf(SCRUTINY_TEXT_RED SCRUTINY_TEXT_BOLD "\nTEST FAILED" SCRUTINY_TEXT_NORMAL " (%s): " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " failed " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " on line %zu\n\n", file, function, __func__, line);
            printf("\tDifference at index: %zu\n", byte / sizeof_type);
            return;
        }
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_string(char* expected, char* actual, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    if (!compare_null_terminated_strings(expected, actual))
    {
        succeeded_test_contract_and_remove(function);
        failed_test_expand_and_add(function);
        failed_test_print_failure(expected, actual, file, function, line, __func__);
        return;
    }

    succeeded_test_expand_and_add(function);
}

void scrutiny_report_assert_equal_non_terminated_string(char* expected, char* actual, size_t size, const char* file, const char* function, size_t line)
{
    test_file_expand_and_add(file);

    for (size_t i = 0; i < size; i++)
    {
        if (expected[i] != actual[i])
        {
            succeeded_test_contract_and_remove(function);
            failed_test_expand_and_add(function);
            printf(SCRUTINY_TEXT_RED SCRUTINY_TEXT_BOLD "\nTEST FAILED" SCRUTINY_TEXT_NORMAL " (%s): " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " failed " SCRUTINY_TEXT_ITALIC "%s" SCRUTINY_TEXT_NORMAL " on line %zu\n\n", file, function, __func__, line);
            printf("\tExpected:  %c at index %zu\n", expected[i], i);
            printf("\tActual:    %c\n", actual[i]);
            return;
        }
    }

    succeeded_test_expand_and_add(function);
}

