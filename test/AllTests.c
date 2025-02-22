/**
 * @file    AllTests.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Source file which contains main() and runs all the test group runners specified within RunAllTests().
 * 
 *          This code structure is based on the test examples provided in the "Test-Driven Development for Embedded C"
 *          book by James W. Grenning, which use unity_fixture.h instead of the examples in https://www.throwtheswitch.org/unity
 *          which use unity.h instead.
 * 
 * @version 0.1
 * @date    2025-02-21
 */

#include "unity_fixture.h"

static void RunAllTests( void )
{
    RUN_TEST_GROUP( DumbExample );
}

int main( int argc, const char **argv )
{
    return UnityMain( argc, argv, RunAllTests );
}
