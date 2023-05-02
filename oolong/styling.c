/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "styling.h"
#include <stdio.h>

static const wchar_t* style_escapes[] =
{
    [ OOLONG_STYLE_CLEAR             ] = L"\033[0m",
    [ OOLONG_STYLE_ITALIC            ] = L"\033[3m",
    [ OOLONG_STYLE_BOLD              ] = L"\033[1m",
    [ OOLONG_STYLE_UNDERLINE         ] = L"\033[4m",
    [ OOLONG_STYLE_RED               ] = L"\033[31m",
    [ OOLONG_STYLE_GREEN             ] = L"\033[32m",
    [ OOLONG_STYLE_BLUE              ] = L"\033[34m",
    [ OOLONG_STYLE_YELLOW            ] = L"\033[33m",
    [ OOLONG_STYLE_PURPLE            ] = L"\033[35m",
    [ OOLONG_STYLE_CYAN              ] = L"\033[36m",
    [ OOLONG_STYLE_WHITE             ] = L"\033[37m",
    [ OOLONG_STYLE_BLACK             ] = L"\033[30m",
    [ OOLONG_STYLE_BACKGROUND_RED    ] = L"\033[41m",
    [ OOLONG_STYLE_BACKGROUND_GREEN  ] = L"\033[42m",
    [ OOLONG_STYLE_BACKGROUND_BLUE   ] = L"\033[44m",
    [ OOLONG_STYLE_BACKGROUND_YELLOW ] = L"\033[43m",
    [ OOLONG_STYLE_BACKGROUND_PURPLE ] = L"\033[45m",
    [ OOLONG_STYLE_BACKGROUND_CYAN   ] = L"\033[46m",
    [ OOLONG_STYLE_BACKGROUND_WHITE  ] = L"\033[47m",
    [ OOLONG_STYLE_BACKGROUND_BLACK  ] = L"\033[40m",
};

oolong_style_set_t* oolong_style_set_create()
{
    oolong_style_set_t* style_set = calloc(1, sizeof *style_set);
    style_set[0] = L'\0';
    return style_set;
}

oolong_error_t oolong_style_set_add(oolong_style_set_t** style_set, oolong_style_t style)
{
    if (style_set == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    if (style == OOLONG_STYLE_CLEAR)
    {
        oolong_style_set_destroy(*style_set);
        *style_set = oolong_style_set_create();
        return OOLONG_ERROR_NONE;
    }
    
    size_t set_length = wcslen(*style_set);
    size_t style_length = wcslen(style_escapes[style]) + 1;
    
    oolong_style_set_t* new_set = reallocarray(*style_set, set_length + style_length, sizeof(oolong_style_t));

    if (new_set == NULL)
        return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);

    *style_set = new_set;

    for (size_t i = 0; i < style_length; i++)
        (*style_set)[i + set_length] = style_escapes[style][i];
    
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

