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
#include "RuntimeErrorStub.h"

/* address of the 16 LEDs */
static uint16_t virtualLeds;

TEST_GROUP( LedDriver );

TEST_SETUP( LedDriver )
{
   /* this will be called at the beginning of
      every TEST().
      LedDriver_Create() will assign the address of virtualLeds (used here in the tests)
      as the address of ledsAddress (which is used in the LedDriver) and will initialize
      all of the LEDs to 0 (off) */
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
   /* turn the first LED (LED 1) on */
   LedDriver_TurnOn( 1 );

   /* check LED 1 is on */
   TEST_ASSERT_EQUAL_HEX16( 1, virtualLeds );
}

/* TEST 3 */
TEST( LedDriver, TurnOnMultipleLeds )
{
   /* turn on both LEDs 9 and 8 */
   LedDriver_TurnOn( 8 );
   LedDriver_TurnOn( 9 );

   /* check both LEDs 9 and 8 are on.
      The LEDs are numbered 1 through 16, so bit 8 is LED 9
      and bit 7 is LED 8, which equals 0x180 */
   TEST_ASSERT_EQUAL_HEX16( 0x180, virtualLeds );
}

/* TEST 4 */
TEST( LedDriver, AllOn )
{
   /* turn on all the 16 LEDs */
   LedDriver_TurnAllOn();

   /* check all LEDs are on */
   TEST_ASSERT_EQUAL_HEX( 0xffff, virtualLeds );
}

/* TEST 5 */
TEST( LedDriver, TurnOffLedOne )
{
   /* turn the first LED (LED 1) on */
   LedDriver_TurnOn( 1 );

   /* turn the first LED off.
      The LEDs are numbered 01 through 16, so bit 0 is LED 01 */
   LedDriver_TurnOff( 1 );

   /* check LED 1 is off */
   TEST_ASSERT_EQUAL_HEX16( 0, virtualLeds );
}

/* TEST 6 */
TEST( LedDriver, TurnOffAnyLed )
{
   /* turn on all LEDs */
   LedDriver_TurnAllOn();

   /* turn off LED 8 */
   LedDriver_TurnOff( 8 );

   /* check that only LED 8 is off */
   TEST_ASSERT_EQUAL_HEX16( 0xff7f, virtualLeds );
}

/* TEST 7 */ 
TEST( LedDriver, LedMemoryIsNotReadable )
{
   /* virtualLeds is first initialized to 0 (all LEDs off) due to
      the calling to LedDriver_Create() before each test's execution */

   /* overwrite the variable (not hardware) that stores the
      state of the LEDs */
   virtualLeds = 0xffff;

   /* turn on LED 8 */
   LedDriver_TurnOn( 8 );

   /* at this point, if the driver can read the state of the LEDs,
      then virtualLeds would be 0x80 (7 bit high, which corresponds
      to the LED 8 just turned on), overwriting the previous forced 0xffff */
   TEST_ASSERT_EQUAL_HEX16( 0x80, virtualLeds );
}

/* TEST 8 */
TEST( LedDriver, UpperAndLowerBounds )
{
   /* this test checks the upper and lower bounds of the legal LED values */

   /* turn on both LEDs 1 and 16 */
   LedDriver_TurnOn( 1 );
   LedDriver_TurnOn( 16 );

   /* check that both LEDs 1 and 16 are on */
   TEST_ASSERT_EQUAL_HEX16( 0x8001, virtualLeds );
}

/* TEST 9 */
TEST( LedDriver, OutOfBoundsTurnOnDoesNoHarm )
{
   /* this test exercises the LedDriver with some fence-post values
      and a way out-of-bounds value */
   LedDriver_TurnOn( -1 );
   LedDriver_TurnOn( 0 );
   LedDriver_TurnOn( 17 );
   LedDriver_TurnOn( 3141 );

   /* if everything was well done, then nothing should have happened
      when writing out of the legal LED values. All 16 LEDs should be off */
   TEST_ASSERT_EQUAL_HEX16( 0, virtualLeds );
}

/* TEST 10 */
TEST( LedDriver, OutOfBoundsTurnOffDoesNoHarm )
{
   /* this test exercises the LedDriver with some fence-post values
      and a way out-of-bounds value */

   /* turn on all the LEDs */
   LedDriver_TurnAllOn();

   /* attempt to turn off out-of-bounds LEDs */
   LedDriver_TurnOff( -1 );
   LedDriver_TurnOff( 0 );
   LedDriver_TurnOff( 17 );
   LedDriver_TurnOff( 3141 );

   /* if everything was well done, then nothing should have happened
      when writing out of the legal LED values, All 16 LEDs should be on */
   TEST_ASSERT_EQUAL_HEX16( 0xffff, virtualLeds );
}

/* TEST 11 */
TEST( LedDriver, OutOfBoundsProducesRuntimeError )
{
   const char *errorMessage;
   int errorParameter;

   /* attempt to turn off an out-of-bounds LED */
   LedDriver_TurnOn( -2 );

   /* get the last runtime error message and error parameter */
   errorMessage   = RuntimeErrorStub_GetLastError();
   errorParameter = RuntimeErrorStub_GetLastParameter();

   /* check the error message is "LED Driver: out-of-bounds LED" */
   TEST_ASSERT_EQUAL_STRING( "LED Driver: out-of-bounds LED", errorMessage );

   /* check the last error parameter is the same as the out-of-bounds LED number (-2).
      Note: the cast to uint16_t is necessary because LedDriver_TurnOn() receives as an argument a
            uint16_t data type, so the -2 used above is actually promoted to (uint16_t) -2 */
   TEST_ASSERT_EQUAL( ( uint16_t ) -2, errorParameter );
}

TEST_GROUP_RUNNER( LedDriver )
{
   /* TEST 1 */
   RUN_TEST_CASE( LedDriver, LedsOffAfterCreate );

   /* TEST 2 */
   RUN_TEST_CASE( LedDriver, TurnOnLedOne );

   /* TEST 3 */
   RUN_TEST_CASE( LedDriver, TurnOnMultipleLeds );

   /* TEST 4 */
   RUN_TEST_CASE( LedDriver, AllOn );

   /* TEST 5 */
   RUN_TEST_CASE( LedDriver, TurnOffLedOne );

   /* TEST 6 */
   RUN_TEST_CASE( LedDriver, TurnOffAnyLed );

   /* TEST 7 */
   RUN_TEST_CASE( LedDriver, LedMemoryIsNotReadable );

   /* TEST 8 */
   RUN_TEST_CASE( LedDriver, UpperAndLowerBounds );

   /* TEST 9 */
   RUN_TEST_CASE( LedDriver, OutOfBoundsTurnOnDoesNoHarm );

   /* TEST 10 */
   RUN_TEST_CASE( LedDriver, OutOfBoundsTurnOffDoesNoHarm );

   /* TEST 11 */
   RUN_TEST_CASE( LedDriver, OutOfBoundsProducesRuntimeError );
}
