/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "label.h"

#define OOLONG_LABEL_SUPPORTED_STATES OOLONG_ELEMENT_STATE_NORMAL

struct oolong_label_s
{
	oolong_element_t element_data;
};

oolong_label_t* oolong_label_create(oolong_label_options_t* options)
{
	if (options == NULL)
	{
		oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
		return NULL;
	}

	oolong_label_t* label = malloc(sizeof(oolong_label_t));

	if (label == NULL)
	{
		oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
		return NULL;
	}

	label->element_data.identifier 			= options->identifier;
	label->element_data.supported_states 	= OOLONG_LABEL_SUPPORTED_STATES;
	label->element_data.state 				= OOLONG_ELEMENT_STATE_NORMAL;
	label->element_data.alignment 			= options->alignment;
	label->element_data.padding 			= options->padding;
	label->element_data.width 				= options->width;
	label->element_data.style_normal 		= options->style;
	label->element_data.style_selected 		= NULL;
	label->element_data.style_active		= NULL;
	label->element_data.style_disabled 		= NULL;
	label->element_data.content 			= options->content;
	label->element_data.string 				= NULL;

	return label;
}

oolong_error_t oolong_label_destroy(oolong_label_t* label)
{
	if (label == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	oolong_style_set_destroy(label->element_data.style_normal);
	free(label->element_data.string);
	free(label);
	return OOLONG_ERROR_NONE;
}

