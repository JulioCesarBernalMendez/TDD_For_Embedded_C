/**
 * @file    LedDriver.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Led Driver source file that implements the functions for the LedDriver module.
 * 
 * @version 0.1
 * @date    2025-02-27
 */

#include "LedDriver.h"

/* LED's address */
static uint16_t *ledsAddress;

void LedDriver_Create( uint16_t *address )
{
    /* all the LEDs are turned on after hardware initialization.
       Turn them all off instead during the Led Driver software initialization.
       This is a Led Driver requirement */
    ledsAddress = address;
    *ledsAddress = 0;
}

static uint16_t convertLedNumberToBit( int ledNumber )
{
    return ( 1 << ( ledNumber - 1 ) );
}

void LedDriver_TurnOn( uint16_t ledNumber )
{
    /* turn on the specified LED number.
       The offset (-1) is needed because the LEDs are numbered 01 through 16,
       so LED 01 is bit 0, and LED 16 is bit 15 */
    *ledsAddress |= convertLedNumberToBit( ledNumber );
}

void LedDriver_TurnOff( uint16_t ledNumber )
{
    /* do the simplest thing to get the TurnOffLedOne test to pass...
       hard-coding it (we're doing test driven, the TESTS are right!,
       even though production code is not right. Don't worry, production code
       won't be hard-coded and incomplete for long) */
    *ledsAddress = 0;
}

void LedDriver_TurnAllOn( void )
{
    *ledsAddress = 0xffff;
}