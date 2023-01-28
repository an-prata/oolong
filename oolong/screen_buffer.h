/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_SCREEN_BUFFER_H
#define OOLONG_SCREEN_BUFFER_H

/* This constant is required for the full set of wide character functions. */
#define __USE_XOPEN

#include <locale.h>
#include "error.h"

/* For convinience, any locale can be used but this is whats tested. */
#define oolong_set_locale() setlocale(LC_CTYPE, "C.UTF-8")

typedef struct oolong_screen_buffer_dimensions_s oolong_screen_buffer_dimensions_t;

struct oolong_screen_buffer_dimensions_s
{
    size_t columns;
    size_t rows;
};

/*
 * The the dimensions of the current terminal screen.
 */
oolong_screen_buffer_dimensions_t oolong_get_screen_dimensions(void);

#endif // OOLONG_SCREEN_BUFFER_H

