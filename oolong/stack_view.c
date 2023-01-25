/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include <stdlib.h>
#include "screen_buffer.h"
#include "stack_view.h"

struct oolong_stack_view_s
{
    oolong_stack_view_options_t* options;
    oolong_stack_view_element_t** elements;
};

static oolong_error_t print_text_box(oolong_stack_view_text_box_data_t* text_box, oolong_stack_view_options_t* options, file_t* file)
{
    if (text_box->entered_text == NULL)
    {
        text_box->entered_text = calloc(1, sizeof(wchar_t));
        text_box->entered_text[0] = L'\0';    
    }
    
    if (text_box->state == OOLONG_ELEMENT_STATE_SELECTED && text_box->entered_text[0] != L'\0')
        fwprintf(file, L"%ls", text_box->entered_style_selected);
    else if (text_box->state == OOLONG_ELEMENT_STATE_SELECTED)
        fwprintf(file, L"%ls", text_box->display_style_selected);
    else if (text_box->state == OOLONG_ELEMENT_STATE_ACTIVE)
        fwprintf(file, L"%ls", text_box->entered_style_selected);
    else if (text_box->entered_text[0] != L'\0')
        fwprintf(file, L"%ls", text_box->entered_style);
    else
        fwprintf(file, L"%ls", text_box->display_style);
    
    size_t printed_characters = 0;
    
    for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
        fputwc(L' ', file);

    if (text_box->entered_text[0] != L'\0' || text_box->state == OOLONG_ELEMENT_STATE_ACTIVE)
    {
        for (size_t index = 0; text_box->entered_text[index] != L'\0'; index++)
        {
            fputwc(text_box->entered_text[index], file);
            printed_characters += wcwidth(text_box->entered_text[index]);
        }
    }
    else
    {
        for (size_t index = 0; text_box->display_text[index] != L'\0'; index++)
        {
            fputwc(text_box->display_text[index], file);
            printed_characters += wcwidth(text_box->display_text[index]);
        }
    }

    if (options->align != OOLONG_ALIGN_WIDTH)
    {
        for (size_t start_index = printed_characters; printed_characters < options->element_width - options->element_padding; printed_characters++)
            fputwc(L' ', file);

        for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
            fputwc(L' ', file);
    }
    else
    {
        oolong_screen_buffer_dimensions_t screen_dimensions = oolong_get_screen_dimensions();

        if (screen_dimensions.columns == 0 && screen_dimensions.rows == 0)
            return OOLONG_ERROR_FAILED_IO_READ;

        for (size_t start_index = printed_characters; printed_characters < screen_dimensions.columns - (options->view_side_margin * 2); printed_characters++)
            fputwc(L' ', file);
    }
    
    fwprintf(file, L"%ls", OOLONG_STYLE_CLEAR_STRING);

    if (ferror(file))
        return OOLONG_ERROR_FAILED_IO_WRITE;

    return OOLONG_ERROR_NONE;
}

static oolong_error_t print_button(oolong_stack_view_button_data_t* button, oolong_stack_view_options_t* options, file_t* file)
{
    if (button->state == OOLONG_ELEMENT_STATE_SELECTED)
        fwprintf(file, L"%ls", button->style_selected);
    else
        fwprintf(file, L"%ls", button->style);

    size_t printed_characters = 0;
    
    for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
        fputwc(L' ', file);

    for (size_t index = 0; button->text[index] != L'\0'; index++)
    {
        fputwc(button->text[index], file);
        printed_characters += wcwidth(button->text[index]);
    }

    if (options->align != OOLONG_ALIGN_WIDTH)
    {
        for (size_t start_index = printed_characters; printed_characters < options->element_width - options->element_padding; printed_characters++)
            fputwc(L' ', file);

        for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
            fputwc(L' ', file);
    }
    else
    {
        oolong_screen_buffer_dimensions_t screen_dimensions = oolong_get_screen_dimensions();

        if (screen_dimensions.columns == 0 && screen_dimensions.rows == 0)
            return OOLONG_ERROR_FAILED_IO_READ;

        for (size_t start_index = printed_characters; printed_characters < screen_dimensions.columns - (options->view_side_margin * 2); printed_characters++)
            fputwc(L' ', file);
    }

    fwprintf(file, L"%ls", OOLONG_STYLE_CLEAR_STRING);

    if (ferror(file))
        return OOLONG_ERROR_FAILED_IO_WRITE;

    return OOLONG_ERROR_NONE;
}

