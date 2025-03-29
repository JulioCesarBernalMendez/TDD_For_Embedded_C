/**
 * @file    TestDummyDriver.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Dummy Driver example source file containing the test cases and test group for the DummyDriver module.
 * 
 * @version 0.1
 * @date    2025-03-28
 */

extern "C"
{
    /* includes for things with C linkage */
    #include "../../include/03_DummyDriver/DummyDriver.h"
}

/* includes for things with C++ linkage */
#include "../../cpputest/include/CppUTest/TestHarness.h"

TEST_GROUP( DummyDriver )
{
    /* define data accessible to test group members here */

    void setup()
    {
        /* initialization steps are executed before each TEST */ 
    }

    void teardown()
    {
        /* clean up steps are executed after each TEST */
    }
};

TEST( DummyDriver, FirstTest )
{
    CHECK_EQUAL( 30, AddTwoBytes( 20, 10 ) );
    CHECK_EQUAL( -1, AddTwoBytes( 3, -4 ) );
    CHECK_EQUAL( 255, AddTwoBytes( 0, 255 ) );
    CHECK_EQUAL( 0, AddTwoBytes( 0, 0 ) );
}

TEST( DummyDriver, SecondTest )
{
    CHECK_EQUAL( 0xFF, 0xFF );
    FAIL( "Intentional Fail, Here!" );
}
