/**
 * @file    CircularBuffer.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Circular Buffer module header file containing the prototype functions implemented by CircularBuffer.c
 * 
 * @version 0.1
 * @date    2025-04-22
 */

#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

    typedef struct CircularBufferStruct *CircularBuffer; /* pointer type to a CircularBufferStruct */

    CircularBuffer CircularBuffer_Create( int capacity );
    void CircularBuffer_Destroy( CircularBuffer self );
    int CircularBuffer_Put( CircularBuffer self, int value );
    void CircularBuffer_Print( CircularBuffer self );

#endif
