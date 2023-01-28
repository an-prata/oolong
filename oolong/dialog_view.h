/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_DIALOG_VIEW_H
#define OOLONG_DIALOG_VIEW_H

#include "styling.h"

enum oolong_dialog_view_button_state_e
{
	OOLONG_DIALOG_BUTTON_STATE_NORMAL,
	OOLONG_DIALOG_BUTTON_STATE_SELECTED,
	OOLONG_DIALOG_BUTTON_STATE_DISABLED
};

typedef int enum_t;  /* Serves only to clarify use as a generic enum type. */
typedef FILE file_t;
typedef enum oolong_dialog_view_button_state_e oolong_dialog_view_button_state_t;
typedef struct oolong_dialog_view_options_s oolong_dialog_view_options_t;
typedef struct oolong_dialog_view_button_s oolong_dialog_view_button_t;
typedef struct oolong_dialog_view_s oolong_dialog_view_t;

struct oolong_dialog_view_options_s
{
	size_t button_gap;
	size_t button_padding;
	size_t text_button_gap;
	size_t view_top_margin;
	size_t view_side_margin;
};

struct oolong_dialog_view_button_s
{
	wchar_t* text;
	oolong_style_set_t* style;
	oolong_style_set_t* style_selected;
	oolong_dialog_view_button_state_t state;
	enum_t identifier;
};

/*
 * Creates a new dialog view.
 */
oolong_dialog_view_t* oolong_dialog_view_create(oolong_dialog_view_options_t* options);

/*
 * Free all memory used by the given dialog.
 */
oolong_error_t oolong_dialog_view_destroy(oolong_dialog_view_t* dialog);

/*
 * Add a button to the dialog.
 */
oolong_error_t oolong_dialog_view_add_button(oolong_dialog_view_t* dialog, oolong_dialog_view_button_t* button);

/*
 * Set the dialog text.
 */
oolong_error_t oolong_dialog_view_set_text(oolong_dialog_view_t* dialog, wchar_t* text);

/*
 * Print the given dialog to the given file.
 */
oolong_error_t oolong_dialog_view_print(oolong_dialog_view_t* dialog, file_t* file);

/*
 * Select the next button to the current selection.
 */
oolong_error_t oolong_dialog_view_select_next(oolong_dialog_view_t* dialog);

/*
 * Select the previous button to the current selection.
 */
oolong_error_t oolong_dialog_view_select_previous(oolong_dialog_view_t* dialog);

/*
 * Gets the identifier of the selected button, return -1 on error or not found.
 */
enum_t oolong_dialog_get_selected_identifier(oolong_dialog_view_t* dialog);

#endif // OOLONG_DIALOG_VIEW_H

