/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_ERROR_H
#define OOLONG_ERROR_H

#include <stdlib.h>
#include <stdbool.h>

#ifndef __GNUC__
#warning "OOLONG: not compiling with GCC, error.h utilises GCC compiler extensions."
#endif // __GNUC__

#define oolong_error_record(n) oolong_error_debug_record(n, __FILE__, __func__, __LINE__)

enum oolong_error_e
{
    OOLONG_ERROR_NONE               = 0b00000000,  /* No error. */
    OOLONG_ERROR_INVALID_ARGUMENT   = 0b00000001,  /* Argument passed to function was invalid. */
    OOLONG_ERROR_NO_SUCH_ELEMENT    = 0b00000010,  /* Requested element was not found in array or similar. */
    OOLONG_ERROR_NOT_ENOUGH_MEMORY  = 0b00000100,  /* Oolong has run out of, or failed to allocate memory. */
    OOLONG_ERROR_FAILED_IO_READ     = 0b00001000,  /* Failed to read from I/O. */
    OOLONG_ERROR_FAILED_IO_WRITE    = 0b00010000,  /* Failed to write to I/O. */
};

typedef enum oolong_error_e oolong_error_t;

/*
 * Set whether or not oolong exits on errors.
 */
void oolong_error_set_exit_on_error(bool exit_on_error);

/*
 * Records the given error, recording the same type of error twice, without
 * clearing errors inbetween, does nothing. Returns the recorded error.
 *
 * This function should, in most cases, be called the moment an error is found
 * rather than have the error returned up a level and recorder there. This
 * means that if a function ever returns an error, it should have also recorded
 * it and the function receiving this error should not record it again, it may
 * however return the error itself.
 *
 * It should also never be assumed that the program shall exit on an error,
 * this means all functions should be expected to prevent things such as heap
 * corruption or overuns in the event of an error. If ever a function records
 * an error without also returning it, the return value should be indicative of
 * an error or prevent further errors in its use (e.g. return 0 for unsigned
 * types, -1 for signed, NULL for pointers, etc.).
 */
oolong_error_t oolong_error_debug_record(oolong_error_t error, const char* file, const char* function, size_t line);

/*
 * Clears all recorded errors.
 */
void oolong_error_clear_all(void);

/*
 * Clears the specific given error.
 */
void oolong_error_clear(oolong_error_t error);

/*
 * Gets all recorded errors since the last clear.
 */ 
oolong_error_t oolong_error_get_all(void);

/*
 * Checks the recorded errors to the given error, returns that error if it has
 * occured, returns OOLONG_ERROR_NONE otherwise.
 */
oolong_error_t oolong_error_check(oolong_error_t error);

#endif // OOLONG_ERROR_H

