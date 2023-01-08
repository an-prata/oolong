/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#define __USE_XOPEN

#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>
#include "error.h"

/* For convinience, any locale can be used but this is whats tested. */
#define oolong_set_locale() setlocale(LC_CTYPE, "C.UTF-8")

typedef struct oolong_screen_buffer_s oolong_screen_buffer_t;
typedef struct oolong_screen_buffer_dimensions_s oolong_screen_buffer_dimensions_t;

struct oolong_screen_buffer_dimensions_s
{
    size_t columns;
    size_t rows;
};

/*
 * Creates and returns a pointer to a new screen buffer.
 */
oolong_screen_buffer_t* screen_buffer_create(void);

/*
 * Updates the dimensions of the given screen buffer. Existing contents will be
 * realligned to the new dimensions so that they still appear correctly.
 */
oolong_error_t screen_buffer_update_dimensions(oolong_screen_buffer_t* screen_buffer);

/*
 * Gets the dimensions of the given screen buffer.
 */
oolong_screen_buffer_dimensions_t screen_buffer_get_dimensions(oolong_screen_buffer_t* screen_buffer);

/*
 * Gets a pointer to the contents of the given screen buffer, this may be used
 * to modify the screen buffer's contents within the bounds of its dimensions.
 */
wchar_t* screen_buffer_get_contents(oolong_screen_buffer_t* screen_buffer);

/*
 * Prints the given screen buffer to the terminal, it is recommended that this
 * be used in an alternate terminal "screen" so that when the program exits it
 * doesnt leave behind the output of each frame it rendered.
 */
oolong_error_t screen_buffer_print(oolong_screen_buffer_t* screen_buffer);

