#include <stdbool.h>
#include "../oolong/oolong.h"

int main()
{
    oolong_set_locale();

    oolong_stack_view_options_t options =
    {
        .element_gap = 1,
        .element_padding = 1,
        .element_width = 20,
        .view_side_margin = 2,
        .view_top_margin = 2,
        .align = OOLONG_ALIGN_LEFT
    };
    
    oolong_stack_view_t* view = oolong_stack_view_create(&options);

    oolong_stack_view_element_t title = 
    {
        .data.label =
        {
            .style = oolong_style_set_create(),
            .text = L"TITLE"
        },
        .type = OOLONG_ELEMENT_TYPE_LABEL,
        .identifier = 0
    };

    oolong_style_set_add(&title.data.label.style, OOLONG_STYLE_BOLD);

    oolong_stack_view_element_t button0 =
    {
        .data.button = 
        {
            .state = OOLONG_ELEMENT_STATE_NORMAL,
            .text = L"green",
            .style = oolong_style_set_create(),
            .style_selected = oolong_style_set_create()
        },
        .type = OOLONG_ELEMENT_TYPE_BUTTON,
        .identifier = 1
    };

    oolong_stack_view_element_t button1 =
    {
        .data.button = 
        {
            .state = OOLONG_ELEMENT_STATE_SELECTED,
            .text = L"yellow",
            .style = oolong_style_set_create(),
            .style_selected = oolong_style_set_create()
        },
        .type = OOLONG_ELEMENT_TYPE_BUTTON,
        .identifier = 2
    };

    oolong_stack_view_element_t button2 =
    {
        .data.button = 
        {
            .state = OOLONG_ELEMENT_STATE_NORMAL,
            .text = L"red",
            .style = oolong_style_set_create(),
            .style_selected = oolong_style_set_create()
        },
        .type = OOLONG_ELEMENT_TYPE_BUTTON,
        .identifier = 3
    };

    oolong_style_set_add(&button0.data.button.style_selected, OOLONG_STYLE_BACKGROUND_CYAN);
    oolong_style_set_add(&button1.data.button.style_selected, OOLONG_STYLE_BACKGROUND_CYAN);
    oolong_style_set_add(&button2.data.button.style_selected, OOLONG_STYLE_BACKGROUND_CYAN);

    oolong_stack_view_add_element(view, &title);
    oolong_stack_view_add_element(view, &button0);
    oolong_stack_view_add_element(view, &button1);
    oolong_stack_view_add_element(view, &button2);

    oolong_terminal_enter_alternate_screen();
    oolong_disable_canonical_input();
    oolong_terminal_hide_cursor();

    while (true)
    {
        oolong_stack_view_print(view, stdout);

        oolong_key_t key = oolong_keyboard_get_key();
        
        if (key == KEY_LOWERCASE_J || key == KEY_J || key == KEY_DOWN)
            oolong_stack_view_select_next_element(view);
        if (key == KEY_LOWERCASE_K || key == KEY_K || key == KEY_UP)
            oolong_stack_view_select_previous_element(view);
        if (key == KEY_LOWERCASE_Q || key == KEY_Q)
        {
            oolong_terminal_exit_alternate_screen();
            oolong_restore_canonical_input();
            oolong_terminal_show_cursor();
            exit(0);
        }

        if (key == KEY_RETURN)
        {
            switch (oolong_stack_view_get_selected_identifier(view))
            {
                case (1):
                    oolong_style_set_destroy(button0.data.button.style_selected);
                    oolong_style_set_destroy(button1.data.button.style_selected);
                    oolong_style_set_destroy(button2.data.button.style_selected);
                    button0.data.button.style_selected = oolong_style_set_create();
                    button1.data.button.style_selected = oolong_style_set_create();
                    button2.data.button.style_selected = oolong_style_set_create();
                    oolong_style_set_add(&button0.data.button.style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
                    oolong_style_set_add(&button1.data.button.style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
                    oolong_style_set_add(&button2.data.button.style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
                    break;
                case (2):
                    oolong_style_set_destroy(button0.data.button.style_selected);
                    oolong_style_set_destroy(button1.data.button.style_selected);
                    oolong_style_set_destroy(button2.data.button.style_selected);
                    button0.data.button.style_selected = oolong_style_set_create();
                    button1.data.button.style_selected = oolong_style_set_create();
                    button2.data.button.style_selected = oolong_style_set_create();
                    oolong_style_set_add(&button0.data.button.style_selected, OOLONG_STYLE_BACKGROUND_YELLOW);
                    oolong_style_set_add(&button1.data.button.style_selected, OOLONG_STYLE_BACKGROUND_YELLOW);
                    oolong_style_set_add(&button2.data.button.style_selected, OOLONG_STYLE_BACKGROUND_YELLOW);
                    break;
                case (3):
                    oolong_style_set_destroy(button0.data.button.style_selected);
                    oolong_style_set_destroy(button1.data.button.style_selected);
                    oolong_style_set_destroy(button2.data.button.style_selected);
                    button0.data.button.style_selected = oolong_style_set_create();
                    button1.data.button.style_selected = oolong_style_set_create();
                    button2.data.button.style_selected = oolong_style_set_create();
                    oolong_style_set_add(&button0.data.button.style_selected, OOLONG_STYLE_BACKGROUND_RED);
                    oolong_style_set_add(&button1.data.button.style_selected, OOLONG_STYLE_BACKGROUND_RED);
                    oolong_style_set_add(&button2.data.button.style_selected, OOLONG_STYLE_BACKGROUND_RED);
                    break;
            }
        }
    }
}

