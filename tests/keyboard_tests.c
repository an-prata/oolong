/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "keyboard_tests.h"

SCRUTINY_UNIT_TEST buffered_keys_test(void)
{
	const size_t keys_length = 4;
	
	oolong_key_t keys[] =
	{
		KEY_A,
		KEY_BACKSPACE,
		KEY_RETURN,
		KEY_DOWN
	};

	oolong_keyboard_buffer_keys(keys, keys_length);

	for (size_t i = 0; i < keys_length; i++)
		scrutiny_assert_equal_enum(keys[i], oolong_keyboard_get_key());
}

