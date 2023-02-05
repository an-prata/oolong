/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_BUTTON_H
#define OOLONG_BUTTON_H

#include "error.h"
#include "element.h"

typedef struct oolong_button_s oolong_button_t;
typedef struct oolong_button_options_s oolong_button_options_t;

struct oolong_button_options_s
{
	enum_t identifier;
	oolong_element_state_t state;
	oolong_element_content_alignment_t alignment;
	unsigned int padding;
	unsigned int width;
	oolong_style_set_t* style_normal;
	oolong_style_set_t* style_selected;
	oolong_style_set_t* style_disabled;
	wchar_t* content;
};

/*
 * Creates a new button using the given button options. The button's struct
 * starts with an element struct and can therefore be cast to and used as an 
 * element.
 */
oolong_button_t* oolong_button_create(oolong_button_options_t* options);

/*
 * Frees all memory used by the button. This will not free the content member
 * given with the options struct while creating the button.
 */
oolong_error_t oolong_button_destroy(oolong_button_t* button);

#endif // OOLONG_BUTTON_H
