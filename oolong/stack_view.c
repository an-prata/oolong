/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <stdio.h>
#include "error.h"
#include "screen.h"
#include "stack_view.h"

static oolong_error_t print_left_aligned(oolong_stack_view_t* view, file_t* file)
{
	unsigned int columns;
	unsigned int content_columns;

	oolong_get_screen_dimensions(&columns, NULL);
	content_columns = columns - (2 * view->margin_sides);

	for (size_t index = 0; view->elements[index]; index++)
	{
		for (unsigned int i = 0; i < view->margin_sides; i++)
			putwc(L' ', file);

		oolong_element_render_string(view->elements[index]);
		wchar_t* element_string = oolong_element_get_string(view->elements[index]);

		for (size_t element_string_index = 0; element_string[element_string_index] != L'\0'; element_string_index++)
		{
			if ((element_string_index - oolong_element_get_preceding_style_size(view->elements[index])) % (content_columns - 1))
				putwc(L'\n', file);

			putwc(element_string[element_string_index], file);
		}

		if (view->elements[index + 1])
			for (unsigned int i = 0; i < view->element_gap; i++)
				putwc(L'\n', file);
	}

	if (ferror(file))
		return oolong_error_record(OOLONG_ERROR_FAILED_IO_WRITE);

	return OOLONG_ERROR_NONE;
}

static oolong_error_t print_center_aligned(oolong_stack_view_t* view, file_t* file)
{
	unsigned int columns;
	unsigned int content_columns;

	oolong_get_screen_dimensions(&columns, NULL);
	content_columns = columns - (2 * view->margin_sides);

	for (size_t index = 0; view->elements[index]; index++)
	{
		oolong_element_render_string(view->elements[index]);
		
		unsigned int preceding_spaces = 0;
		wchar_t* element_string = oolong_element_get_string(view->elements[index]);
		size_t element_string_length = wcslen(element_string) - oolong_element_get_preceding_style_size(view->elements[index]) - oolong_element_get_following_style_size(view->elements[index]);

		if (element_string_length < content_columns)
		{
			unsigned int total_spaces = content_columns - element_string_length;
			unsigned int remainder = total_spaces % content_columns;
			preceding_spaces = (total_spaces - remainder) / 2;
		}

		for (unsigned int i = 0; i < preceding_spaces; i++)
			putwc(L' ', file);

		fwprintf(file, L"%ls\n", element_string);

		if (view->elements[index + 1] != NULL)
			for (unsigned int i = 0; i < view->element_gap; i++)
				putwc(L'\n', file);
	}

	if (ferror(file))
		return oolong_error_record(OOLONG_ERROR_FAILED_IO_WRITE);

	return OOLONG_ERROR_NONE;
}

static oolong_error_t print_right_aligned(oolong_stack_view_t* view, file_t* file)
{
	unsigned int columns;
	unsigned int content_columns;

	oolong_get_screen_dimensions(&columns, NULL);
	content_columns = columns - (2 * view->margin_sides);

	for (size_t index = 0; view->elements[index]; index++)
	{
		oolong_element_render_string(view->elements[index]);

		wchar_t* element_string = oolong_element_get_string(view->elements[index]);
		unsigned int preceding_spaces = 0;
		size_t element_string_length = wcslen(element_string) - oolong_element_get_preceding_style_size(view->elements[index]) - oolong_element_get_following_style_size(view->elements[index]);

		if (element_string_length < content_columns)
			preceding_spaces = content_columns - element_string_length;

		for (unsigned int i = 0; i < preceding_spaces; i++)
			putwc(L' ', file);

		fwprintf(file, L"%ls\n", element_string);

		if (view->elements[index + 1])
			for (unsigned int i = 0; i < view->element_gap; i++)
				putwc(L'\n', file);
	}

	if (ferror(file))
		return oolong_error_record(OOLONG_ERROR_FAILED_IO_WRITE);

	return OOLONG_ERROR_NONE;
}

static oolong_error_t print_width_aligned(oolong_stack_view_t* view, file_t* file)
{
	unsigned int columns;
	unsigned int content_columns;

	oolong_get_screen_dimensions(&columns, NULL);
	content_columns = columns - (2 * view->margin_sides);

	for (size_t index = 0; view->elements[index]; index++)
		view->elements[index]->width = content_columns;

	return print_left_aligned(view, file);
}

oolong_error_t oolong_stack_view_print(oolong_stack_view_t* view, file_t* file)
{
	if (view == NULL || file == NULL || view->elements == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	for (unsigned int i = 0; i < view->margin_top; i++)
		putwc(L'\n', file);

	switch (view->alignment)
	{
		case (OOLONG_ALIGN_LEFT):	return print_left_aligned(view, file);
		case (OOLONG_ALIGN_CENTER):	return print_center_aligned(view, file);
		case (OOLONG_ALIGN_RIGHT):	return print_right_aligned(view, file);
		case (OOLONG_ALIGN_WIDTH):	return print_width_aligned(view, file);
	}

	return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
}

