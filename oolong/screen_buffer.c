/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "escapes.h"
#include "screen_buffer.h"

typedef struct winsize window_size_t;

struct oolong_screen_buffer_s
{
    size_t columns;
    size_t rows;
    wchar_t* contents;
};

oolong_screen_buffer_t* screen_buffer_create(void)
{
    oolong_screen_buffer_t* screen_buffer = malloc(sizeof(oolong_screen_buffer_t));
    window_size_t window_size;

    if (screen_buffer == NULL)
    {
        oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
        return NULL;
    }

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size) == -1)
    {
        oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);
        return NULL;
    }

    screen_buffer->columns = window_size.ws_col;
    screen_buffer->rows = window_size.ws_row;
    screen_buffer->contents = calloc(screen_buffer->columns * screen_buffer->rows, sizeof(wchar_t));

    if (screen_buffer->contents == NULL)
    {
        oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
        return NULL;
    }
    
    return screen_buffer;
}

oolong_error_t screen_buffer_update_dimensions(oolong_screen_buffer_t* screen_buffer)
{
    if (screen_buffer == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
    
    window_size_t window_size;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size) == -1)
        return oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);

    screen_buffer->columns = window_size.ws_col;
    screen_buffer->rows = window_size.ws_row;

    if (screen_buffer->contents == NULL)
        return OOLONG_ERROR_NONE;

    wchar_t* new_contents = reallocarray(screen_buffer->contents, screen_buffer->columns * screen_buffer->rows, sizeof(wchar_t));

    if (new_contents == NULL)
        return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);

    screen_buffer->contents = new_contents;
    return OOLONG_ERROR_NONE;
}

oolong_screen_buffer_dimensions_t screen_buffer_get_dimensions(oolong_screen_buffer_t* screen_buffer)
{
    if (screen_buffer == NULL)
    {
        oolong_screen_buffer_dimensions_t dimensions = { .columns = 0, .rows = 0 };
        return dimensions;
    }
    
    oolong_screen_buffer_dimensions_t dimensions = { .columns = screen_buffer->columns, .rows = screen_buffer->rows };
    return dimensions;
}

wchar_t* screen_buffer_get_contents(oolong_screen_buffer_t* screen_buffer)
{
    if (screen_buffer == NULL)
        return NULL;

    return screen_buffer->contents;
}

oolong_error_t screen_buffer_print(oolong_screen_buffer_t* screen_buffer)
{
    if (screen_buffer == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    oolong_terminal_clear();
    
    for (size_t i = 0; i < screen_buffer->rows * screen_buffer->columns; i += wcwidth(screen_buffer->contents[i]))
        printf("%lc", screen_buffer->contents[i]);

    fflush(stdout);
    return OOLONG_ERROR_NONE;
}

