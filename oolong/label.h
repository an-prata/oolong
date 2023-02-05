/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_LABEL_H
#define OOLONG_LABEL_H

#include "error.h"
#include "element.h"

#define OOLONG_LABEL_SUPPORTED_STATES OOLONG_ELEMENT_STATE_NORMAL

typedef struct oolong_label_s oolong_label_t;
typedef struct oolong_label_options_s oolong_label_options_t;

struct oolong_label_options_s
{
	enum_t identifier;
	oolong_element_content_alignment_t alignment;
	unsigned int padding;
	unsigned int width;
	oolong_style_set_t* style;
	wchar_t* content;
};

/*
 * Creates a new label using the given label options. The label's struct starts 
 * with an element struct and can therefore be cast to and used as an element.
 */
oolong_label_t* oolong_label_create(oolong_label_options_t* options);

/*
 * Frees all memory used by the label. This will not free the content member
 * given with the options struct while creating the label.
 */
oolong_error_t oolong_label_destroy(oolong_label_t* label);

#endif // OOLONG_LABEL_H