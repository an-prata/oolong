#include <stdbool.h>
#include "../oolong/oolong.h"

/*
 * If you haven't gone through the first example do that before looking here.
 * This example will showcase a page containing one of two different views.
 */

enum element_ids_e
{
	HOME_TITLE,
	HOME_QUIT_BUTTON,
	HOME_TEXT_BOX,
	DIALOG_QUIT,
	DIALOG_CANCEL
};

typedef enum element_ids_e element_ids_t;

int main()
{
	oolong_set_locale();

	/*
	 * The dialog view is new; it has a large text box at the top which can line 
	 * wrap down and the buttons are displayed in a row next to eachother on the
	 * bottom. Its functions operate very similarly to a stack view.
	 */
	
	oolong_dialog_view_options_t dialog_options =
	{
		.view_top_margin = 2,
		.view_side_margin = 6,
		.button_gap = 2,
		.text_button_gap = 2,
		.button_padding = 1
	};

	oolong_stack_view_options_t stack_options =
	{
		.view_top_margin = 2,
		.view_side_margin = 2,
		.align = OOLONG_ALIGN_WIDTH,
		.element_width = 32,
		.element_padding = 1,
		.element_gap = 1
	};

	oolong_dialog_view_t* dialog = oolong_dialog_view_create(&dialog_options);
	oolong_stack_view_t* stack = oolong_stack_view_create(&stack_options);

	oolong_stack_view_element_t title =
	{
		.identifier = HOME_TITLE,
		.type = OOLONG_ELEMENT_TYPE_LABEL,
		.data.label =
		{
			.style = oolong_style_set_create(),
			.text = L"A Lovely Home Screen"
		}
	};

	oolong_stack_view_element_t text_box =
	{
		.identifier = HOME_TEXT_BOX,
		.type = OOLONG_ELEMENT_TYPE_TEXT_BOX,
		.data.text_box = 
		{
			.display_text = L"A Lovely Text Box",
			.entered_text = NULL,
			.state = OOLONG_ELEMENT_STATE_SELECTED,
			.display_style = oolong_style_set_create(),
			.entered_style = oolong_style_set_create(),
			.display_style_selected = oolong_style_set_create(),
			.entered_style_selected = oolong_style_set_create()
		}
	};

	oolong_stack_view_element_t quit_button =
	{
		.identifier = HOME_QUIT_BUTTON,
		.type = OOLONG_ELEMENT_TYPE_BUTTON,
		.data.button =
		{
			.text = L"Quite This Lovely Example Program :( ...",
			.state = OOLONG_ELEMENT_STATE_NORMAL,
			.style = oolong_style_set_create(),
			.style_selected = oolong_style_set_create()
		}
	};

	oolong_dialog_view_button_t cancel_quit_button =
	{
		.identifier = DIALOG_CANCEL,
		.state = OOLONG_DIALOG_BUTTON_STATE_SELECTED,
		.text = L"Go Back :D",
		.style = oolong_style_set_create(),
		.style_selected = oolong_style_set_create()
	};

	oolong_dialog_view_button_t confirm_quit_button =
	{
		.identifier = DIALOG_QUIT,
		.state = OOLONG_DIALOG_BUTTON_STATE_NORMAL,
		.text = L"Leave...",
		.style = oolong_style_set_create(),
		.style_selected = oolong_style_set_create()
	};

	oolong_style_set_add(&title.data.label.style, OOLONG_STYLE_BOLD);
	oolong_style_set_add(&title.data.label.style, OOLONG_STYLE_ITALIC);
	
	oolong_style_set_add(&text_box.data.text_box.display_style_selected, OOLONG_STYLE_BACKGROUND_BLUE);
	oolong_style_set_add(&text_box.data.text_box.entered_style_selected, OOLONG_STYLE_BACKGROUND_CYAN);

	oolong_style_set_add(&quit_button.data.button.style_selected, OOLONG_STYLE_BACKGROUND_BLUE);
	oolong_style_set_add(&confirm_quit_button.style_selected, OOLONG_STYLE_BACKGROUND_BLUE);
	oolong_style_set_add(&cancel_quit_button.style_selected, OOLONG_STYLE_BACKGROUND_BLUE);

	oolong_dialog_view_set_text(dialog, L"Do you really want to quit this lovely example? Like really, really want to? Are you absolutely sure about it? :(");

	oolong_stack_view_add_element(stack, &title);
	oolong_stack_view_add_element(stack, &text_box);
	oolong_stack_view_add_element(stack, &quit_button);

	oolong_dialog_view_add_button(dialog, &cancel_quit_button);
	oolong_dialog_view_add_button(dialog, &confirm_quit_button);

	oolong_terminal_hide_cursor(stdout);
	oolong_terminal_enter_alternate_screen(stdout);
	oolong_disable_canonical_input();

	oolong_page_t page =
	{
		.view.stack_view = stack,
		.view_type = OOLONG_VIEW_TYPE_STACK
	};

	while (true)
	{
		oolong_page_print(&page, stdout);

		oolong_key_t key = oolong_keyboard_get_key();

		if (page.view_type == OOLONG_VIEW_TYPE_STACK && oolong_stack_view_get_is_text_box_active(page.view.stack_view))
		{
			/* 
			 * Since page doesnt handle text boxes we need to do a little extra work. 
			 */

			oolong_stack_view_active_text_box_register_key(page.view.stack_view, key);
			continue;
		}

		/*
		 * Since the stack view and dialog views display in opposite directions it
		 * makes sense to change the orientation of the controls from up/down to 
		 * left/right.
		 */

		if (page.view_type == OOLONG_VIEW_TYPE_STACK && (key == KEY_LOWERCASE_J || key == KEY_J || key == KEY_DOWN))
		{
			oolong_page_select_next_element(&page);
		}
		else if (page.view_type == OOLONG_VIEW_TYPE_STACK && (key == KEY_LOWERCASE_K || key == KEY_K || key == KEY_UP))
		{
			oolong_page_select_previous_element(&page);
		}
		else if (page.view_type == OOLONG_VIEW_TYPE_DIALOG && (key == KEY_LOWERCASE_L || key == KEY_L || key == KEY_RIGHT))
		{
			oolong_page_select_next_element(&page);
		}
		else if (page.view_type == OOLONG_VIEW_TYPE_DIALOG && (key == KEY_LOWERCASE_H || key == KEY_H || key == KEY_LEFT))
		{
			oolong_page_select_previous_element(&page);
		}
		else if (key == KEY_LOWERCASE_Q || key == KEY_Q)
		{
			/* Here we switch views. */
			page.view.dialog_view = dialog;
			page.view_type = OOLONG_VIEW_TYPE_DIALOG;
		}
		else if (key == KEY_RETURN)
		{
			switch (oolong_page_get_selected_identifier(&page))
			{
				case (HOME_QUIT_BUTTON):
					/* We switch views here too. */
					page.view.dialog_view = dialog;
					page.view_type = OOLONG_VIEW_TYPE_DIALOG;
					break;

				case (DIALOG_CANCEL):
					/* Here we switch as well, but back to the stack view. */
					page.view.stack_view = stack;
					page.view_type = OOLONG_VIEW_TYPE_STACK;
					break;

				case (DIALOG_QUIT):
					oolong_terminal_show_cursor(stdout);
					oolong_terminal_exit_alternate_screen(stdout);
					oolong_restore_canonical_input();
					exit(EXIT_SUCCESS);

				case (HOME_TEXT_BOX):
					text_box.data.text_box.state = OOLONG_ELEMENT_STATE_ACTIVE;
					break;
			}
		}
	}
}

