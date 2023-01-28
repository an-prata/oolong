/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "page.h"

oolong_error_t oolong_page_print(oolong_page_t* page, file_t* file)
{
	if (page == NULL || file == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	switch (page->view_type)
	{
		case (OOLONG_VIEW_TYPE_STACK):
			return oolong_stack_view_print(page->view.stack_view, file);
		case (OOLONG_VIEW_TYPE_DIALOG):
			return oolong_dialog_view_print(page->view.dialog_view, file);
		default:
			return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
	}
}

oolong_error_t oolong_page_select_next_element(oolong_page_t* page)
{
	if (page == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	switch (page->view_type)
	{
		case (OOLONG_VIEW_TYPE_STACK):
			return oolong_stack_view_select_next_element(page->view.stack_view);
		case (OOLONG_VIEW_TYPE_DIALOG):
			return oolong_dialog_view_select_next(page->view.dialog_view);
		default:
			return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
	}
}

oolong_error_t oolong_page_select_previous_element(oolong_page_t* page)
{
	if (page == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	switch (page->view_type)
	{
		case (OOLONG_VIEW_TYPE_STACK):
			return oolong_stack_view_select_previous_element(page->view.stack_view);
		case (OOLONG_VIEW_TYPE_DIALOG):
			return oolong_dialog_view_select_previous(page->view.dialog_view);
		default:
			return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
	}
}

enum_t oolong_page_get_selected_identifier(oolong_page_t* page)
{
	if (page == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	switch (page->view_type)
	{
		case (OOLONG_VIEW_TYPE_STACK):
			return oolong_stack_view_get_selected_identifier(page->view.stack_view);
		case (OOLONG_VIEW_TYPE_DIALOG):
			return oolong_dialog_get_selected_identifier(page->view.dialog_view);
		default:
			oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
			return -1;
	}
}

