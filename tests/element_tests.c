/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "element_tests.h"
#include "../oolong/element.h"

#define SELECTION_TEST_ELEMENTS 6

SCRUTINY_UNIT_TEST element_selected_index_test(void)
{
	oolong_element_t* elements[SELECTION_TEST_ELEMENTS + 1];
	elements[SELECTION_TEST_ELEMENTS] = NULL;

	for (size_t index = 0; index < SELECTION_TEST_ELEMENTS; index++)
	{
		elements[index] = malloc(sizeof(oolong_element_t));
		elements[index]->state = OOLONG_ELEMENT_STATE_NORMAL;
	}

	scrutiny_assert_equal_ssize_t(-1, oolong_element_get_selected_index(elements));

	for (ssize_t selection = 0; selection < SELECTION_TEST_ELEMENTS; selection++)
	{
		elements[selection]->state = OOLONG_ELEMENT_STATE_SELECTED;
		scrutiny_assert_equal_ssize_t(selection, oolong_element_get_selected_index(elements));
		elements[selection]->state = OOLONG_ELEMENT_STATE_NORMAL;
	}

	/* 
	 * Makes sure that when more than one element is selected the first one is
	 * returned.
	 */

	for (ssize_t selection = 0; selection + 1 < SELECTION_TEST_ELEMENTS; selection++)
	{
		elements[selection]->state = OOLONG_ELEMENT_STATE_SELECTED;
		elements[selection + 1]->state = OOLONG_ELEMENT_STATE_SELECTED;
		scrutiny_assert_equal_ssize_t(selection, oolong_element_get_selected_index(elements));
		elements[selection]->state = OOLONG_ELEMENT_STATE_NORMAL;
		elements[selection + 1]->state = OOLONG_ELEMENT_STATE_SELECTED;
	}

	for (size_t index = 0; index < SELECTION_TEST_ELEMENTS; index++)
		free(elements[index]);
}

SCRUTINY_UNIT_TEST element_selected_identifier_test(void)
{
	oolong_element_t* elements[SELECTION_TEST_ELEMENTS + 1];
	elements[SELECTION_TEST_ELEMENTS] = NULL;

	for (size_t index = 0; index < SELECTION_TEST_ELEMENTS; index++)
	{
		elements[index] = malloc(sizeof(oolong_element_t));
		elements[index]->state = OOLONG_ELEMENT_STATE_NORMAL;
		elements[index]->identifier = index;
	}

	scrutiny_assert_equal_enum(-1, oolong_element_get_selected_identifier(elements, -1));

	for (ssize_t selection = 0; selection < SELECTION_TEST_ELEMENTS; selection++)
	{
		elements[selection]->state = OOLONG_ELEMENT_STATE_SELECTED;
		scrutiny_assert_equal_enum(elements[selection]->identifier, oolong_element_get_selected_identifier(elements, -1));
		elements[selection]->state = OOLONG_ELEMENT_STATE_NORMAL;
	}

	/* 
	 * Makes sure that when more than one element is selected the first one is
	 * returned.
	 */

	for (ssize_t selection = 0; selection + 1 < SELECTION_TEST_ELEMENTS; selection++)
	{
		elements[selection]->state = OOLONG_ELEMENT_STATE_SELECTED;
		elements[selection + 1]->state = OOLONG_ELEMENT_STATE_SELECTED;
		scrutiny_assert_equal_ssize_t(elements[selection]->identifier, oolong_element_get_selected_identifier(elements, -1));
		elements[selection]->state = OOLONG_ELEMENT_STATE_NORMAL;
		elements[selection + 1]->state = OOLONG_ELEMENT_STATE_SELECTED;
	}

	for (size_t index = 0; index < SELECTION_TEST_ELEMENTS; index++)
		free(elements[index]);
}

