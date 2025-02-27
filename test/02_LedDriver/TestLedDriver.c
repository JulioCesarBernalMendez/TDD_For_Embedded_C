/**
 * @file    TestLedDriver.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Test led driver example source file containing the test cases and test group runner for the LedDriver module.
 *          This code structure is based on the test examples provided in the "Test-Driven Development for Embedded C"
 *          book by James W. Grenning, which use unity_fixture.h unlike the examples in https://www.throwtheswitch.org/unity
 *          which use unity.h instead.
 * 
 * @version 0.1
 * @date    2025-02-21
 */

#include "unity_fixture.h"

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
    TEST_FAIL_MESSAGE( "Start here" );
}

TEST_GROUP_RUNNER( LedDriver )
{
    RUN_TEST_CASE( LedDriver, LedsOffAfterCreate );
}
