#include <stdbool.h>
#include "../oolong/oolong.h"

int main()
{
    oolong_set_locale();

    /*
     * Here we set up the options for the whole view, some of these may be
     * ignored in some cases, such as the side margins when using center align.
     */

    oolong_stack_view_options_t options =
    {
        .element_gap = 1,
        .element_padding = 1,
        .element_width = 20,
        .view_side_margin = 2,
        .view_top_margin = 2,
        .align = OOLONG_ALIGN_WIDTH
    };
    
    /*
     * Stack view options are then used to create an empty stack view.
     */

    oolong_stack_view_t* view = oolong_stack_view_create(&options);

    /*
     * Lets create a label to server as a title.
     */

    oolong_stack_view_element_t title = 
    {
        .data.label =
        {
            /* We create an empty style set to be configured later. */
            .style = oolong_style_set_create(),

            /* Oolong uses wide characters, all strings should reflect this. */
            .text = L"TITLE"
        },

        /* Set type to reflect the data we gave out label. */
        .type = OOLONG_ELEMENT_TYPE_LABEL,

        /*
         * identifier, and the enum_t typedef that oolong makes, are meant to
         * store enum values but in the end its all just integers. For this
         * example we just use a basic int.
         *
         * Oolong will never touch the indentifier, it is only used to return
         * back to your program so that you can determine what to do with it.
         */

        .identifier = 0
    };

    /*
     * Now we can configure the style set we initialized, since its supposed to
     * be a title lets make it pop with some bolding and italics.
     */

    oolong_style_set_add(&title.data.label.style, OOLONG_STYLE_BOLD);
    oolong_style_set_add(&title.data.label.style, OOLONG_STYLE_ITALIC);
	oolong_style_set_add(&title.data.label.style, OOLONG_STYLE_BACKGROUND_RED);

    /*
     * Now lets make some buttons, these buttons will change all of the view's
     * buttons' styling to a different color, lets start with green.
     */
    oolong_stack_view_element_t button0 =
    {
        .data.button = 
        {
            /*
             * state is used by oolong both for determining how to render the
             * element as well as for communicating with your program. Only one
             * element should ever be selected or active if you plan on using
             * oolong's element handling functions. If you plan on handling
             * all the elements yourself then oolong wont complain about more
             * than one selection.
             *
             * Since we want to use oolong to handle the element selecting
             * logic lets choose this element to be the first selected.
             */
        
            .state = OOLONG_ELEMENT_STATE_NORMAL,

            /* This button will change the selection highlight to green. */
            .text = L"green",

            /* Initialize out styles. */
            .style = oolong_style_set_create(),
            .style_selected = oolong_style_set_create()
        },

        .type = OOLONG_ELEMENT_TYPE_BUTTON,
        .identifier = 1
    };

    /*
     * Lets keep going with two more buttons for yellow and red.
     */

    oolong_stack_view_element_t button1 =
    {
        .data.button = 
        {
            .state = OOLONG_ELEMENT_STATE_NORMAL,
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

    /*
     * For the sake of demoing the library lets add a text box too.
     */

    oolong_stack_view_element_t text_box =
    {
        .data.text_box =
        {
            .state = OOLONG_ELEMENT_STATE_SELECTED,

            /*
             * Display text is displayed if the user has not entered text yet.
             * The only exception is if the text box is active, in which case
             * entered text is always displayed, if its NULL then an empty
             * string will be initialized to the value, for this reason its
             * best to just initialize to NULL, since if the user tries to
             * type into a box that had its entered text initialized to a
             * string literal the program will itmediatly segfault.
             *
             * One thing to note is that you may allocate memory to the entered
             * text member, just remember to NULL terminate the string.
             */

            .display_text = L"enter text!!",
            .entered_text = NULL,

            /*
             * We have a few more styles in a text box that a button or label.
             * Selected styles are simple enough, but the display and entered
             * text styles are used when their similarly named strings are used.
             */

            .display_style = oolong_style_set_create(),
            .entered_style = oolong_style_set_create(),
            .display_style_selected = oolong_style_set_create(),
            .entered_style_selected = oolong_style_set_create()
        },

        .type = OOLONG_ELEMENT_TYPE_TEXT_BOX,
        .identifier = 4
    };

    /*
     * Lets make all out selections start on green.
     */
    
    oolong_style_set_add(&button0.data.button.style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
    oolong_style_set_add(&button1.data.button.style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
    oolong_style_set_add(&button2.data.button.style_selected, OOLONG_STYLE_BACKGROUND_GREEN);

    oolong_style_set_add(&text_box.data.text_box.display_style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
    oolong_style_set_add(&text_box.data.text_box.entered_style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
    
    /*
     * Now we can add all of our elements to the stack view. You may absolutely
     * continue to access and write to the original elements.
     */

    oolong_stack_view_add_element(view, &title);
    oolong_stack_view_add_element(view, &button0);
    oolong_stack_view_add_element(view, &button1);
    oolong_stack_view_add_element(view, &button2);
    oolong_stack_view_add_element(view, &text_box);

    /* 
     * Alternate screen, keeps the terminal from being cluttered with each
     * application frame that oolong prints.
     */

    oolong_terminal_enter_alternate_screen();

    /*
     * Disable canonical input, oolong's keyboard function only work on 
     * non-canonical input. In breif, this makes it so functions like getc()
     * return as soon as a key is pressed instead of waiting for an enter.
     */
    
    oolong_disable_canonical_input();

    /*
     * Hide the cursor, simple enough.
     */

    oolong_terminal_hide_cursor();

    /*
     * Now that we've initialized our view, lets render it in a loop, in this
     * example we print after every registered keystroke, though you could
     * print multiple times between input if your trying to animate, not that
     * oolong is built for that.
     */

    while (true)
    {
        /* You dont have to use stdout, not sure why you wouldn't though. */
        oolong_stack_view_print(view, stdout);

        /* Get a key, this blocks the thread until a key is registered. */
        oolong_key_t key = oolong_keyboard_get_key();

        /*
         * Since we dont want to register things like selection changes or
         * quits while in a text box we check if one is active, that means that
         * the user is entering text, and if so commit all input handling to
         * that text box.
         */

        if (oolong_stack_view_get_is_text_box_active(view))
        {
            /*
             * This function will register any typable characters to the text
             * box and if eitehr enter or escape is received it will deactivate
             * the text box.
             */
            
            oolong_stack_view_active_text_box_register_key(view, key);
            continue;
        }
        
        /*
         * Change selection with J/K or the arrow keys.
         */

        if (key == KEY_LOWERCASE_J || key == KEY_J || key == KEY_DOWN)
            oolong_stack_view_select_next_element(view);
        if (key == KEY_LOWERCASE_K || key == KEY_K || key == KEY_UP)
            oolong_stack_view_select_previous_element(view);

        /*
         * Quit on Q.
         */

        if (key == KEY_LOWERCASE_Q || key == KEY_Q)
        {
            /*
             * All we do is the inverse of what we did right before enetring the
             * loop to clean up after ourselves.
             */

            oolong_terminal_exit_alternate_screen();
            oolong_restore_canonical_input();
            oolong_terminal_show_cursor();
            exit(0);
        }

        /*
         * On enter we need to register the action corralated with the current
         * selection. There are more elegant ways to do this, but in the
         * interest of clarity we will do things the tedious way.
         */
        
        if (key == KEY_RETURN)
        {
            /*
             * Heres where oolong gives us our identifiers back.
             */
            
            switch (oolong_stack_view_get_selected_identifier(view))
            {
                /*
                 * Cases 1 - 3 will just change selection color.
                 */
                case (1):
                    /* Destroy previous styles. */
                    oolong_style_set_destroy(button0.data.button.style_selected);
                    oolong_style_set_destroy(button1.data.button.style_selected);
                    oolong_style_set_destroy(button2.data.button.style_selected);
                    oolong_style_set_destroy(text_box.data.text_box.display_style_selected);
                    oolong_style_set_destroy(text_box.data.text_box.entered_style_selected);

                    /* Create new ones. */
                    button0.data.button.style_selected = oolong_style_set_create();
                    button1.data.button.style_selected = oolong_style_set_create();
                    button2.data.button.style_selected = oolong_style_set_create();
                    text_box.data.text_box.display_style_selected = oolong_style_set_create();
                    text_box.data.text_box.entered_style_selected = oolong_style_set_create();

                    /* Make them green. */
                    oolong_style_set_add(&button0.data.button.style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
                    oolong_style_set_add(&button1.data.button.style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
                    oolong_style_set_add(&button2.data.button.style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
                    oolong_style_set_add(&text_box.data.text_box.display_style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
                    oolong_style_set_add(&text_box.data.text_box.entered_style_selected, OOLONG_STYLE_BACKGROUND_GREEN);
                    break;
                case (2):
                    oolong_style_set_destroy(button0.data.button.style_selected);
                    oolong_style_set_destroy(button1.data.button.style_selected);
                    oolong_style_set_destroy(button2.data.button.style_selected);
                    oolong_style_set_destroy(text_box.data.text_box.display_style_selected);
                    oolong_style_set_destroy(text_box.data.text_box.entered_style_selected);
                    button0.data.button.style_selected = oolong_style_set_create();
                    button1.data.button.style_selected = oolong_style_set_create();
                    button2.data.button.style_selected = oolong_style_set_create();
                    text_box.data.text_box.display_style_selected = oolong_style_set_create();
                    text_box.data.text_box.entered_style_selected = oolong_style_set_create();
                    oolong_style_set_add(&button0.data.button.style_selected, OOLONG_STYLE_BACKGROUND_YELLOW);
                    oolong_style_set_add(&button1.data.button.style_selected, OOLONG_STYLE_BACKGROUND_YELLOW);
                    oolong_style_set_add(&button2.data.button.style_selected, OOLONG_STYLE_BACKGROUND_YELLOW);
                    oolong_style_set_add(&text_box.data.text_box.display_style_selected, OOLONG_STYLE_BACKGROUND_YELLOW);
                    oolong_style_set_add(&text_box.data.text_box.entered_style_selected, OOLONG_STYLE_BACKGROUND_YELLOW);
                    break;
                case (3):
                    oolong_style_set_destroy(button0.data.button.style_selected);
                    oolong_style_set_destroy(button1.data.button.style_selected);
                    oolong_style_set_destroy(button2.data.button.style_selected);
                    oolong_style_set_destroy(text_box.data.text_box.display_style_selected);
                    oolong_style_set_destroy(text_box.data.text_box.entered_style_selected);
                    button0.data.button.style_selected = oolong_style_set_create();
                    button1.data.button.style_selected = oolong_style_set_create();
                    button2.data.button.style_selected = oolong_style_set_create();
                    text_box.data.text_box.display_style_selected = oolong_style_set_create();
                    text_box.data.text_box.entered_style_selected = oolong_style_set_create();
                    oolong_style_set_add(&button0.data.button.style_selected, OOLONG_STYLE_BACKGROUND_RED);
                    oolong_style_set_add(&button1.data.button.style_selected, OOLONG_STYLE_BACKGROUND_RED);
                    oolong_style_set_add(&button2.data.button.style_selected, OOLONG_STYLE_BACKGROUND_RED);
                    oolong_style_set_add(&text_box.data.text_box.display_style_selected, OOLONG_STYLE_BACKGROUND_RED);
                    oolong_style_set_add(&text_box.data.text_box.entered_style_selected, OOLONG_STYLE_BACKGROUND_RED);
                    break;
                case (4):

                    /* 
                     * If our text box was selected and entered, we just have to
                     * make it active and the beginning of our loop will take
                     * care of the text entering, and oolong the deactivation.
                     */

                    text_box.data.text_box.state = OOLONG_ELEMENT_STATE_ACTIVE;
                    break;
            }
        }
    }
}

