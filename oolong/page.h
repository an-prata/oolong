/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_PAGE_H
#define OOLONG_PAGE_H

/*
 * The purpose of oolong's page structure is to provide a common interface by
 * which all views can be used. It will support changing the selected option,
 * getting the selected option's identifier, and printing the view; beyond
 * those three things all tasks are to be done with the view directly at the
 * discretion of the programmer.
 */

#include <stdio.h>
#include "stack_view.h"
#include "dialog_view.h"
#include "error.h"

enum oolong_view_type_e
{
	OOLONG_VIEW_TYPE_STACK,
	OOLONG_VIEW_TYPE_DIALOG
};

typedef int enum_t;
typedef FILE file_t;
typedef enum oolong_view_type_e oolong_view_type_t;
typedef union oolong_view_u oolong_view_t;
typedef struct oolong_page_s oolong_page_t;

union oolong_view_u
{
	oolong_stack_view_t* stack_view;
	oolong_dialog_view_t* dialog_view;
};

struct oolong_page_s
{
	oolong_view_t view;
	oolong_view_type_t view_type;
};

/*
 * Prints the given page to the given file or stream.
 */
oolong_error_t oolong_page_print(oolong_page_t* page, file_t* file);

/*
 * Select the next element of the given page's member view.
 */
oolong_error_t oolong_page_select_next_element(oolong_page_t* page);

/*
 * Select the previous element of the given page's member view.
 */
oolong_error_t oolong_page_select_previous_element(oolong_page_t* page);

/*
 * Get the given page's member view's currently selected element's identifier.
 * Returns -1 on error.
 */
enum_t oolong_page_get_selected_identifier(oolong_page_t* page);

#endif // OOONG_PAGE_H

