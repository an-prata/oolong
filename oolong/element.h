/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_ELEMENT_H
#define OOLONG_ELEMENT_H

#include <stdlib.h>
#include "styling.h"

#ifndef __GNUC__
#warning "OOLONG: not compiling with GCC, element.h utilises GCC compiler extensions."
#endif // __GNUC__

/*
 * An element's state is mostly used to determine the style in which to render
 * it, but also has a behavioral change in some functions, for example, a 
 * select next/previous function shall not affect either disabled nor active
 * elements. A select next/previous function should simply search for the first
 * occurance of a selected element, and then set the first found normal element
 * to selected in whichever direction is appropriate.
 *
 * An element does not have to support all states and may replace style sets for
 * those states with NULL. For all supported states using the bitwise '&' on the
 * supported states member should result in the supported state, otherwise 0.
 * The only requirment for state support is that any element supporting 
 * selection must also support a normal state.
 */
enum oolong_element_state_e
{
	OOLONG_ELEMENT_STATE_NORMAL		= 0b0001,
	OOLONG_ELEMENT_STATE_SELECTED	= 0b0010,
	OOLONG_ELEMENT_STATE_ACTIVE		= 0b0100,
	OOLONG_ELEMENT_STATE_DISABLED	= 0b1000
};

/*
 * Alignment only aligns the content within the element, the element itself does
 * not render centered, left, or right as it renders only to a string its exact
 * size therefore placing the element's overall alignment on the view.
 */
enum oolong_alignment_e
{
	OOLONG_ALIGN_LEFT,
	OOLONG_ALIGN_CENTER,
	OOLONG_ALIGN_RIGHT,
	OOLONG_ALIGN_WIDTH
};

typedef int enum_t;
typedef enum oolong_element_state_e oolong_element_state_t;
typedef enum oolong_alignment_e oolong_alignment_t;
typedef struct oolong_element_s oolong_element_t;

/*
 * By making an instance of an element the first member of a struct you can
 * cast it to an element and it can then be used in any functions that take an
 * element, for this reason none of the afformentioned functions will mess with
 * the element's memory block and all memory management is the responsibility of
 * the containing structure.
 */

struct oolong_element_s
{
	enum_t identifier;							/* The enum value (or int) by which the element will be identified. */
	oolong_element_state_t supported_states;	/* The supported states of the element. */
	oolong_element_state_t state;				/* Current state of the element. */
	oolong_alignment_t alignment;				/* Alignment of content within the element. */
	unsigned int padding;						/* Padding of content within the element. */
	unsigned int width;							/* Minimum width of the element, it may extend further. */
	oolong_style_set_t* style_normal;			/* Style while state is normal. */
	oolong_style_set_t* style_selected;			/* Style while state is selected. */
	oolong_style_set_t* style_active;			/* Style while state is active. */
	oolong_style_set_t* style_disabled;			/* Style while state is disabled. */
	wchar_t* content;							/* Pointer to the element's content. */
	wchar_t* string;							/* Output of rendering the element. */
	size_t preceding_style_size;				/* Number of style characters at the beginning of the rendered string. */
	size_t following_style_size;				/* Number of style characters at the end of the rendered string. */
};

/*
 * Returns the first selected element's identifier. Returns the given 'on_error'
 * enum value if no selected element is found.
 *
 * This function assumes that the array is terminated with a NULL pointer.
 */
enum_t oolong_element_get_selected_identifier(oolong_element_t** elements, enum_t on_error);

/*
 * Returns the first selected element's index. Returns -1 if none where found.
 *
 * This function assumes that the array is terminated with a NULL pointer.
 */
ssize_t oolong_element_get_selected_index(oolong_element_t** elements);

/*
 * Renders the element and makes it's member 'string' point to the result. This
 * function will allocate memory if 'string' is NULL and reallocate memory if
 * the string's current length is to short to hold the result, this is done
 * regardless of the actual size of the memory block. This function will not
 * reallocate to less that the previous size unless the string is terminated
 * before the end of the memory block.
 */
oolong_error_t oolong_element_render_string(oolong_element_t* element);

/*
 * Gets the element's rendered string.
 */
wchar_t* oolong_element_get_string(oolong_element_t* element);

/*
 * Sets the first found selected element's state to normal and makes the next 
 * element found to support the selected state selected. This function will 
 * loop from the end of the given array of elements back to the beginning if no
 * elements are found to support selection before then, it stops looping when it
 * reached the original selection.
 *
 * This function assumes that the array is terminated with a NULL pointer. Not
 * finding a selection is not considered an error and simply returns.
 */
oolong_error_t oolong_element_select_next(oolong_element_t** elements);

/*
 * Sets the first found selected element's state to normal and makes the next 
 * element at a lesser index found to support the selected state selected. This 
 * function will loop from the beginning of the given array of elements back to
 * its end if no elements are found to support selection before then, it stops 
 * looping when it reached the original selection.
 *
 * This function assumes that the array is terminated with a NULL pointer. Not
 * finding a selection is not considered an error and simply returns.
 */
oolong_error_t oolong_element_select_previous(oolong_element_t** elements);

/*
 * Gets the number of characters that belong to the zero width style escape
 * sequence that precede the displayed characters of the element.
 */
size_t oolong_element_get_preceding_style_size(oolong_element_t* element);

/*
 * Gets the number of characters that belong to the zero width style escape
 * sequence that follow the displayed characters of the element.
 */
size_t oolong_element_get_following_style_size(oolong_element_t* element);

#endif // OOLONG_ELEMENT_H

