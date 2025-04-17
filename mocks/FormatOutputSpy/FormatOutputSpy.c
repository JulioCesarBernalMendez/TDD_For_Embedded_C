/**
 * @file    FormatOutputSpy.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Format Output module spy file, it contains the necessary resources to make use of the fake
 *          FormatOutput() function (printf), which is replaced during TEST() by updating the FormatOutput
 *          function pointer (FormatOutput is defined and implemented in Utils.c)
 * 
 * @version 0.1
 * @date    2025-04-16
 */

#include "FormatOutputSpy.h"
#include <stdlib.h> /* calloc() / free() */
#include <stdarg.h> /* va_start() / va_end() */
#include <stdio.h>  /* vsnprintf() */

/* file-scope buffer (aka. string) used to "catch" the contents of of the production FormatOutput() (i.e. printf()) */
static char *buffer = 0;

/* File-scope variable used to keep track of the size of the string.
   This is used for dynamically allocation of the buffer by calloc() */
static size_t buffer_size;

/* Internal buffer (string) index.
   This is used in future calls to FormatOutput() so that the new string is appended
   to the previously written string */
static int buffer_offset = 0;

/* Used to know the number of spaces (chars) left in the buffer (string) */
static int buffer_used = 0;

void FormatOutputSpy_Create( int size )
{
    /* destroy any previous buffer (if any) */
    FormatOutputSpy_Destroy();

    /* take into account the null termination character as well */
    buffer_size = size + 1;

    /* Allocate dynamically a memory space of "buffer_size" bytes */
    buffer = ( char* ) calloc( buffer_size, sizeof( char ) );

    /* restart both buffer offset and buffer used */
    buffer_offset = 0;
    buffer_used = 0;

    /* the buffer is empty after creation, there is only the null character */
    buffer[ 0 ] = '\0';
}

void FormatOutputSpy_Destroy( void )
{
    /* if the buffer (string) has not been created */
    if ( buffer == 0 )
        return; /* return, there's no buffer to destroy */

    /* otherwise, deallocate the buffer, buffer is not usable and
       needs to be created again */
    free( buffer );

    /* buffer has no valid address assiged to it now (0 = NULL).
       Any attempt to write to it after a FormatOutput_Destroy() will results in segmentation fault.
       buffer is not usable and needs to be created again */
    buffer = 0;
}

/* The elipsis (...) indicatres that the function receives a variable number of arguments of any type */
int FormatOutputSpy( const char* format, ... )
{
    /* used to store number of bytes written to the buffer by each call to vsnprint(),
       then compute both the buffer_offset and buffer_used values */
    int written_size;

    /* stores information needed by macros va_start, va_arg and va_end */
    va_list arguments;

    /* initializes the va_list object */
    va_start( arguments, format );

    /* vsnprintf() function formats and stores a series of characters and values in the buffer target-string.

       vsnprintf() function works just like snprintf(), except that the va_list "arguments" points to a list of arguments
       whose number can vary from call to call in the program. These arguments should be initialized by the va_start
       function for each call.
       
       In contrast, the snprintf() function can have a list of arguments, but the number of arguments in that list
       is fixed when you compile the program.
       
       vsnprintf() returns the number of bytes that are written in the array, not counting the ending null character.
       
       Format:
       int vsnprintf( char *target-string, size_t n, const char *format, va_list arg_ptr ) */
    written_size = vsnprintf( buffer + buffer_offset, buffer_size - buffer_used, format, arguments );

    /* new information was written to the buffer, update both its offset and the number of bytes (chars) used */
    buffer_offset += written_size;
    buffer_used += written_size;

    /* Clean up variable-lenght argument list.
       va_end is a macro that facilitates a normal return from a function whose
       variable-length argument list was referred to by the va_start macro */
    va_end( arguments );

    return 1;
}

const char *FormatOutputSpy_GetOutput( void )
{
    return buffer;
}
