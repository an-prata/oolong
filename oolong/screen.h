/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_SCREEN_H
#define OOLONG_SCREEN_H

/* This constant is required for the full set of wide character functions. */
#define __USE_XOPEN

#include <locale.h>
#include "error.h"

/* For convinience, any locale can be used but this is whats tested. */
#define oolong_set_locale() setlocale(LC_CTYPE, "C.UTF-8")

/*
 * The the dimensions of the current terminal screen. Passing NULL will discard
 * that part of the operations result.
 */
oolong_error_t oolong_get_screen_dimensions(unsigned int* columns, unsigned int* rows);

#endif // OOLONG_SCREEN_H

