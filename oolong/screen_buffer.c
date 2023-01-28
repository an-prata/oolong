/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <unistd.h>
#include <sys/ioctl.h>
#include "screen_buffer.h"

typedef struct winsize window_size_t;

oolong_screen_buffer_dimensions_t oolong_get_screen_dimensions(void)
{
    window_size_t window_size;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size) == -1)
    {
        oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);
        oolong_screen_buffer_dimensions_t dimensions = { .columns = 0, .rows = 0 };
        return dimensions;
    }

    oolong_screen_buffer_dimensions_t dimensions = { .columns = window_size.ws_col, .rows = window_size.ws_row};
    return dimensions;
}

