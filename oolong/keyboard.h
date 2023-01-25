/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <limits.h>
#include "error.h"

enum oolong_key_e
{
    KEY_ERROR = INT_MIN, /* Failed to get key. */
    KEY_NULL          = 0,
    KEY_TAB           = 9,
    KEY_RETURN        = 10,
    KEY_ESCAPE        = 27,
    KEY_SPACE         = 32,
    KEY_BANG          = 33,   /*  !  */
    KEY_QUOTE         = 34,   /*  "  */
    KEY_HASH          = 35,   /*  #  */
    KEY_DOLLAR        = 36,
    KEY_PERCENT       = 37,
    KEY_AMPERSAND     = 38,   /*  &  */
    KEY_SINGLE_QUOTE  = 39,   /*  '  */
    KEY_OPEN_PAREN    = 40,   /*  (  */
    KEY_CLOSE_PAREN   = 41,   /*  )  */
    KEY_ASTERISK      = 42,   /*  *  */
    KEY_PLUS          = 43,   /*  +  */
    KEY_COMMA         = 44,   /*  ,  */
    KEY_MINUS         = 45,   /*  -  */
    KEY_PERIOD        = 46,   /*  .  */
    KEY_SLASH         = 47,
    KEY_0             = 48,
    KEY_1             = 49,
    KEY_2             = 50,
    KEY_3             = 51,
    KEY_4             = 52,
    KEY_5             = 53,
    KEY_6             = 54,
    KEY_7             = 55,
    KEY_8             = 56,
    KEY_9             = 57,
    KEY_COLON         = 58,   /*  :  */
    KEY_SEMICOLON     = 59,   /*  ;  */
    KEY_LESS_THAN     = 60,   /*  <  */
    KEY_EQUALS        = 61,   /*  =  */
    KEY_GREATER_THAN  = 62,   /*  >  */
    KEY_QUESTION_MARK = 63,   /*  ?  */
    KEY_ASPERAND      = 64,   /*  @  */
    KEY_A             = 65,
    KEY_B             = 66,
    KEY_C             = 67,
    KEY_D             = 68,
    KEY_E             = 69,
    KEY_F             = 70,
    KEY_G             = 71,
    KEY_H             = 72,
    KEY_I             = 73,
    KEY_J             = 74,
    KEY_K             = 75,
    KEY_L             = 76,
    KEY_M             = 77,
    KEY_N             = 78,
    KEY_O             = 79,
    KEY_P             = 80,
    KEY_Q             = 81,
    KEY_R             = 82,
    KEY_S             = 83,
    KEY_T             = 84,
    KEY_U             = 85,
    KEY_V             = 86,
    KEY_W             = 87,
    KEY_X             = 88,
    KEY_Y             = 89,
    KEY_Z             = 90,
    KEY_OPEN_BRACKET  = 91,   /*  [  */
    KEY_BACKSLASH     = 92,   /*  \  */
    KEY_CLOSE_BRACKET = 93,   /*  ]  */
    KEY_CARROT        = 94,   /*  ^  */
    KEY_UNDERSCORE    = 95,   /*  _  */
    KEY_BACKTICK      = 96,   /*  `  */
    KEY_LOWERCASE_A   = 97,
    KEY_LOWERCASE_B   = 98,
    KEY_LOWERCASE_C   = 99,
    KEY_LOWERCASE_D   = 100,
    KEY_LOWERCASE_E   = 101,
    KEY_LOWERCASE_F   = 102,
    KEY_LOWERCASE_G   = 103,
    KEY_LOWERCASE_H   = 104,
    KEY_LOWERCASE_I   = 105,
    KEY_LOWERCASE_J   = 106,
    KEY_LOWERCASE_K   = 107,
    KEY_LOWERCASE_L   = 108,
    KEY_LOWERCASE_M   = 109,
    KEY_LOWERCASE_N   = 110,
    KEY_LOWERCASE_O   = 111,
    KEY_LOWERCASE_P   = 112,
    KEY_LOWERCASE_Q   = 113,
    KEY_LOWERCASE_R   = 114,
    KEY_LOWERCASE_S   = 115,
    KEY_LOWERCASE_T   = 116,
    KEY_LOWERCASE_U   = 117,
    KEY_LOWERCASE_V   = 118,
    KEY_LOWERCASE_W   = 119,
    KEY_LOWERCASE_X   = 120,
    KEY_LOWERCASE_Y   = 121,
    KEY_LOWERCASE_Z   = 122,
    KEY_OPEN_BRACE    = 123,  /*  {  */
    KEY_PIPE          = 124,  /*  |  */
    KEY_CLOSE_BRACE   = 125,  /*  }  */
    KEY_TILDE         = 126,  /*  ~  */
    KEY_BACKSPACE     = 127,
    KEY_UP            = -1,
    KEY_DOWN          = -2,
    KEY_RIGHT         = -3,
    KEY_LEFT          = -4,
    KEY_HOME          = -5,
    KEY_END           = -6,
    KEY_STRING        = -7,   /* More than one key was recieved, probably from the clipboard. */
};

typedef enum oolong_key_e oolong_key_t;

/*
 * Disables canonical terminal input, causing stdin to not wait for a newline
 * and for functions like getc() to instantly return on a keystroke.
 */
oolong_error_t oolong_disable_canonical_input(void);

/*
 * Restores the terminal to its original state.
 */
oolong_error_t oolong_restore_canonical_input(void);

/*
 * Gets a single keypress, canonical input should be disabled.
 */
oolong_key_t oolong_keyboard_get_key(void);

/*
 * Gets a NULL terminated string of all wide characters read from the standard
 * input stream. Returns NULL if oolong_keyboard_get_key() has not been called.
 */
wchar_t* oolong_keyboard_get_string(void);

/*
 * Buffer keys to be returned by oolong_keyboard_get_key(). This is mostly for
 * making the input unit testable.
 */
oolong_error_t oolong_keyboard_buffer_keys(oolong_key_t* keys, size_t keys_length);

#endif // KEYBOARD_H
