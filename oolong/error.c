/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <stdio.h>
#include "error.h"

static oolong_error_t recorded_errors = OOLONG_ERROR_NONE;
static bool exit_on_error = true;

void oolong_error_set_exit_on_error(bool exit_on_error_value)
{
    exit_on_error = exit_on_error_value;
}

oolong_error_t oolong_error_debug_record(oolong_error_t error, const char* file, const char* function, size_t line)
{
    if (error == OOLONG_ERROR_NONE)
        return error;

    if (exit_on_error)
    {
        fprintf(stderr, "%s: line %zu of %s in %s\n", __func__, line, function, file);
        exit(EXIT_FAILURE);
    }

    recorded_errors |= error;
    return error;
}

void oolong_error_clear_all(void)
{
    recorded_errors = OOLONG_ERROR_NONE;
}

void oolong_error_clear(oolong_error_t error)
{
    recorded_errors &= ~error;
}

oolong_error_t oolong_error_get_all(void)
{
    return recorded_errors;
}

oolong_error_t oolong_error_check(oolong_error_t error)
{
    return recorded_errors & error;
}

