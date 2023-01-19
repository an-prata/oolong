/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "stack_view.h"

struct oolong_stack_view_s
{
    oolong_stack_view_options_t* options;
    oolong_stack_view_element_t** elements;
};

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

    size_t elements_length = 0;
    for (; stack_view->elements[elements_length] != NULL; elements_length++);

    oolong_stack_view_element_t** new_elements = reallocarray(stack_view->elements, elements_length + 1, sizeof(oolong_stack_view_element_t*));

    if (new_elements == NULL)
        return oolong_error_record(OOLONG_ERROR_NOT_ENOUGH_MEMORY);

    stack_view->elements = new_elements;
    stack_view->elements[elements_length] = element;
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
        if (stack_view->elements[index] != OOLONG_ELEMENT_TYPE_BUTTON)
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
        if (stack_view->elements[index] != OOLONG_ELEMENT_TYPE_BUTTON)
            continue;

        if (stack_view->elements[index]->data.button.state != OOLONG_ELEMENT_STATE_SELECTED)
            continue;
        
        stack_view->elements[index]->data.button.state = OOLONG_ELEMENT_STATE_NORMAL;

        for (size_t next_selected = index - 1; next_selected != index; next_selected--)
        {
            if (stack_view->elements[next_selected] == 0)
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

