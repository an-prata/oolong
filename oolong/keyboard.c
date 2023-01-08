/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "keyboard.h"

/* Size of the longest escape sequence oolong will accept. */
#define READ_SIZE 3

#define CONTROL_UP 65
#define CONTROL_DOWN 66
#define CONTROL_RIGHT 67
#define CONTROL_LEFT 68
#define CONTROL_HOME 72
#define CONTROL_END 70

typedef struct termios terminal_attributes_t;

static terminal_attributes_t original_terminal_state;

static key_t interpret_escape_sequence(char* in, size_t in_size)
{
    if (in_size < 3)
        goto error;
    
    if (in[0] != KEY_ESCAPE || in[1] != KEY_OPEN_BRACKET)
        goto error;
    
    switch (in[2])
    {
        case CONTROL_UP:     return KEY_UP;
        case CONTROL_DOWN:   return KEY_DOWN;
        case CONTROL_RIGHT:  return KEY_RIGHT;
        case CONTROL_LEFT:   return KEY_LEFT;
        case CONTROL_HOME:   return KEY_HOME;
        case CONTROL_END:    return KEY_END;
    }

    /* Unsupported escape. */
    return KEY_NULL;

error:
    oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);
    return KEY_ERROR;
}

oolong_error_t oolong_disable_canonical_input(void)
{
    if (tcgetattr(STDIN_FILENO, &original_terminal_state))
        return OOLONG_ERROR_FAILED_IO_READ;
    
    terminal_attributes_t non_canonical = original_terminal_state;
    
    non_canonical.c_iflag &= ~(ICANON | ECHO);  /* Dont wait for newline and dont echo typed characters. */
    non_canonical.c_cc[VMIN] = 0;               /* Set minimum bytes from read to 0. */
    non_canonical.c_cc[VTIME] = 0;              /* Dont wait for input with read. */
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &non_canonical))
        return OOLONG_ERROR_FAILED_IO_WRITE;
    
    return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_restore_canonical_input(void)
{
    oolong_error_t error = OOLONG_ERROR_NONE;
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_state))
        error = OOLONG_ERROR_FAILED_IO_WRITE;

    oolong_error_record(error);
    return error;
}

oolong_key_t oolong_keyboard_get_key(void)
{
    char in[READ_SIZE];
    ssize_t in_size = read(STDIN_FILENO, in, READ_SIZE);

    if (in_size < 0)
        goto error;

    if (in_size == 1)
        return (key_t)in[0];

    if (in_size == 2)
        goto error;
    
    return interpret_escape_sequence(in, in_size);

error:
    oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);
    return KEY_ERROR;
}