static oolong_error_t print_label(oolong_stack_view_label_data_t* label, oolong_stack_view_options_t* options, file_t* file)
{
    size_t printed_characters = 0;

    fwprintf(file, L"%ls", label->style);

    for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
        fputwc(L' ', file);

    for (size_t index = 0; label->text[index] != L'\0'; index++)
    {
        fputwc(label->text[index], file);
        printed_characters += wcwidth(label->text[index]);
    }

    if (options->align != OOLONG_ALIGN_WIDTH)
    {
        for (size_t start_index = printed_characters; printed_characters < options->element_width - options->element_padding; printed_characters++)
            fputwc(L' ', file);

        for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
            fputwc(L' ', file);
    }
    else
    {
        oolong_screen_buffer_dimensions_t screen_dimensions = oolong_get_screen_dimensions();

        if (screen_dimensions.columns == 0 && screen_dimensions.rows == 0)
            return OOLONG_ERROR_FAILED_IO_READ;

        for (size_t start_index = printed_characters; printed_characters < screen_dimensions.columns - (options->view_side_margin * 2); printed_characters++)
            fputwc(L' ', file);
    }

    fwprintf(file, L"%ls", OOLONG_STYLE_CLEAR_STRING);

    if (ferror(file))
        return OOLONG_ERROR_FAILED_IO_WRITE;

    return OOLONG_ERROR_NONE;
}

static oolong_error_t print_element(oolong_stack_view_element_t* element, oolong_stack_view_options_t* options, file_t* file)
{
    switch (element->type)
    {
        case (OOLONG_ELEMENT_TYPE_BUTTON):
            return print_button(&element->data.button, options, file);
        case (OOLONG_ELEMENT_TYPE_LABEL):
            return print_label(&element->data.label, options, file);
        case (OOLONG_ELEMENT_TYPE_TEXT_BOX):
            return print_text_box(&element->data.text_box, options, file);
    }
}

static oolong_error_t print_centered_aligned(oolong_stack_view_t* stack_view, file_t* file)
{
    oolong_screen_buffer_dimensions_t screen_dimensions = oolong_get_screen_dimensions();

    if (screen_dimensions.columns == 0 && screen_dimensions.rows == 0)
        return OOLONG_ERROR_FAILED_IO_READ;

    oolong_error_t running_error = OOLONG_ERROR_NONE;
    
    for (size_t element = 0; stack_view->elements[element] != NULL; element++)
    {
        size_t start_column = 0;
        size_t default_start = screen_dimensions.columns - (stack_view->options->element_width + stack_view->options->element_padding) - 1;
        default_start /= 2;

        if (stack_view->elements[element]->type == OOLONG_ELEMENT_TYPE_BUTTON)
        {
            start_column = screen_dimensions.columns - wcslen(stack_view->elements[element]->data.button.text) - 1;
            start_column /= 2;

            if (start_column > default_start)
                start_column = default_start;
        }
        else if (stack_view->elements[element]->type == OOLONG_ELEMENT_TYPE_LABEL)
        {
            start_column = screen_dimensions.columns - wcslen(stack_view->elements[element]->data.button.text) - 1;
            start_column /= 2;
        }
        else if (stack_view->elements[element]->type == OOLONG_ELEMENT_TYPE_TEXT_BOX)
        {
            if (stack_view->elements[element]->data.text_box.entered_text == NULL)
                start_column = wcslen(stack_view->elements[element]->data.text_box.display_text);
            else if (stack_view->elements[element]->data.text_box.entered_text[0] != L'\0')
                start_column = wcslen(stack_view->elements[element]->data.text_box.entered_text);
            else if (stack_view->elements[element]->data.text_box.state == OOLONG_ELEMENT_STATE_ACTIVE)
                start_column = wcslen(stack_view->elements[element]->data.text_box.entered_text);
            else
                start_column = wcslen(stack_view->elements[element]->data.text_box.display_text);

            start_column = screen_dimensions.columns - start_column - 1;
            start_column /= 2;

            if (start_column > default_start)
                start_column = default_start;
        }
        
        for (size_t i = 0; i < start_column; i++)
            fputwc(L' ', file);

        running_error |= print_element(stack_view->elements[element], stack_view->options, file);
        fputwc(L'\n', file);

        for (size_t i = 0; i < stack_view->options->element_gap; i++)
            fputwc(L'\n', file);
    }

    fflush(file);
    return running_error;
}

