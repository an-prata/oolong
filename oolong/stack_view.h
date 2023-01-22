/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef STACK_VIEW_H
#define STACK_VIEW_H

#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>
#include "styling.h"
#include "keyboard.h"
#include "error.h"

enum oolong_alignment_e
{
    OOLONG_ALIGN_LEFT,
    OOLONG_ALIGN_CENTER,
    OOLONG_ALIGN_RIGHT,
    OOLONG_ALIGN_DEFAULT = OOLONG_ALIGN_LEFT
};

enum oolong_stack_view_element_type_e
{
    OOLONG_ELEMENT_TYPE_LABEL,
    OOLONG_ELEMENT_TYPE_BUTTON,
	OOLONG_ELEMENT_TYPE_TEXT_BOX
};

enum oolong_stack_view_element_state_e
{
    OOLONG_ELEMENT_STATE_NORMAL,
    OOLONG_ELEMENT_STATE_SELECTED,
	OOLONG_ELEMENT_STATE_ACTIVE,
    OOLONG_ELEMENT_STATE_DISABLED
};

typedef int enum_t;  /* Serves only to clarify use as a generic enum type. */
typedef FILE file_t;
typedef enum oolong_alignment_e oolong_alignment_t;
typedef enum oolong_stack_view_element_type_e oolong_stack_view_element_type_t;
typedef enum oolong_stack_view_element_state_e oolong_stack_view_element_state_t;
typedef union oolong_stack_view_element_data_u oolong_stack_view_element_data_t;
typedef struct oolong_stack_view_s oolong_stack_view_t;
typedef struct oolong_stack_view_options_s oolong_stack_view_options_t;
typedef struct oolong_stack_view_element_s oolong_stack_view_element_t;
typedef struct oolong_stack_view_label_data_s oolong_stack_view_label_data_t;
typedef struct oolong_stack_view_button_data_s oolong_stack_view_button_data_t;
typedef struct oolong_stack_view_text_box_data_s oolong_stack_view_text_box_data_t;

struct oolong_stack_view_options_s
{
    size_t element_width;      /* Width of each element. */
    size_t element_padding;    /* Padding on the sides of each element. */
    size_t element_gap;        /* Gap between elements. */
    size_t view_side_margin;   /* Margin from side of terminal to view. */
    size_t view_top_margin;    /* Margin from top of terminal to view. */
    oolong_alignment_t align;  /* Alignment for view elements. */
};

struct oolong_stack_view_label_data_s
{
    wchar_t* text;              /* Pointer to the label's text. */
    oolong_style_set_t* style;  /* An array of wide string escape codes to style text. */
};

struct oolong_stack_view_button_data_s
{
    wchar_t* text;                            /* Pointer to the button's text. */
    oolong_stack_view_element_state_t state;  /* State of the element. */
    oolong_style_set_t* style;                /* An array of wide string escape codes to style text. */
    oolong_style_set_t* style_selected;       /* An array of wide string escape codes to style text while selected. */
};

struct oolong_stack_view_text_box_data_s
{
	wchar_t* display_text;                       /* Text to show while no text is entered by the user. */
	wchar_t* entered_text;                       /* Text entered by the user. Should be initialized to NULL. */
	oolong_stack_view_element_state_t state;     /* Text box state. */
	oolong_style_set_t* display_style;           /* Style while showing display text. */
	oolong_style_set_t* display_style_selected;  /* Style while showing display text and selected. */
	oolong_style_set_t* entered_style;           /* Style while the user has entered text. */
	oolong_style_set_t* entered_style_selected;  /* Style while the user has entered text and selected. */
};

union oolong_stack_view_element_data_u
{
    oolong_stack_view_label_data_t label;
    oolong_stack_view_button_data_t button;
	oolong_stack_view_text_box_data_t text_box;
};

struct oolong_stack_view_element_s
{
    oolong_stack_view_element_type_t type;  /* The type of element, denotes the backing struct of data. */
    oolong_stack_view_element_data_t data;  /* The elements data. */
    enum_t identifier;                      /* Not used by oolong, for indentifying the element elsewhere. */
};

/*
 * Create a new stack view given stack view options.
 */
oolong_stack_view_t* oolong_stack_view_create(oolong_stack_view_options_t* options);

/*
 * Frees all memory used by the given stack view. Note that this will not free
 * any options or elements passed into functions as pointers, only memory
 * allocated by a stack view function will be freed.
 */
void oolong_stack_view_destroy(oolong_stack_view_t* stack_view);

/*
 * Add an element to the given stack view.
 */
oolong_error_t oolong_stack_view_add_element(oolong_stack_view_t* stack_view, oolong_stack_view_element_t* element);

/*
 * Gets the first selected or active element's identifier. -1 if none are found.
 */
enum_t oolong_stack_view_get_selected_identifier(oolong_stack_view_t* stack_view);

/*
 * Deselects the first selected element and selected the next. If the last
 * element is the only selected element, then it is deselected and the first
 * element is selected.
 *
 * If the given stack view pointer is NULL, or if it contains less than two
 * elements, then this function does nothing.
 */
void oolong_stack_view_select_next_element(oolong_stack_view_t* stack_view);

/*
 * Deselected the first selected element and selected the previous. If the
 * first element is selected then it is deselected and the last element is
 * selected.
 *
 * If the given stack view pointer is NULL, or if it contains less than two
 * elements, then this function does nothing.
 *
 * Since I beleive in good documentation I'll tell you that if you have 
 * SIZE_MAX elements then this function will break, since it detects the 
 * underflow of size_t using that constant making it impossible to select. I 
 * dont think anyone needs more that even 4 bits to store their maximum index 
 * though, and if you do... well, lets just say you should work on your UI a 
 * bit...
 */
void oolong_stack_view_select_previous_element(oolong_stack_view_t* stack_view);

/*
 * Returns true if the given stack view has an active text box.
 */
bool oolong_stack_view_get_is_text_box_active(oolong_stack_view_t* stack_view);

/*
 * Performs action denoted by the given key on the first active text box of the
 * given view. Escape or enter will change the text boxes active status to
 * selected.
 *
 * If no text box is active then this function returns with an invalid argument
 * error. Check that a text box is active before calling, in the event that one
 * is active all other input handling should stop.
 */
oolong_error_t oolong_stack_view_active_text_box_register_key(oolong_stack_view_t* stack_view, oolong_key_t key);

/*
 * Prints the given stack view to the given file.
 */
oolong_error_t oolong_stack_view_print(oolong_stack_view_t* stack_view, file_t* file);

#endif // STACK_VIEW_H

