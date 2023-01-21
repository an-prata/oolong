/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <stdio.h>
#include "error.h"

static oolong_error_t recorded_errors = OOLONG_ERROR_NONE;

oolong_error_t oolong_error_debug_record(oolong_error_t error, const char* file, const char* function, size_t line)
{
    if (error == OOLONG_ERROR_NONE)
        return error;

    recorded_errors |= error;

#ifdef OOLONG_EXIT_ON_ERROR
    fprintf(stderr, "%s: line %zu of %s in %s\n", __func__, line, function, file);
    exit(-1);
#endif // OOLONG_EXIT_ON_ERROR
    
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