static oolong_error_t print_left_aligned(oolong_stack_view_t* stack_view, file_t* file)
{
    oolong_error_t running_error = OOLONG_ERROR_NONE;
    
    for (size_t element = 0; stack_view->elements[element] != NULL; element++)
    {
        for (size_t i = 0; i < stack_view->options->view_side_margin; i++)
            fputwc(L' ', file);

        running_error |= print_element(stack_view->elements[element], stack_view->options, file);
        fputwc(L'\n', file);

        for (size_t i = 0; i < stack_view->options->element_gap; i++)
            fputwc(L'\n', file);
    }

    fflush(file);
    return running_error;
}

static oolong_error_t print_right_aligned(oolong_stack_view_t* stack_view, file_t* file)
{
    oolong_screen_buffer_dimensions_t screen_dimensions = oolong_get_screen_dimensions();

    if (screen_dimensions.columns == 0 && screen_dimensions.rows == 0)
        return OOLONG_ERROR_FAILED_IO_READ;

    oolong_error_t running_error = OOLONG_ERROR_NONE;
    
    for (size_t element = 0; stack_view->elements[element] != NULL; element++)
    {
        size_t start_column = 0;
        size_t default_start = screen_dimensions.columns - (stack_view->options->element_width + stack_view->options->element_padding) - 1;

        if (stack_view->elements[element]->type == OOLONG_ELEMENT_TYPE_BUTTON)
        {
            start_column = screen_dimensions.columns - wcslen(stack_view->elements[element]->data.button.text) - 1;
            start_column -= stack_view->options->view_side_margin + stack_view->options->element_padding;

            if (start_column > default_start)
                start_column = default_start;
        }
        else if (stack_view->elements[element]->type == OOLONG_ELEMENT_TYPE_LABEL)
        {
            start_column = screen_dimensions.columns - wcslen(stack_view->elements[element]->data.button.text) - 1;
            start_column -= stack_view->options->view_side_margin + stack_view->options->element_padding;
        }
        else if (stack_view->elements[element]->type == OOLONG_ELEMENT_TYPE_TEXT_BOX)
        {
            if (stack_view->elements[element]->data.text_box.entered_text == NULL)
                start_column = wcslen(stack_view->elements[element]->data.text_box.display_text);
            else if (stack_view->elements[element]->data.text_box.entered_text[0] != L'\0')
                start_column = wcslen(stack_view->elements[element]->data.text_box.entered_text);
            else if (stack_view->elements[element]->data.text_box.state == OOLONG_ELEMENT_STATE_ACTIVE)
                start_column = wcslen(stack_view->elements[element]->data.text_box.entered_text);
            else
                start_column = wcslen(stack_view->elements[element]->data.text_box.display_text);

            start_column = screen_dimensions.columns - start_column - 1;
            start_column -= stack_view->options->view_side_margin + stack_view->options->element_padding;

            if (start_column > default_start)
                start_column = default_start;
        }

        for (size_t i = 0; i < start_column; i++)
            fputwc(L' ', file);

        running_error |= print_element(stack_view->elements[element], stack_view->options, file);
        fputwc(L'\n', file);

        for (size_t i = 0; i < stack_view->options->element_gap; i++)
            fputwc(L'\n', file);
    }

    fflush(file);
    return running_error;
}

oolong_stack_view_t* oolong_stack_view_create(oolong_stack_view_options_t* options)
{
    oolong_stack_view_t* stack_view = malloc(sizeof(oolong_stack_view_t));

    if (stack_view == NULL)
    {
        oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);
        return NULL;
    }

    stack_view->options = options;
    stack_view->elements = calloc(1, sizeof(oolong_stack_view_element_t*));
    stack_view->elements[0] = NULL;

    return stack_view;
}

void oolong_stack_view_destroy(oolong_stack_view_t* stack_view)
{
    if (stack_view == NULL)
        return;

    free(stack_view->elements);
    free(stack_view);
}

oolong_error_t oolong_stack_view_add_element(oolong_stack_view_t* stack_view, oolong_stack_view_element_t* element)
{
    if (stack_view == NULL || element == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    size_t final_index = 0;
    for (; stack_view->elements[final_index] != NULL; final_index++);

    oolong_stack_view_element_t** new_elements = reallocarray(stack_view->elements, final_index + 2, sizeof(oolong_stack_view_element_t*));

    if (new_elements == NULL)
        return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);

    stack_view->elements = new_elements;
    stack_view->elements[final_index] = element;
    stack_view->elements[final_index + 1] = NULL;
    return OOLONG_ERROR_NONE;
}

