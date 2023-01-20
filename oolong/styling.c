/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "styling.h"

static const wchar_t* style_escapes[] =
{
    [ OOLONG_STYLE_CLEAR             ] = (wchar_t*)U"\033[0m",
    [ OOLONG_STYLE_ITALIC            ] = (wchar_t*)U"\033[3m",
    [ OOLONG_STYLE_BOLD              ] = (wchar_t*)U"\033[1m",
    [ OOLONG_STYLE_UNDERLINE         ] = (wchar_t*)U"\033[4m",
    [ OOLONG_STYLE_RED               ] = (wchar_t*)U"\033[31m",
    [ OOLONG_STYLE_GREEN             ] = (wchar_t*)U"\033[32m",
    [ OOLONG_STYLE_BLUE              ] = (wchar_t*)U"\033[34m",
    [ OOLONG_STYLE_YELLOW            ] = (wchar_t*)U"\033[33m",
    [ OOLONG_STYLE_PURPLE            ] = (wchar_t*)U"\033[35m",
    [ OOLONG_STYLE_CYAN              ] = (wchar_t*)U"\033[36m",
    [ OOLONG_STYLE_WHITE             ] = (wchar_t*)U"\033[37m",
    [ OOLONG_STYLE_BLACK             ] = (wchar_t*)U"\033[30m",
    [ OOLONG_STYLE_BACKGROUND_RED    ] = (wchar_t*)U"\033[41m",
    [ OOLONG_STYLE_BACKGROUND_GREEN  ] = (wchar_t*)U"\033[42m",
    [ OOLONG_STYLE_BACKGROUND_BLUE   ] = (wchar_t*)U"\033[44m",
    [ OOLONG_STYLE_BACKGROUND_YELLOW ] = (wchar_t*)U"\033[43m",
    [ OOLONG_STYLE_BACKGROUND_PURPLE ] = (wchar_t*)U"\033[45m",
    [ OOLONG_STYLE_BACKGROUND_CYAN   ] = (wchar_t*)U"\033[46m",
    [ OOLONG_STYLE_BACKGROUND_WHITE  ] = (wchar_t*)U"\033[47m",
    [ OOLONG_STYLE_BACKGROUND_BLACK  ] = (wchar_t*)U"\033[40m",
};

oolong_style_set_t* oolong_style_set_create()
{
    oolong_style_set_t* style_set = calloc(1, sizeof(wchar_t));
    style_set[0] = U'\0';
    return style_set;
}

oolong_error_t oolong_style_set_add(oolong_style_set_t** style_set, oolong_style_t style)
{
    if (style_set == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
    
    size_t length = 0;
    for (; (*style_set)[length] != U'\0'; length++);
    size_t new_length = length + wcslen(style_escapes[style]);
    
    oolong_style_set_t* new_set = reallocarray(style_set, new_length, sizeof(oolong_style_t));

    if (new_set == NULL)
        return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);

    *style_set = new_set;

    for (size_t i = 0; i < new_length; i++)
        (*style_set)[i + length] = style_escapes[style][i];
    
    /* Probably not needed, but gives peace of mind. :D */
    (*style_set)[new_length] = U'\0';
    return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_style_set_destroy(oolong_style_set_t* style_set)
{
    if (style_set == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    /* 
     * Since all styles in the style set should be defined constants, we dont 
     * need to free them.
     */
    
    free(style_set);
    return OOLONG_ERROR_NONE;
}

