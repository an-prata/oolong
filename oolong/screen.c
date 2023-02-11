/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <unistd.h>
#include <sys/ioctl.h>
#include "screen.h"

typedef struct winsize window_size_t;

oolong_error_t oolong_get_screen_dimensions(unsigned int* columns, unsigned int* rows)
{
    window_size_t window_size;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size) == -1)
        return oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);

    if (columns != NULL)
        *columns = window_size.ws_col;

    if (rows != NULL)
        *rows = window_size.ws_row;

    return OOLONG_ERROR_NONE;
}

