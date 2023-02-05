/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

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
		if (elements[selected_index] == NULL)
			return -1;

		if (elements[selected_index]->state == OOLONG_ELEMENT_STATE_SELECTED)
			break;

		selected_index++;
	}

	return selected_index;
}

oolong_error_t oolong_element_render_string(oolong_element_t* element)
{
	size_t element_string_length = 0;
	oolong_style_set_t* current_style = NULL;

	switch (element->state)
	{
		case (OOLONG_ELEMENT_STATE_NORMAL):		current_style = element->style_normal;		break;
		case (OOLONG_ELEMENT_STATE_SELECTED):	current_style = element->style_selected;	break;
		case (OOLONG_ELEMENT_STATE_ACTIVE):		current_style = element->style_active;		break;
		case (OOLONG_ELEMENT_STATE_DISABLED):	current_style = element->style_disabled;	break;
	}

	element_string_length += wcslen(current_style);
	element_string_length += element->padding * 2;
	element_string_length += wcslen(element->content);
	element_string_length = element_string_length < element->width ? element->width : element_string_length;

	if (element_string_length > wcslen(element->string))
	{
		wchar_t* new_string = calloc(element_string_length + 1, sizeof(wchar_t));

		if (new_string == NULL)
			return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
		
		free(element->string);
		element->string = new_string;
		element->string[element_string_length] = L'\0';
	}

	size_t current_index = 0;

	wcscpy(element->string, current_style);
	current_index += wcslen(current_style);

	wmemset(&element->string[current_index], L' ', element->padding);
	current_index += element->padding;
	
	wcscpy(&element->string[current_index], element->content);
	current_index += wcslen(element->content);

	wmemset(&element->string[current_index], L' ', (element_string_length - 1) - current_index);
	return OOLONG_ERROR_NONE;
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
		if (elements[selected_index_next]->supported_states & OOLONG_ELEMENT_STATE_SELECTED)
		{
			elements[selected_index]->state = OOLONG_ELEMENT_STATE_NORMAL;
			elements[selected_index_next]->state = OOLONG_ELEMENT_STATE_SELECTED;
			return OOLONG_ERROR_NONE;
		}
		
		selected_index_next++;
		
		if (elements[selected_index_next] == NULL)
			selected_index_next = 0;
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

	ssize_t selected_index_previous = selected_index + 1;

	while (selected_index_previous != selected_index)
	{
		if (elements[selected_index_previous]->supported_states & OOLONG_ELEMENT_STATE_SELECTED)
		{
			elements[selected_index]->state = OOLONG_ELEMENT_STATE_NORMAL;
			elements[selected_index_previous]->state = OOLONG_ELEMENT_STATE_SELECTED;
			return OOLONG_ERROR_NONE;
		}
		
		if (selected_index_previous == 0)
			for (; elements[selected_index_previous] != NULL; selected_index_previous++);

		selected_index_previous--;
	}

	return OOLONG_ERROR_NONE;
}

