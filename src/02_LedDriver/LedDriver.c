/**
 * @file    LedDriver.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Led Driver source file that implements the functions for the LedDriver module.
 * 
 * @version 0.1
 * @date    2025-02-27
 */

#include "LedDriver.h"
#include "RuntimeError.h"

enum { ALL_LEDS_ON = ~0, ALL_LEDS_OFF = ~ALL_LEDS_ON };
enum { FIRST_LED = 1, LAST_LED = 16 };

/* LED's address */
static uint16_t *ledsAddress;

/* LED's state */
static uint16_t ledsImage;

static void updateHardware( void )
{
    /* set the LEDs' state */
    *ledsAddress = ledsImage;
}

static uint16_t convertLedNumberToBit( uint16_t ledNumber )
{
    /* The offset (-1) is needed because the LEDs are numbered 1 through 16,
       so LED 1 is bit 0, and LED 16 is bit 15 */
    return ( 1 << ( ledNumber - 1 ) );
}

static BOOL IsLedInOfBounds( uint16_t ledNumber )
{
    /* return TRUE if LED is within the valid range, return FALSE otherwise */
    return ( ( ledNumber >= FIRST_LED ) && ( ledNumber <= LAST_LED ) );
}

static void setLedImageBit( uint16_t ledNumber )
{
    /* update the LEDs' state */
    ledsImage |= convertLedNumberToBit( ledNumber );
}

static void clearLedImageBit( uint16_t ledNumber )
{
    /* update the LEDs' state */
    ledsImage &= ~convertLedNumberToBit( ledNumber );
}

void LedDriver_Create( uint16_t *address )
{
    /* all the LEDs are turned on after hardware initialization.
       Turn them all off instead during the Led Driver software initialization.
       This is a Led Driver requirement */

    ledsAddress  = address;      /* assign the LEDs' address */
    ledsImage    = ALL_LEDS_OFF; /* store the LEDs' state */
    updateHardware();            /* set the LEDs' state */
}

void LedDriver_TurnOn( uint16_t ledNumber )
{
    /* only turn on LEDs within the 1-16 range */
    if ( IsLedInOfBounds( ledNumber ) )
    {
        /* update the LEDs' state */
        setLedImageBit( ledNumber );

        /* turn on the specified LED number */
        updateHardware();
    }
    /* if an attempt is made to turn on an out-of-bounds LED */
    else
    {
        /* produce a runtime error:
           - the first parameter is the error message
           - the second parameter is the error paramater */
        RUNTIME_ERROR( "LED Driver: out-of-bounds LED", ledNumber );
    }
}

void LedDriver_TurnOff( uint16_t ledNumber )
{
    /* only turn off LEDs within the 1-16 range */
    if ( IsLedInOfBounds( ledNumber ) )
    {
        /* update the LEDs' state */
        clearLedImageBit( ledNumber );

        /* turn off the specified LED number, */
        updateHardware();
    }
}

void LedDriver_TurnAllOn( void )
{
    /* store the LEDs' state */
    ledsImage = ALL_LEDS_ON;

    /* turn on all the LEDs */
    updateHardware();
}