SCRUTINY_UNIT_TEST element_render_test(void)
{
	/* Element 1: */
	{
		wchar_t* expected = L"  Content           " OOLONG_STYLE_CLEAR_STRING;
		oolong_element_t element =
		{
			.padding = 2,
			.style_normal = oolong_style_set_create(),
			.style_selected = oolong_style_set_create(),
			.content = L"Content",
			.width = 20,
			.state = OOLONG_ELEMENT_STATE_NORMAL,
			.alignment = OOLONG_ALIGN_LEFT
		};

		oolong_element_render_string(&element);
		scrutiny_assert_equal_array(expected, element.string, sizeof(wchar_t), wcslen(expected) + 1);
	}

	/* Element 2: */
	{
		wchar_t* expected = L"           Content  " OOLONG_STYLE_CLEAR_STRING;
		oolong_element_t element =
		{
			.padding = 2,
			.style_normal = oolong_style_set_create(),
			.style_selected = oolong_style_set_create(),
			.content = L"Content",
			.width = 20,
			.state = OOLONG_ELEMENT_STATE_NORMAL,
			.alignment = OOLONG_ALIGN_RIGHT
		};

		oolong_element_render_string(&element);
		scrutiny_assert_equal_array(expected, element.string, sizeof(wchar_t), wcslen(expected) + 1);
	}

	/* Element 3: */
	{
		wchar_t* expected = L"      Content       " OOLONG_STYLE_CLEAR_STRING;
		oolong_element_t element =
		{
			.padding = 2,
			.style_normal = oolong_style_set_create(),
			.style_selected = oolong_style_set_create(),
			.content = L"Content",
			.width = 20,
			.state = OOLONG_ELEMENT_STATE_NORMAL,
			.alignment = OOLONG_ALIGN_CENTER
		};

		oolong_element_render_string(&element);
		scrutiny_assert_equal_array(expected, element.string, sizeof(wchar_t), wcslen(expected) + 1);
	}

	/* Element 4: */
	{
		oolong_style_set_t* style_one = oolong_style_set_create();
		oolong_style_set_t* style_two = oolong_style_set_create();

		oolong_style_set_add(&style_one, OOLONG_STYLE_BACKGROUND_BLUE);
		oolong_style_set_add(&style_two, OOLONG_STYLE_BOLD);
		
		wchar_t* expected = L"  Content           " OOLONG_STYLE_CLEAR_STRING;
		wchar_t expected_one[wcslen(style_one) + wcslen(expected) + 1];
		wchar_t expected_two[wcslen(style_two) + wcslen(expected) + 1];

		wcscpy(expected_one, style_one);
		wcscpy(expected_two, style_two);
		wcscpy(&expected_one[wcslen(style_one)], expected);
		wcscpy(&expected_two[wcslen(style_two)], expected);
		
		oolong_element_t element =
		{
			.padding = 2,
			.style_normal = style_one,
			.style_selected = style_two,
			.content = L"Content",
			.width = 20,
			.state = OOLONG_ELEMENT_STATE_NORMAL,
			.alignment = OOLONG_ALIGN_LEFT
		};

		oolong_element_render_string(&element);
		scrutiny_assert_equal_array(expected_one, element.string, sizeof(wchar_t), wcslen(expected_one));

		element.state = OOLONG_ELEMENT_STATE_SELECTED;
		oolong_element_render_string(&element);
		scrutiny_assert_equal_array(expected_two, element.string, sizeof(wchar_t), wcslen(expected_two));
	}
}

SCRUTINY_UNIT_TEST element_select_next_test(void)
{
	oolong_element_t* elements[SELECTION_TEST_ELEMENTS + 1];
	elements[SELECTION_TEST_ELEMENTS] = NULL;

	for (size_t index = 0; index < SELECTION_TEST_ELEMENTS; index++)
	{
		elements[index] = malloc(sizeof(oolong_element_t));
		elements[index]->state = OOLONG_ELEMENT_STATE_NORMAL;
		elements[index]->supported_states = OOLONG_ELEMENT_STATE_NORMAL | OOLONG_ELEMENT_STATE_SELECTED;
	}
	
	elements[0]->state = OOLONG_ELEMENT_STATE_SELECTED;
	
	for (size_t index = 0; index < SELECTION_TEST_ELEMENTS; index++)
	{
		scrutiny_assert_equal_enum(OOLONG_ELEMENT_STATE_SELECTED, elements[index]->state);
		oolong_element_select_next(elements);
	}

	/* Checks that it loops back to the beginning. */
	scrutiny_assert_equal_enum(OOLONG_ELEMENT_STATE_SELECTED, elements[0]->state);

	for (size_t index = 0; index < SELECTION_TEST_ELEMENTS; index++)
		free(elements[index]);
}

SCRUTINY_UNIT_TEST element_select_previous_test(void)
{
	oolong_element_t* elements[SELECTION_TEST_ELEMENTS + 1];
	elements[SELECTION_TEST_ELEMENTS] = NULL;

	for (size_t index = 0; index < SELECTION_TEST_ELEMENTS; index++)
	{
		elements[index] = malloc(sizeof(oolong_element_t));
		elements[index]->state = OOLONG_ELEMENT_STATE_NORMAL;
		elements[index]->supported_states = OOLONG_ELEMENT_STATE_NORMAL | OOLONG_ELEMENT_STATE_SELECTED;
	}
	
	/* This will check the loop around first. */
	elements[0]->state = OOLONG_ELEMENT_STATE_SELECTED;
	
	for (ssize_t index = SELECTION_TEST_ELEMENTS - 1; index > 0; index--)
	{
		oolong_element_select_previous(elements);
		scrutiny_assert_equal_enum(OOLONG_ELEMENT_STATE_SELECTED, elements[index]->state);
	}

	for (size_t index = 0; index < SELECTION_TEST_ELEMENTS; index++)
		free(elements[index]);
}

