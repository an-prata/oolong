/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef ESCAPES_H
#define ESCAPES_H

#include "error.h"

#define oolong_terminal_enter_alternate_screen(file) fwprintf(file, L"\033[?47h")
#define oolong_terminal_exit_alternate_screen(file) fwprintf(file, L"\033[?47l")
#define oolong_terminal_hide_cursor(file) fwprintf(file, L"\033[?25l")
#define oolong_terminal_show_cursor(file) fwprintf(file, L"\033[?25h")
#define oolong_terminal_set_cursor_position(column, row, file) fwprintf(file, L"\033[%zu;%zuH", row, column)
#define oolong_terminal_clear(file) fwprintf(file, L"\033[2J")

#endif // ESCAPES_H

