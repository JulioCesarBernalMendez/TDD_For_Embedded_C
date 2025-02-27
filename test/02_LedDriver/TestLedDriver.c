/**
 * @file    TestLedDriver.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Test led driver example source file containing the test cases and test group runner for the LedDriver module.
 *          This code structure is based on the test examples provided in the "Test-Driven Development for Embedded C"
 *          book by James W. Grenning, which use unity_fixture.h unlike the examples in https://www.throwtheswitch.org/unity
 *          which use unity.h instead.
 * 
 *          LED Driver Tests:
 *          -----------------
 *          - All LEDs are off after the driver is initialized
 *          - A single LED can be turned on
 *          - A single LED can be turned off
 *          - Multiple LEDs can be turned on/off
 *          - Turn on all LEDs
 *          - Turn off all LEDs
 *          - Query LED state
 *          - Check boundary values
 *          - Check out-of-bounds values
 * 
 * @version 0.1
 * @date    2025-02-21
 */

#include "unity_fixture.h"
#include "LedDriver.h"

TEST_GROUP( LedDriver );

TEST_SETUP( LedDriver )
{
}

TEST_TEAR_DOWN( LedDriver )
{
}

/* TEST1 */
TEST( LedDriver, LedsOffAfterCreate )
{
    /* simulate the 16 LEDs are all turned on
       during hardware initialization */
    uint16_t virtualLeds = 0xffff;

    /* per the spec, the LedDriver_Create() is responsible for 
       turning all LEDs off during initialization */
    LedDriver_Create( &virtualLeds );

    /* check that in fact all LEDs are off upon initialization */
    TEST_ASSERT_EQUAL_HEX16( 0, virtualLeds );
}

TEST_GROUP_RUNNER( LedDriver )
{
    RUN_TEST_CASE( LedDriver, LedsOffAfterCreate );
}
