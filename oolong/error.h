/* 
 * Copyright (c) 2023 Evan Overman (https://an-prata.it). Licensed under the MIT License.
 * See LICENSE file in repository root for complete license text.
 */

#ifndef OOLONG_ERROR_H
#define OOLONG_ERROR_H

#ifndef __GNUC__
#warning "OOLONG: not compiling with GCC, error.h utilises GCC compiler extensions."
#endif // __GNUC__

enum oolong_error_e
{
    OOLONG_ERROR_NONE                 = 0b00000000,  /* No error. */
    OOLONG_ERROR_INVALID_ARGUMENT     = 0b00000001,  /* Argument passed to function was invalid. */
    OOLONG_ERROR_NO_SUCH_ELEMENT      = 0b00000010,  /* Requested element was not found in array or similar. */
    OOLONG_ERROR_COULD_NOT_OPEN_FILE  = 0b00000100,  /* Failed to open file. */
    OOLONG_ERROR_FAILED_IO_READ       = 0b00001000,  /* Failed to read from I/O. */
    OOLONG_ERROR_FAILED_IO_WRITE      = 0b00010000,  /* Failed to write to I/O. */
};

typedef enum oolong_error_e oolong_error_t;

/*
 * Records the given error, recording the same type of error twice, without
 * clearing errors inbetween, does nothing.
 */
void oolong_error_record(oolong_error_t error);

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

