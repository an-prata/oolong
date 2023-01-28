/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_STYLING_H
#define OOLONG_STYLING_H

#include <wchar.h>
#include "error.h"
#include "escapes.h"

#define OOLONG_STYLE_CLEAR_STRING L"\033[0m"

enum oolong_style_e
{
    OOLONG_STYLE_CLEAR              = 0,
    OOLONG_STYLE_ITALIC             = 1,
    OOLONG_STYLE_BOLD               = 2,
    OOLONG_STYLE_UNDERLINE          = 3,
    OOLONG_STYLE_RED                = 4,
    OOLONG_STYLE_GREEN              = 5,
    OOLONG_STYLE_BLUE               = 6,
    OOLONG_STYLE_YELLOW             = 7,
    OOLONG_STYLE_PURPLE             = 8,
    OOLONG_STYLE_CYAN               = 9,
    OOLONG_STYLE_WHITE              = 10,
    OOLONG_STYLE_BLACK              = 11,
    OOLONG_STYLE_BACKGROUND_RED     = 12,
    OOLONG_STYLE_BACKGROUND_GREEN   = 13,
    OOLONG_STYLE_BACKGROUND_BLUE    = 14,
    OOLONG_STYLE_BACKGROUND_YELLOW  = 15,
    OOLONG_STYLE_BACKGROUND_PURPLE  = 16,
    OOLONG_STYLE_BACKGROUND_CYAN    = 17,
    OOLONG_STYLE_BACKGROUND_WHITE   = 18,
    OOLONG_STYLE_BACKGROUND_BLACK   = 19
};

typedef enum oolong_style_e oolong_style_t;
typedef wchar_t oolong_style_set_t;

/*
 * Create a new style set.
 */
oolong_style_set_t* oolong_style_set_create();

/*
 * Add a style to the given style set.
 */
oolong_error_t oolong_style_set_add(oolong_style_set_t** style_set, oolong_style_t style);

/*
 * Free all memory used by the given style set.
 */
oolong_error_t oolong_style_set_destroy(oolong_style_set_t* style_set);

#endif // OOLONG_STYLING_H

