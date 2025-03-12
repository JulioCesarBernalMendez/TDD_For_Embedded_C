/**
 * @file    LedDriver.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Led Driver header file containing the prototype functions implemented by LedDriver.c
 * 
 * @version 0.1
 * @date    2025-02-27
 */

#ifndef LEDDRIVER_H
#define LEDDRIVER_H

    #include <stdint.h>

    typedef int BOOL;

    void LedDriver_Create( uint16_t *virtualLeds );
    void LedDriver_TurnOn( uint16_t ledNumber );
    void LedDriver_TurnOff( uint16_t ledNumber );
    void LedDriver_TurnAllOn( void );
    BOOL LedDriver_IsOn( int ledNumber );

#endif
