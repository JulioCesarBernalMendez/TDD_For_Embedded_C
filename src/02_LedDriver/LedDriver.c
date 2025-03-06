/**
 * @file    LedDriver.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Led Driver source file that implements the functions for the LedDriver module.
 * 
 * @version 0.1
 * @date    2025-02-27
 */

#include "LedDriver.h"

enum { ALL_LEDS_ON = ~0, ALL_LEDS_OFF = ~ALL_LEDS_ON };

/* LED's address */
static uint16_t *ledsAddress;

void LedDriver_Create( uint16_t *address )
{
    /* all the LEDs are turned on after hardware initialization.
       Turn them all off instead during the Led Driver software initialization.
       This is a Led Driver requirement */
    ledsAddress = address;
    *ledsAddress = ALL_LEDS_OFF;
}

static uint16_t convertLedNumberToBit( int ledNumber )
{
    /* The offset (-1) is needed because the LEDs are numbered 1 through 16,
       so LED 1 is bit 0, and LED 16 is bit 15 */
    return ( 1 << ( ledNumber - 1 ) );
}

void LedDriver_TurnOn( uint16_t ledNumber )
{
    /* turn on the specified LED number */
    *ledsAddress |= convertLedNumberToBit( ledNumber );
}

void LedDriver_TurnOff( uint16_t ledNumber )
{
    /* turn off the specified LED number, */
    *ledsAddress &= ~convertLedNumberToBit( ledNumber );
}

void LedDriver_TurnAllOn( void )
{
    *ledsAddress = ALL_LEDS_ON;
}
