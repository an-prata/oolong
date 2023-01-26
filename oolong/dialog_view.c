/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <stdio.h>
#include "screen_buffer.h"
#include "dialog_view.h"

struct oolong_dialog_view_s
{
	oolong_dialog_view_options_t* options;
	oolong_dialog_view_button_t** buttons;
	wchar_t* text;
};

oolong_dialog_view_t* oolong_dialog_view_create(oolong_dialog_view_options_t* options)
{
	oolong_dialog_view_t* dialog = malloc(sizeof(oolong_dialog_view_t));

	if (dialog == NULL)
	{
		oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
		return NULL;
	}

	dialog->options = options;
	dialog->buttons = calloc(1, sizeof(oolong_dialog_view_button_t*));
	dialog->buttons[0] = NULL;
	return dialog;
}

oolong_error_t oolong_dialog_view_destroy(oolong_dialog_view_t* dialog)
{
	if (dialog == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	free(dialog->buttons);
	free(dialog);
	return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_dialog_view_add_button(oolong_dialog_view_t* dialog, oolong_dialog_view_button_t* button)
{
    if (dialog == NULL || button == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    size_t final_index = 0;
    for (; dialog->buttons[final_index] != NULL; final_index++);

    oolong_dialog_view_button_t** new_buttons = reallocarray(dialog->buttons, final_index + 2, sizeof(oolong_dialog_view_button_t*));

    if (new_buttons == NULL)
        return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);

    dialog->buttons = new_buttons;
    dialog->buttons[final_index] = button;
    dialog->buttons[final_index + 1] = NULL;
    return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_dialog_view_set_text(oolong_dialog_view_t* dialog, wchar_t* text)
{
	if (dialog == NULL || text == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
	
	dialog->text = text;
	return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_dialog_view_print(oolong_dialog_view_t* dialog, file_t* file)
{
	if (dialog == NULL || file == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	oolong_terminal_clear(file);
	oolong_terminal_set_cursor_position(0, 0, file);

	oolong_screen_buffer_dimensions_t dimensions = oolong_get_screen_dimensions();

	if (dimensions.columns == 0 && dimensions.rows == 0)
		return oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);

	size_t text_characters_per_line = dimensions.columns - (dialog->options->view_side_margin * 2);
	size_t text_characters_printed = 0;

	for (size_t i = 0; i < dialog->options->view_top_margin; i++)
		fputwc(L'\n', file);

	for (size_t i = 0; i < dialog->options->view_side_margin; i++)
		fputwc(L' ', file);

	for (size_t index = 0; dialog->text[index] != L'\0'; index++)
	{
		size_t columns_until_space = 0;

		while (dialog->text[columns_until_space + index] != L' ' && dialog->text[columns_until_space + index] != L'\0')
			columns_until_space += wcwidth(dialog->text[columns_until_space + index]);
		
		bool go_to_next_line = (text_characters_printed % text_characters_per_line) + columns_until_space > text_characters_per_line;
		
		/* For catching text without enough spaces to wrap each line. */
		go_to_next_line |= text_characters_printed != 0 && text_characters_printed % text_characters_per_line == 0;

		if (go_to_next_line)
		{
			text_characters_printed += text_characters_per_line - (text_characters_printed % text_characters_per_line);
			fputwc(L'\n', file);

			for (size_t i = 0; i < dialog->options->view_side_margin; i++)
				fputwc(L' ', file);
		}

		fputwc(dialog->text[index], file);
		text_characters_printed += wcwidth(dialog->text[index]);
	}

	/* Print buttons. */

	oolong_terminal_set_cursor_position(0, dimensions.rows - dialog->options->view_top_margin, file);

	size_t buttons_length = 0;
	for (; dialog->buttons[buttons_length] != NULL; buttons_length++);
	size_t button_size = dimensions.columns - (dialog->options->view_side_margin * 2);
	button_size /= buttons_length;

	for (size_t i = 0; i < dialog->options->view_side_margin; i++)
		fputwc(L' ', file);

	for (size_t button = 0; button < buttons_length; button++)
	{
		if (dialog->buttons[button]->state == OOLONG_DIALOG_BUTTON_STATE_SELECTED)
			fwprintf(file, dialog->buttons[button]->style_selected);
		else
			fwprintf(file, dialog->buttons[button]->style);

		size_t button_printed_characters = 0;

		for (; button_printed_characters < dialog->options->button_padding; button_printed_characters++)
			fputwc(L' ', file);

		for (size_t index = 0; dialog->buttons[button]->text[index] != L'\0'; index++)
		{
			fputwc(dialog->buttons[button]->text[index], file);
			button_printed_characters += wcwidth(dialog->buttons[button]->text[index]);
		}

		for (; button_printed_characters < button_size; button_printed_characters++)
			fputwc(L' ', file);
		
		fwprintf(file, OOLONG_STYLE_CLEAR_STRING);

		/*
		 * Dont print button gap on last button since it might go over onto the next
		 * line, which looks very ugly.
		 */
		
		if (button < buttons_length - 1)
			for (size_t i = 0; i < dialog->options->button_gap; i++)
				fputwc(L' ', file);
	}

	fflush(file);
	
	if (ferror(file))
		return oolong_error_record(OOLONG_ERROR_FAILED_IO_WRITE);

	return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_dialog_view_select_next(oolong_dialog_view_t* dialog)
{
	if (dialog == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	size_t index = 0;
	
	/* Find current selection. */

	for (; dialog->buttons[index] != NULL; index++)
	{
		if (dialog->buttons[index]->state == OOLONG_DIALOG_BUTTON_STATE_SELECTED)
		{
			dialog->buttons[index]->state = OOLONG_DIALOG_BUTTON_STATE_NORMAL;
			break;
		}
	}

	/* Select next button. */

	for (size_t next_selection = index + 1; next_selection != index; next_selection++)
	{
		if (dialog->buttons[next_selection] == NULL)
			next_selection = 0;

		if (dialog->buttons[next_selection]->state != OOLONG_DIALOG_BUTTON_STATE_DISABLED)
		{
			dialog->buttons[next_selection]->state = OOLONG_DIALOG_BUTTON_STATE_SELECTED;
			return OOLONG_ERROR_NONE;
		}
	}

	return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_dialog_view_select_previous(oolong_dialog_view_t* dialog)
{
	if (dialog == NULL)
		return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

	size_t index = 0;
	
	/* Find current selection. */

	for (; dialog->buttons[index] != NULL; index++)
	{
		if (dialog->buttons[index]->state == OOLONG_DIALOG_BUTTON_STATE_SELECTED)
		{
			dialog->buttons[index]->state = OOLONG_DIALOG_BUTTON_STATE_NORMAL;
			break;
		}
	}

	/* Select previous button. */

	for (size_t next_selection = index - 1; next_selection != index; next_selection--)
	{
		if (next_selection == SIZE_MAX)
			for (next_selection = 0; dialog->buttons[next_selection + 1] != NULL; next_selection++);

		if (dialog->buttons[next_selection]->state != OOLONG_DIALOG_BUTTON_STATE_DISABLED)
		{
			dialog->buttons[next_selection]->state = OOLONG_DIALOG_BUTTON_STATE_SELECTED;
			return OOLONG_ERROR_NONE;
		}
	}

	return OOLONG_ERROR_NONE;
}

enum_t oolong_dialog_get_selected_identifier(oolong_dialog_view_t* dialog)
{
    if (dialog == NULL)
        return -1;

    for (size_t index = 0; dialog->buttons[index] != NULL; index++)
    	if (dialog->buttons[index]->state == OOLONG_DIALOG_BUTTON_STATE_SELECTED)
			return dialog->buttons[index]->identifier;

    return -1;
}

