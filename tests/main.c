/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <stdio.h>
#include "include/scrutiny.h"
#include "error_tests.h"
#include "style_set_tests.h"
#include "keyboard_tests.h"

int main()
{
    scrutiny_unit_test_t unit_tests[] =
    {
        error_test,
        style_set_add_test,
        buffered_keys_test,
        NULL
    };

    scrutiny_run_tests(unit_tests);
    scrutiny_output_test_results(stdout);

    scrutiny_test_results_t results = scrutiny_get_test_results();

    if (results.failed_cases > 0)
        exit(EXIT_FAILURE);
    else
        exit(EXIT_SUCCESS);
}

