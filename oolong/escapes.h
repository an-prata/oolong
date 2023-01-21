/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef ESCAPES_H
#define ESCAPES_H

#include "error.h"

#define OOLONG_TEXT_NORMAL "\033[0m"
#define OOLONG_TEXT_BOLD "\033[1m"
#define OOLONG_TEXT_DIM "\033[2m"
#define OOLONG_TEXT_ITALIC "\033[3m"
#define OOLONG_TEXT_UNDERLINE "\033[4m"
#define OOLONG_TEXT_BLINK "\033[5m"
#define OOLONG_TEXT_INVERTED "\033[7m"
#define OOLONG_TEXT_HIDDEN "\033[8m"

#define OOLONG_TEXT_HIGHIGHT_BLACK "\033[40m"
#define OOLONG_TEXT_HIGHIGHT_RED "\033[41m"
#define OOLONG_TEXT_HIGHIGHT_GREEN "\033[42m"
#define OOLONG_TEXT_HIGHIGHT_YELLOW "\033[43m"
#define OOLONG_TEXT_HIGHIGHT_BLUE "\033[44m"
#define OOLONG_TEXT_HIGHIGHT_PURPLE "\033[45m"
#define OOLONG_TEXT_HIGHIGHT_CYAN "\033[46m"
#define OOLONG_TEXT_HIGHIGHT_WHITE "\033[47m"

#define oolong_terminal_enter_alternate_screen() wprintf(L"\033[?47h")
#define oolong_terminal_exit_alternate_screen() wprintf(L"\033[?47l")
#define oolong_terminal_hide_cursor() wprintf(L"\033[?25l")
#define oolong_terminal_show_cursor() wprintf(L"\033[?25h")
#define oolong_terminal_set_cursor_position(column, row) wprintf(L"\033[%zu;%zuH", column, row);
#define oolong_terminal_clear() wprintf(L"\033[2J")

#endif // ESCAPES_H