enum_t oolong_stack_view_get_selected_identifier(oolong_stack_view_t* stack_view)
{
    if (stack_view == NULL)
        return -1;

    for (size_t index = 0; stack_view->elements[index] != NULL; index++)
    {
        if (stack_view->elements[index]->type == OOLONG_ELEMENT_TYPE_BUTTON)
            if (stack_view->elements[index]->data.button.state == OOLONG_ELEMENT_STATE_SELECTED)
                return stack_view->elements[index]->identifier;
        if (stack_view->elements[index]->type == OOLONG_ELEMENT_TYPE_TEXT_BOX)
            if (stack_view->elements[index]->data.text_box.state == OOLONG_ELEMENT_STATE_SELECTED || stack_view->elements[index]->data.text_box.state == OOLONG_ELEMENT_STATE_ACTIVE)
                return stack_view->elements[index]->identifier;
    }

    return -1;
}

oolong_error_t oolong_stack_view_select_next_element(oolong_stack_view_t* stack_view)
{
    if (stack_view == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    size_t index = 0;

    /* Find current selection. */

    for (; stack_view->elements[index] != NULL; index++)
    {
        if (stack_view->elements[index]->type == OOLONG_ELEMENT_TYPE_BUTTON)
        {
            if (stack_view->elements[index]->data.button.state != OOLONG_ELEMENT_STATE_SELECTED)
                continue;
        
            stack_view->elements[index]->data.button.state = OOLONG_ELEMENT_STATE_NORMAL;
            break;
        }

        if (stack_view->elements[index]->type == OOLONG_ELEMENT_TYPE_TEXT_BOX)
        {
            if (stack_view->elements[index]->data.text_box.state != OOLONG_ELEMENT_STATE_SELECTED && stack_view->elements[index]->data.text_box.state != OOLONG_ELEMENT_STATE_ACTIVE)
                continue;
        
            stack_view->elements[index]->data.text_box.state = OOLONG_ELEMENT_STATE_NORMAL;
            break;
        }
    }

    /* Select the next element. */

    for (size_t next_selected = index + 1; next_selected != index; next_selected++)
    {
        if (stack_view->elements[next_selected] == NULL)
            next_selected = 0;
    
        if (stack_view->elements[next_selected]->type == OOLONG_ELEMENT_TYPE_BUTTON)
        {
            if (stack_view->elements[next_selected]->data.button.state == OOLONG_ELEMENT_STATE_DISABLED)
                continue;

            stack_view->elements[next_selected]->data.button.state = OOLONG_ELEMENT_STATE_SELECTED;
            return OOLONG_ERROR_NONE;
        }

        if (stack_view->elements[next_selected]->type == OOLONG_ELEMENT_TYPE_TEXT_BOX)
        {
            if (stack_view->elements[next_selected]->data.text_box.state == OOLONG_ELEMENT_STATE_DISABLED)
                continue;

            stack_view->elements[next_selected]->data.text_box.state = OOLONG_ELEMENT_STATE_SELECTED;
            return OOLONG_ERROR_NONE;
        }
    }

    return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_stack_view_select_previous_element(oolong_stack_view_t* stack_view)
{
    if (stack_view == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    size_t index = 0;

    /* Find current selection. */

    for (; stack_view->elements[index] != NULL; index++)
    {
        if (stack_view->elements[index]->type == OOLONG_ELEMENT_TYPE_BUTTON)
        {
            if (stack_view->elements[index]->data.button.state != OOLONG_ELEMENT_STATE_SELECTED)
                continue;
        
            stack_view->elements[index]->data.button.state = OOLONG_ELEMENT_STATE_NORMAL;
            break;
        }

        if (stack_view->elements[index]->type == OOLONG_ELEMENT_TYPE_TEXT_BOX)
        {
            if (stack_view->elements[index]->data.text_box.state != OOLONG_ELEMENT_STATE_SELECTED && stack_view->elements[index]->data.text_box.state != OOLONG_ELEMENT_STATE_ACTIVE)
                continue;
        
            stack_view->elements[index]->data.text_box.state = OOLONG_ELEMENT_STATE_NORMAL;
            break;
        }
    }

    /* Select previous element. */
    
    for (size_t next_selected = index - 1; next_selected != index; next_selected--)
    {
        /*
         * Lucky for me unsigned underflow is very well defined behavior,
         * so (size_t)(0 - 1) is always SIZE_MAX, and frankly giving up the
         * one element to make this possible is fine with me, since any UI
         * with that many buttons is shit anyways.
         */

        if (next_selected == SIZE_MAX)
            for (; stack_view->elements[next_selected + 1] != NULL; next_selected++);

        if (stack_view->elements[next_selected]->type == OOLONG_ELEMENT_TYPE_BUTTON)
        {
            if (stack_view->elements[next_selected]->data.button.state == OOLONG_ELEMENT_STATE_DISABLED)
                continue;

            stack_view->elements[next_selected]->data.button.state = OOLONG_ELEMENT_STATE_SELECTED;
            return OOLONG_ERROR_NONE;
        }

        if (stack_view->elements[next_selected]->type == OOLONG_ELEMENT_TYPE_TEXT_BOX)
        {
            if (stack_view->elements[next_selected]->data.text_box.state == OOLONG_ELEMENT_STATE_DISABLED)
                continue;

            stack_view->elements[next_selected]->data.text_box.state = OOLONG_ELEMENT_STATE_SELECTED;
            return OOLONG_ERROR_NONE;
        }
    }

    return OOLONG_ERROR_NONE;
}

bool oolong_stack_view_get_is_text_box_active(oolong_stack_view_t* stack_view)
{
    for (size_t element = 0; stack_view->elements[element] != NULL; element++)
        if (stack_view->elements[element]->type == OOLONG_ELEMENT_TYPE_TEXT_BOX)
            if (stack_view->elements[element]->data.text_box.state == OOLONG_ELEMENT_STATE_ACTIVE)
                return true;

    return false;
}

oolong_error_t oolong_stack_view_active_text_box_register_key(oolong_stack_view_t* stack_view, oolong_key_t key)
{
    if (stack_view == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
    
    oolong_stack_view_text_box_data_t* text_box;
    
    for (size_t element = 0; stack_view->elements[element] != NULL; element++)
        if (stack_view->elements[element]->type == OOLONG_ELEMENT_TYPE_TEXT_BOX)
            if (stack_view->elements[element]->data.text_box.state == OOLONG_ELEMENT_STATE_ACTIVE)
                text_box = &stack_view->elements[element]->data.text_box;

    if (key == KEY_ESCAPE || key == KEY_RETURN)
    {
        text_box->state = OOLONG_ELEMENT_STATE_SELECTED;
        return OOLONG_ERROR_NONE;
    }

    if (text_box->entered_text == NULL)
    {
        text_box->entered_text = calloc(1, sizeof(wchar_t));
        text_box->entered_text[0] = L'\0';
    }

    if (key == KEY_BACKSPACE)
    {
        if (text_box->entered_text[0] == L'\0')
            return OOLONG_ERROR_NONE;

        size_t length = 0;
        for (; text_box->entered_text[length] != L'\0'; length++);
        text_box->entered_text[length - 1] = L'\0';
        return OOLONG_ERROR_NONE;
    }

    size_t length = 0;
    for (; text_box->entered_text[length] != L'\0'; length++);

    if (32 > key || 126 < key)
        return OOLONG_ERROR_NONE;
    
    wchar_t* new_text = reallocarray(text_box->entered_text, length + 2, sizeof(wchar_t));
    length++;

    if (new_text == NULL)
        return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);

    text_box->entered_text = new_text;
    text_box->entered_text[length - 1] = key;
    text_box->entered_text[length] = L'\0';
    return OOLONG_ERROR_NONE;
}

oolong_error_t oolong_stack_view_print(oolong_stack_view_t* stack_view, file_t* file)
{
    if (stack_view == NULL || file == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    oolong_terminal_clear(file);
    oolong_terminal_set_cursor_position(0, 0, file);

    for (size_t i = 0; i < stack_view->options->view_top_margin; i++)
        fputwc(L'\n', file);

    switch (stack_view->options->align)
    {
        case (OOLONG_ALIGN_CENTER):
            return oolong_error_record(print_centered_aligned(stack_view, file));

        case (OOLONG_ALIGN_LEFT):
            return oolong_error_record(print_left_aligned(stack_view, file));

        case (OOLONG_ALIGN_RIGHT):
            return oolong_error_record(print_right_aligned(stack_view, file));

        case (OOLONG_ALIGN_WIDTH):
            return oolong_error_record(print_left_aligned(stack_view, file));
    }

    return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
}

