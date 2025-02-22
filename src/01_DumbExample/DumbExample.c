/**
 * @file    DumbExample.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Dumb example source file that implements the functions for the DumbExample module.
 *          This code is based on the one explained in https://www.throwtheswitch.org/unity
 * 
 * @version 0.1
 * @date    2025-02-21
 */

#include "DumbExample.h"

int8_t AverageThreeBytes( int8_t a, int8_t b, int8_t c )
{
    return ( int8_t )(  ( ( int16_t )a + ( int16_t )b + ( int16_t )c ) / 3 );
}
