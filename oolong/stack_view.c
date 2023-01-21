/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "stack_view.h"
#include "screen_buffer.h"

struct oolong_stack_view_s
{
    oolong_stack_view_options_t* options;
    oolong_stack_view_element_t** elements;
};

static oolong_error_t print_button(oolong_stack_view_button_data_t* button, oolong_stack_view_options_t* options, file_t* file)
{
    if (button->state == OOLONG_ELEMENT_STATE_SELECTED)
        fwprintf(file, L"%ls", button->style_selected);
    else
        fwprintf(file, L"%ls", button->style);

    size_t printed_characters = 0;
    
    for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
        fputwc(L' ', file);

    for (size_t start_index = printed_characters; button->text[printed_characters - start_index] != L'\0'; printed_characters++)
        fputwc(button->text[printed_characters - start_index], file);

    for (size_t start_index = printed_characters; printed_characters < options->element_width - options->element_padding; printed_characters++)
        fputwc(L' ', file);
    
    for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
        fputwc(L' ', file);

    fwprintf(file, L"%ls", OOLONG_STYLE_CLEAR_STRING);

    if (ferror(file))
        return OOLONG_ERROR_FAILED_IO_WRITE;

    return OOLONG_ERROR_NONE;
}

static oolong_error_t print_label(oolong_stack_view_label_data_t* label, oolong_stack_view_options_t* options, file_t* file)
{
    size_t printed_characters = 0;

    for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
        fputwc(L' ', file);

    fwprintf(file, L"%ls", label->style);
    fwprintf(file, L"%ls", label->text);

    for (size_t start_index = printed_characters; printed_characters < options->element_width - options->element_padding; printed_characters++)
        fputwc(L' ', file);
    
    for (printed_characters = 0; printed_characters < options->element_padding; printed_characters++)
        fputwc(L' ', file);

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
    }
}

static oolong_error_t print_centered_aligned(oolong_stack_view_t* stack_view, file_t* file)
{
    oolong_screen_buffer_dimensions_t screen_dimensions = oolong_get_screen_dimensions();

    if (screen_dimensions.columns == 0 && screen_dimensions.rows == 0)
        return OOLONG_ERROR_FAILED_IO_READ;

    size_t start_column = screen_dimensions.columns - (stack_view->options->view_side_margin + stack_view->options->element_width);
    start_column = (start_column / 2) - 1;
    oolong_error_t running_error = OOLONG_ERROR_NONE;
    
    for (size_t element = 0; stack_view->elements[element] != NULL; element++)
    {
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

    size_t start_column = screen_dimensions.columns - (stack_view->options->view_side_margin + stack_view->options->element_width);
    oolong_error_t running_error = OOLONG_ERROR_NONE;
    
    for (size_t element = 0; stack_view->elements[element] != NULL; element++)
    {
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
        if (stack_view->elements[index]->type == OOLONG_ELEMENT_TYPE_BUTTON)
            if (stack_view->elements[index]->data.button.state == OOLONG_ELEMENT_STATE_SELECTED)
                return stack_view->elements[index]->identifier;

    return -1;
}

void oolong_stack_view_select_next_element(oolong_stack_view_t* stack_view)
{
    if (stack_view == NULL)
        return;

    for (size_t index = 0; stack_view->elements[index] != NULL; index++)
    {
        if (stack_view->elements[index]->type != OOLONG_ELEMENT_TYPE_BUTTON)
            continue;

        if (stack_view->elements[index]->data.button.state != OOLONG_ELEMENT_STATE_SELECTED)
            continue;
        
        stack_view->elements[index]->data.button.state = OOLONG_ELEMENT_STATE_NORMAL;

        for (size_t next_selected = index + 1; next_selected != index; next_selected++)
        {
            if (stack_view->elements[next_selected] == NULL)
                next_selected = 0;
            
            if (stack_view->elements[next_selected]->type != OOLONG_ELEMENT_TYPE_BUTTON)
                continue;

            if (stack_view->elements[next_selected]->data.button.state == OOLONG_ELEMENT_STATE_DISABLED)
                continue;

            stack_view->elements[next_selected]->data.button.state = OOLONG_ELEMENT_STATE_SELECTED;
            return;
        }
    }
}

void oolong_stack_view_select_previous_element(oolong_stack_view_t* stack_view)
{
    if (stack_view == NULL)
        return;

    for (size_t index = 0; stack_view->elements[index] != NULL; index++)
    {
        if (stack_view->elements[index]->type != OOLONG_ELEMENT_TYPE_BUTTON)
            continue;

        if (stack_view->elements[index]->data.button.state != OOLONG_ELEMENT_STATE_SELECTED)
            continue;
        
        stack_view->elements[index]->data.button.state = OOLONG_ELEMENT_STATE_NORMAL;

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

            if (stack_view->elements[next_selected]->type != OOLONG_ELEMENT_TYPE_BUTTON)
                continue;

            if (stack_view->elements[next_selected]->data.button.state == OOLONG_ELEMENT_STATE_DISABLED)
                continue;

            stack_view->elements[next_selected]->data.button.state = OOLONG_ELEMENT_STATE_SELECTED;
            return;
        }
    }
}

oolong_error_t oolong_stack_view_print(oolong_stack_view_t* stack_view, file_t* file)
{
    if (stack_view == NULL || file == NULL)
        return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    oolong_terminal_clear();
    oolong_terminal_set_cursor_position(0, 0);

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
    }

    return oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);
}

