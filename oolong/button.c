/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "button.h"

#define OOLONG_BUTTON_SUPPORTED_STATES OOLONG_ELEMENT_STATE_NORMAL | OOLONG_ELEMENT_STATE_SELECTED | OOLONG_ELEMENT_STATE_DISABLED

struct oolong_button_s
{
	oolong_element_t element_data;
};

oolong_button_t* oolong_button_create(oolong_button_options_t* options)
{
	if (options == NULL)
	{
		oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
		return NULL;
	}

	oolong_button_t* button = malloc(sizeof(oolong_button_t));

	if (button == NULL)
	{
		oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
		return NULL;
	}

	button->element_data.identifier 		= options->identifier;
	button->element_data.supported_states 	= OOLONG_BUTTON_SUPPORTED_STATES;
	button->element_data.state 				= options->state;
	button->element_data.alignment 			= options->alignment;
	button->element_data.padding 			= options->padding;
	button->element_data.width 				= options->width;
	button->element_data.style_normal 		= options->style_normal;
	button->element_data.style_selected 	= options->style_selected;
	button->element_data.style_disabled 	= options->style_disabled;
	button->element_data.content 			= options->content;
	button->element_data.string 			= NULL;

	return button;
}

oolong_error_t oolong_button_destroy(oolong_button_t* button)
{
	if (button == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	free(button->element_data.string);
	free(button);
	return OOLONG_ERROR_NONE;
}

