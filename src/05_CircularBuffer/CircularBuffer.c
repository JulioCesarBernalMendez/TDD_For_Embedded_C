/**
 * @file    CircularBuffer.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Circular Buffer module source file that implements the functions for the Circular Buffer module.
 * 
 * @version 0.1
 * @date    2025-04-22
 */

#include "CircularBuffer.h"
#include <Utils.h>
#include <stdlib.h>

/* structure data type to hold a circular buffer that stores integer values */
typedef struct CircularBufferStruct
{
    int count;    /* number of elements currently stored in the circular buffer (maximum value is limited by 'capacity') */
    int index;    /* "pointer" to the next location to store a value in the circular buffer */
    int outdex;   /* "pointer" to the next location to read a value from the circular buffer */
    int capacity; /* circular buffer capacity (i.e. number of elements it can store) */
    int *values;  /* array/elements of the circular buffer */
} CircularBufferStruct;

enum { BUFFER_GUARD = -999 }; /* circular buffer delimiter */

CircularBuffer CircularBuffer_Create( int capacity )
{
    /* Allocate (dynamically) a block of memory to store the circular buffer
       and initialize it to zero */
    CircularBuffer self = calloc( capacity, sizeof( CircularBufferStruct ) );

    /* Define circular's buffer capacity */
    self->capacity = capacity;

    /* Allocate (dynamically) an array of integers to hold the circular buffer's values
       and initialize all of them to zero */
    self->values = calloc( capacity + 1, sizeof( int ) );

    /* delimit the circular buffer */
    self->values[ capacity ] = BUFFER_GUARD;

    /* return the address of the recently allocated circular buffer */
    return self;
}

void CircularBuffer_Destroy( CircularBuffer self )
{
    /* Deallocate the circular buffer's array */
    free( self->values );

    /* Deallocate the circular buffer */
    free( self );
}

int CircularBuffer_Put( CircularBuffer self, int value )
{
    /* if the circular buffer is already full */
    if ( self->count >= self->capacity )
    {
        /* don't insert anything into the circular buffer */
        return 0;
    }

    /* otherwhise ... */

    /* insert the integer value into the circular buffer */
    self->values[ self->index++ ] = value;

    /* if index ("pointer" to next location to insert) is out of the circular buffer's bounds */
    if ( self->index >= self->capacity )
    {
        /* restart the index */
        self->index = 0;
    }

    /* increase the number of elements currently stored in the circular buffer */
    self->count++;

    return 1;
}

void CircularBuffer_Print( CircularBuffer self )
{
    int i; /* index */
    int currentValue = self->outdex; /* "pointer" to the next location to read a value from the circular buffer */

    FormatOutput( "Circular buffer content:\n<" );

    /* loop through the circular buffer('s array) */
    for ( i = 0; i < self->count; i++ )
    {
        /* for each value (if any) of the circular buffer */
        if ( i != 0 )
        {
            /* separate values with a comma */
            FormatOutput( ", " );
        }

        /* provide format to the current value read from the circular buffer */
        FormatOutput( "%d", self->values[ currentValue++ ] );

        /* if outdex ("pointer" to next value to print) is out of the circular buffer's bounds */
        if ( currentValue >= self->capacity )
        {
            /* restart the index */
            currentValue = 0;
        }
    }

    FormatOutput( ">\n" );
}
