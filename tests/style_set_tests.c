/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "style_set_tests.h"
#include "../oolong/styling.h"

SCRUTINY_UNIT_TEST style_set_add_test(void)
{
	oolong_style_set_t* style_set = oolong_style_set_create();
	scrutiny_assert_equal_array(L"", style_set, sizeof(wchar_t), wcslen(style_set));

	oolong_style_set_add(&style_set, OOLONG_STYLE_BACKGROUND_BLACK);
	scrutiny_assert_equal_array(L"\033[40m", style_set, sizeof(wchar_t), wcslen(style_set));

	oolong_style_set_add(&style_set, OOLONG_STYLE_BLUE);
	scrutiny_assert_equal_array(L"\033[40m\033[34m", style_set, sizeof(wchar_t), wcslen(style_set));
}

