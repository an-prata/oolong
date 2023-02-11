/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_TEXT_BOX_H
#define OOLONG_TEXT_BOX_H

#include "error.h"
#include "keyboard.h"
#include "element.h"

#define OOLONG_TEXT_BOX_SUPPORTED_STATES OOLONG_ELEMENT_STATE_NORMAL | OOLONG_ELEMENT_STATE_SELECTED | OOLONG_ELEMENT_STATE_ACTIVE | OOLONG_ELEMENT_STATE_DISABLED

typedef struct oolong_text_box_s oolong_text_box_t;
typedef struct oolong_text_box_options_s oolong_text_box_options_t;

struct oolong_text_box_options_s
{
	enum_t identifier;
	oolong_element_state_t state;
	oolong_alignment_t alignment;
	unsigned int padding;
	unsigned int width;
	oolong_style_set_t* style_normal;
	oolong_style_set_t* style_selected;
	oolong_style_set_t* style_active;
	oolong_style_set_t* style_disabled;
	wchar_t* display_text;
	oolong_key_t* activations_keys;		/* Should be terminated with KEY_NULL. */
	oolong_key_t* deactivation_keys;	/* Should be terminated with KEY_NULL. */
};

/*
 * Creates a new text box using the given text box options. The text box's 
 * struct  starts with an element struct and can therefore be cast to and used 
 * as an element.
 */
oolong_text_box_t* oolong_text_box_create(oolong_text_box_options_t* options);

/*
 * Frees all memory used by the text box. This will not free the content member
 * given with the options struct while creating the text box.
 */
oolong_error_t oolong_text_box_destroy(oolong_text_box_t* text_box);

/*
 * Register the given key with the text box. This adds the keystroke to the
 * entered text string if the text box's status is active. If any of the keys
 * given in the option's activation keys member are given the text box will make
 * itself active. If the text box is alreday active and a deactivation key is
 * given then the text box will become selected.
 */
oolong_error_t oolong_text_box_register_keystroke(oolong_text_box_t* text_box, oolong_key_t key);

/*
 * Get the user entered text from the given text box.
 */
wchar_t* oolong_text_box_get_entered_text(oolong_text_box_t* text_box);

#endif // OOLONG_TEXT_BOX_H

