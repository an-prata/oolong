/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#include "error_tests.h"
#include "../oolong/error.h"

SCRUTINY_UNIT_TEST error_test(void)
{
	oolong_error_set_exit_on_error(false);
    scrutiny_assert_equal_enum(oolong_error_get_all(), OOLONG_ERROR_NONE);

    oolong_error_record(OOLONG_ERROR_INVALID_ARGUMENT);

    scrutiny_assert_equal_enum(oolong_error_get_all(), OOLONG_ERROR_INVALID_ARGUMENT);
    scrutiny_assert_equal_enum(oolong_error_check(OOLONG_ERROR_INVALID_ARGUMENT), OOLONG_ERROR_INVALID_ARGUMENT);
    
    oolong_error_clear_all();
    
    scrutiny_assert_equal_enum(oolong_error_get_all(), OOLONG_ERROR_NONE);

    oolong_error_record(OOLONG_ERROR_FAILED_IO_READ);
    oolong_error_record(OOLONG_ERROR_FAILED_IO_WRITE);

    scrutiny_assert_equal_enum(oolong_error_check(OOLONG_ERROR_NOT_ENOUGH_MEMORY), OOLONG_ERROR_NONE);
    scrutiny_assert_equal_enum(oolong_error_check(OOLONG_ERROR_FAILED_IO_READ), OOLONG_ERROR_FAILED_IO_READ);

    oolong_error_clear(OOLONG_ERROR_FAILED_IO_READ);

    scrutiny_assert_equal_enum(oolong_error_check(OOLONG_ERROR_FAILED_IO_READ), OOLONG_ERROR_NONE);
	oolong_error_set_exit_on_error(true);
}
