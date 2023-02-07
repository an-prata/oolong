/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "../oolong/oolong.h"
#include "text_box_tests.h"

SCRUTINY_UNIT_TEST text_box_register_key_test(void)
{
	oolong_set_locale();
	
	oolong_text_box_options_t options = 
	{
		.display_text = L"",
		.activations_keys = NULL,
		.deactivation_keys = NULL,
		.state = OOLONG_ELEMENT_STATE_ACTIVE,
		.style_normal = oolong_style_set_create(),
		.style_selected = oolong_style_set_create(),
		.style_active = oolong_style_set_create(),
		.style_disabled = oolong_style_set_create(),
		.padding = 0,
		.width = 0,
		.alignment = OOLONG_ELEMENT_CONTENT_ALIGN_LEFT,
		.identifier = 0
	};
	
	wchar_t input_text[] = L"hello world";
	oolong_text_box_t* text_box = oolong_text_box_create(&options);

	for (size_t index = 0; index < wcslen(input_text); index++)
		oolong_text_box_register_keystroke(text_box, (oolong_key_t)input_text[index]);

	oolong_element_render_string((oolong_element_t*)text_box);

	wchar_t* text_box_string = oolong_element_get_string((oolong_element_t*)text_box);
	scrutiny_assert_equal_array(input_text, text_box_string, sizeof(wchar_t), wcslen(input_text));
	oolong_text_box_destroy(text_box);
}
