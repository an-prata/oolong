/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "text_box.h"

struct oolong_text_box_s
{
	oolong_element_t element_data;
	wchar_t* display_text;
	wchar_t* entered_text;
	oolong_key_t* activation_keys;
	oolong_key_t* deactivation_keys;
};

static bool contains_key(oolong_key_t* key_array, oolong_key_t key)
{
	if (key_array == NULL)
		return false;
	
	for (size_t index = 0; key_array[index] != KEY_NULL; index++)
		if (key_array[index] == key)
			return true;

	return false;
}

oolong_text_box_t* oolong_text_box_create(oolong_text_box_options_t* options)
{
	if (options == NULL)
	{
		oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
		return NULL;
	}

	oolong_text_box_t* text_box = malloc(sizeof *text_box);

	if (text_box == NULL)
	{
		oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
		return NULL;
	}

	text_box->activation_keys		= options->activations_keys;
	text_box->deactivation_keys		= options->activations_keys;
	text_box->display_text			= options->display_text;
	text_box->entered_text			= calloc(1, sizeof *text_box->entered_text);

	text_box->element_data.identifier 			= options->identifier;
	text_box->element_data.supported_states 	= OOLONG_TEXT_BOX_SUPPORTED_STATES;
	text_box->element_data.state 				= options->state;
	text_box->element_data.alignment 			= options->alignment;
	text_box->element_data.padding 				= options->padding;
	text_box->element_data.width 				= options->width;
	text_box->element_data.style_normal 		= options->style_normal;
	text_box->element_data.style_selected 		= options->style_selected;
	text_box->element_data.style_active			= options->style_active;
	text_box->element_data.style_disabled 		= options->style_disabled;
	text_box->element_data.string 				= NULL;
	text_box->element_data.content				= text_box->display_text;
	
	return text_box;
}

oolong_error_t oolong_text_box_destroy(oolong_text_box_t* text_box)
{
	if (text_box == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	oolong_style_set_destroy(text_box->element_data.style_normal);
	free(text_box->element_data.string);
	free(text_box);
	return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_text_box_register_keystroke(oolong_text_box_t* text_box, oolong_key_t key)
{
	if (text_box == NULL || key == KEY_NULL || key == KEY_ERROR)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	if (key == KEY_STRING)
		return OOLONG_ERROR_NONE;

	if (text_box->element_data.state == OOLONG_ELEMENT_STATE_SELECTED && contains_key(text_box->activation_keys, key))
	{
		text_box->element_data.state = OOLONG_ELEMENT_STATE_ACTIVE;
		goto update_content;
	}

	if (text_box->element_data.state == OOLONG_ELEMENT_STATE_ACTIVE && contains_key(text_box->deactivation_keys, key))
	{
		text_box->element_data.state = OOLONG_ELEMENT_STATE_SELECTED;
		goto update_content;
	}

	size_t entered_text_length = wcslen(text_box->entered_text);
	
	if (key == KEY_BACKSPACE)
	{
		if (entered_text_length == 0)
			return OOLONG_ERROR_NONE;

		/*
		 * This reallocation is not strictly necessary but I do it anyways since it 
		 * will prevent text boxes from taking up an abatrary amount of memory. Bonous
		 * is that reallocating to less memory costs very little.
		 */

		wchar_t* new_text = reallocarray(text_box->entered_text, entered_text_length, sizeof *text_box->entered_text);
		entered_text_length--;

		if (new_text == NULL)
			return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);

		text_box->entered_text[entered_text_length] = L'\0';
		text_box->entered_text = new_text;
		goto update_content;
	}

	/*
	 * Make sure we have a key that can cast to a supported character.
	 */
	
	if (key < KEY_SPACE || key > KEY_TILDE)
		return OOLONG_ERROR_NONE;

	wchar_t* new_text = reallocarray(text_box->entered_text, entered_text_length + 2, sizeof *text_box->entered_text);
	entered_text_length++;

	if (new_text == NULL)
		return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);

	text_box->entered_text = new_text;
	text_box->entered_text[entered_text_length] = L'\0';
	text_box->entered_text[entered_text_length - 1] = (wchar_t)key;
	goto update_content;
	
update_content:
	if (text_box->element_data.state == OOLONG_ELEMENT_STATE_ACTIVE || wcslen(text_box->entered_text) > 0)
	{
		text_box->element_data.content = text_box->entered_text;
		return OOLONG_ERROR_NONE;
	}

	text_box->element_data.content = text_box->display_text;
	return OOLONG_ERROR_NONE;
}

wchar_t* oolong_text_box_get_entered_text(oolong_text_box_t* text_box)
{
	if (text_box == NULL)
	{
		oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
		return NULL;
	}

	return text_box->entered_text;
}

