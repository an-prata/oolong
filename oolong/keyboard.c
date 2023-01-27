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

static size_t buffered_keys_index = 0;
static size_t buffered_keys_length = 0;
static oolong_key_t* buffered_keys = NULL;
static terminal_attributes_t original_terminal_state;

static key_t interpret_escape_sequence(char* in, size_t in_size)
{
    if (in_size < 3 || in[0] != KEY_ESCAPE || in[1] != KEY_OPEN_BRACKET)
    {
        oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);
        return KEY_ERROR;
    }
    
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
}

/*
 * To clear a warning for incompatiable pointer for the atexit() function.
 */
static void restore_terminal_atexit(void)
{
    oolong_restore_canonical_input();
}

oolong_error_t oolong_disable_canonical_input(void)
{
    if (tcgetattr(STDIN_FILENO, &original_terminal_state))
        return oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);
    
    terminal_attributes_t non_canonical = original_terminal_state;
    
    non_canonical.c_lflag &= ~(ICANON | ECHO);  /* Dont wait for newline and dont echo typed characters. */
    non_canonical.c_cc[VMIN] = 1;               /* Set minimum bytes from read to 1. */
    non_canonical.c_cc[VTIME] = 0;              /* Dont wait for input with read. */
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &non_canonical))
        return oolong_error_record(OOLONG_ERROR_FAILED_IO_WRITE);
    
    atexit(restore_terminal_atexit);
    return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_restore_canonical_input(void)
{
    oolong_error_t error = OOLONG_ERROR_NONE;
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_state))
        return oolong_error_record(error);

    return OOLONG_ERROR_NONE;
}

oolong_key_t oolong_keyboard_get_key(void)
{
    if (buffered_keys_index < buffered_keys_length)
    {
        buffered_keys_index++;
        return buffered_keys[buffered_keys_index - 1];
    }
    
    char in[READ_SIZE];
    ssize_t in_size = read(STDIN_FILENO, in, READ_SIZE);

    if (in_size < 0)
    {
        oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);
        return KEY_ERROR;
    }

    if (in_size == 1)
        return (key_t)in[0];

    if (in_size > 1 && in[0] == KEY_ESCAPE)
        return interpret_escape_sequence(in, in_size);

    /*
     * Getting here means that more that 1 byte was read and the first byte was
     * not an escape key - likely a copy and paste or multibyte character.
     *
     * In this case we can only relinquish handling input to whatever needs it
     * since we cant know where one input begins and the next ends, putting us
     * in an infinite loop.
     */
    
    return KEY_STRING;
}

oolong_error_t oolong_keyboard_buffer_keys(oolong_key_t* keys, size_t keys_length)
{
    if (keys == NULL || keys_length < 1)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    buffered_keys_length = keys_length;
    buffered_keys = keys;
    return OOLONG_ERROR_NONE;
}

