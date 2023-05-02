/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <stdio.h>
#include <string.h>
#include "element.h"

enum_t oolong_element_get_selected_identifier(oolong_element_t** elements, enum_t on_error)
{
	ssize_t selected_index = oolong_element_get_selected_index(elements);

	if (selected_index == -1)
		return on_error;

	return elements[selected_index]->identifier;
}

ssize_t oolong_element_get_selected_index(oolong_element_t** elements)
{
	if (elements == NULL)
		return -1;
	
	size_t selected_index = 0;

	while (elements[selected_index] != NULL)
	{
		if (elements[selected_index]->state == OOLONG_ELEMENT_STATE_SELECTED)
			return selected_index;

		selected_index++;
	}

	return -1;
}

oolong_error_t oolong_element_render_string(oolong_element_t* element)
{
	size_t element_string_length = 0;
	wchar_t* current_style = NULL;
	wchar_t* current_stye_end = OOLONG_STYLE_CLEAR_STRING;

	switch (element->state)
	{
		case (OOLONG_ELEMENT_STATE_NORMAL):		current_style = element->style_normal;		break;
		case (OOLONG_ELEMENT_STATE_SELECTED):	current_style = element->style_selected;	break;
		case (OOLONG_ELEMENT_STATE_ACTIVE):		current_style = element->style_active;		break;
		case (OOLONG_ELEMENT_STATE_DISABLED):	current_style = element->style_disabled;	break;
	}

	if (current_style == NULL)
	{
		current_style = L"";
		current_stye_end = L"";

		element->preceding_style_size = 0;
		element->following_style_size = 0;
	}
	else
	{
		element->preceding_style_size = wcslen(current_style);
		element->following_style_size = wcslen(current_stye_end);
	}

	element_string_length += element->padding * 2;
	element_string_length += wcslen(element->content);
	element_string_length = element_string_length < element->width ? element->width : element_string_length;

	element_string_length += element->preceding_style_size;
	element_string_length += element->following_style_size;
	
	if (element->string == NULL)
	{
		wchar_t* new_string = calloc(element_string_length + 1, sizeof *element->string);

		if (new_string == NULL)
			return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
		
		element->string = new_string;
		element->string[element_string_length] = L'\0';
	}
	else if (element_string_length != wcslen(element->string))
	{
		wchar_t* new_string = reallocarray(element->string, element_string_length + 1, sizeof *element->string);

		if (new_string == NULL)
			return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
		
		element->string = new_string;
		element->string[element_string_length] = L'\0';
	}

	size_t current_index = 0;

	wcscpy(element->string, current_style);
	current_index += element->preceding_style_size;

	unsigned int total_spaces = element_string_length - wcslen(current_style) - wcslen(OOLONG_STYLE_CLEAR_STRING) - wcslen(element->content) - (2 * element->padding);
	unsigned int preceding_spaces;
	unsigned int following_spaces;
	
	switch (element->alignment)
	{
		case (OOLONG_ALIGN_LEFT):
		{
			preceding_spaces = element->padding;
			following_spaces = total_spaces + element->padding;
			break;
		}

		case (OOLONG_ALIGN_CENTER):
		{
			unsigned int remainder = total_spaces % 2;
			unsigned int half_spaces = (total_spaces - remainder) / 2;
			preceding_spaces = half_spaces + element->padding;
			following_spaces = half_spaces + element->padding + remainder;
			break;
		}

		case (OOLONG_ALIGN_RIGHT):
		{
			preceding_spaces = total_spaces + element->padding;
			following_spaces = element->padding;
			break;
		}

		case (OOLONG_ALIGN_WIDTH):
		{
			/*
			 * Width align is not implementable here and can only be managed cleanly from
			 * whatever knows the margins of the current view.
			 */

			return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
		}
	}

	wmemset(&element->string[current_index], L' ', preceding_spaces);
	current_index += preceding_spaces;

	wcscpy(&element->string[current_index], element->content);
	current_index += wcslen(element->content);

	wmemset(&element->string[current_index], L' ', following_spaces);
	current_index += following_spaces;

	wcscpy(&element->string[current_index], current_stye_end);
	return OOLONG_ERROR_NONE;
}

wchar_t* oolong_element_get_string(oolong_element_t* element)
{
	if (element == NULL)
	{
		oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
		return NULL;
	}

	return element->string;
}

oolong_error_t oolong_element_select_next(oolong_element_t** elements)
{
	if (elements == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	ssize_t selected_index = oolong_element_get_selected_index(elements);

	if (selected_index == -1)
		return OOLONG_ERROR_NONE;

	ssize_t selected_index_next = selected_index + 1;

	while (selected_index_next != selected_index)
	{
		if (elements[selected_index_next] == NULL)
		{
			selected_index_next = 0;
			continue;
		}

		if (elements[selected_index_next]->supported_states & OOLONG_ELEMENT_STATE_SELECTED)
		{
			elements[selected_index]->state = OOLONG_ELEMENT_STATE_NORMAL;
			elements[selected_index_next]->state = OOLONG_ELEMENT_STATE_SELECTED;
			return OOLONG_ERROR_NONE;
		}
		
		selected_index_next++;
	}

	return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_element_select_previous(oolong_element_t** elements)
{
	if (elements == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	ssize_t selected_index = oolong_element_get_selected_index(elements);

	if (selected_index == -1)
		return OOLONG_ERROR_NONE;

	ssize_t selected_index_previous = selected_index - 1;

	while (selected_index_previous != selected_index)
	{
		if (selected_index_previous < 0)
			while (elements[selected_index_previous + 1] != NULL)
				selected_index_previous++;

		if (elements[selected_index_previous]->supported_states & OOLONG_ELEMENT_STATE_SELECTED)
		{
			elements[selected_index]->state = OOLONG_ELEMENT_STATE_NORMAL;
			elements[selected_index_previous]->state = OOLONG_ELEMENT_STATE_SELECTED;
			return OOLONG_ERROR_NONE;
		}
		
		selected_index_previous--;
	}

	return OOLONG_ERROR_NONE;
}

size_t oolong_element_get_preceding_style_size(oolong_element_t* element)
{
	if (element == NULL || element->string == NULL)
	{
		oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
		return 0;
	}

	return element->preceding_style_size;
}

size_t oolong_element_get_following_style_size(oolong_element_t* element)
{
	
	if (element == NULL || element->string == NULL)
	{
		oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
		return 0;
	}

	return element->following_style_size;
}

