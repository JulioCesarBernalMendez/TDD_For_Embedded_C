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

/* address of the 16 LEDs */
static uint16_t virtualLeds;

TEST_GROUP( LedDriver );

TEST_SETUP( LedDriver )
{
   /* this will be called at the beginning of
      every TEST() */
   LedDriver_Create( &virtualLeds );
}

TEST_TEAR_DOWN( LedDriver )
{
}

/* TEST 1 */
TEST( LedDriver, LedsOffAfterCreate )
{
   /* note: this test has another virtualLeds and another
            call to LedDriver_Create(), even though there
            is already a global virtualLeds and LedDriver_Create()
            is called automatically at the beginning of every TEST()
            thanks to TEST_SETUP. This is because this test is a
            special case; its goal is to test the LedDriver_Create()
            function */

   /* simulate the 16 LEDs are all turned on
      during hardware initialization */
   uint16_t virtualLeds = 0xffff;

   /* per the spec, the LedDriver_Create() is responsible for 
      turning all LEDs off during initialization */
   LedDriver_Create( &virtualLeds );

   /* check that in fact all LEDs are off upon initialization */
   TEST_ASSERT_EQUAL_HEX16( 0, virtualLeds );
}

/* TEST 2 */
TEST( LedDriver, TurnOnLedOne )
{
   /* turn the first LED on.
      The LEDs are numbered 01 through 16, so bit 0 is LED 01 */
   LedDriver_TurnOn( 1 << 0 );

   /* check LED 01 is on */
   TEST_ASSERT_EQUAL_HEX16( 1, virtualLeds );
}

/* TEST 3 */
TEST( LedDriver, TurnOffLedOne )
{
   /* turn the first LED on.
      The LEDs are numbered 01 through 16, so bit 0 is LED 01 */
   LedDriver_TurnOn( 1 << 0 );

   /* turn the first LED off.
      The LEDs are numbered 01 through 16, so bit 0 is LED 01 */
   LedDriver_TurnOff( 1 << 0 );

   /* check LED 01 is off */
   TEST_ASSERT_EQUAL_HEX16( 0, virtualLeds );
}

TEST_GROUP_RUNNER( LedDriver )
{
   /* TEST 1 */
   RUN_TEST_CASE( LedDriver, LedsOffAfterCreate );

   /* TEST 2 */
   RUN_TEST_CASE( LedDriver, TurnOnLedOne );

   /* TEST 3 */
   RUN_TEST_CASE( LedDriver, TurnOffLedOne );
}
