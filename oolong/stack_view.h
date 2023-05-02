/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_STACK_VIEW_H
#define OOLONG_STACK_VIEW_H

#include "element.h"

struct oolong_stack_view_s
{
	oolong_element_t** elements;	/* This view's elements, terminated with NULL. */
	oolong_alignment_t alignment;	/* Alignment of the view, width align will change the width of elements. */
	unsigned int margin_top;		/* Number of newlines from top of terminal to first element. */
	unsigned int margin_sides;		/* Number of spaces of either side to an element. */
	unsigned int element_gap;		/* Number of newlines between elements. */
};

typedef FILE file_t;
typedef struct oolong_stack_view_s oolong_stack_view_t;

/*
 * Prints the given stack view to the given file. If the view is width aligned
 * then this function will change the view's element's widths to align to the
 * terminal properly.
 */
oolong_error_t oolong_stack_view_print(oolong_stack_view_t* view, file_t* file);

#endif // OOLONG_STACK_VIEW_H

