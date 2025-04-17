/**
 * @file    FormatOutputSpy.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Format Output module spy header file containing the prototype spy functions for
 *          the Format Output Spy module (FormatOutputSpy.c).
 * 
 * @version 0.1
 * @date    2025-04-16
 */

#ifndef FORMATOUTPUTSPY_H
#define FORMATOUTPUTSPY_H

    void FormatOutputSpy_Create( int size );
    void FormatOutputSpy_Destroy( void );
    int FormatOutputSpy( const char* format, ... );
    const char *FormatOutputSpy_GetOutput( void );

#endif
