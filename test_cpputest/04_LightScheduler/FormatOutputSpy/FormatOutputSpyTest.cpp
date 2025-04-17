/**
 * @file    FormatOutputSpyTest.cpp
 * @author  Julio Cesar Bernal Mendez
 * @brief   Format Output Spy test file
 * 
 * @version 0.1
 * @date    2025-04-14
 */

extern "C"
{
    /* includes for things with C linkage */
    #include "Utils.h"
    #include "FormatOutputSpy.h"
}
 
/* includes for things with C++ linkage */
#include "TestHarness.h"
 
TEST_GROUP( FormatOutputSpy )
{
    /* define data accessible to test group members here */
       
    void setup()
    {
        /* initialization steps are executed before each TEST */

        /* Update FormatOutput (which points to printf() by default) to point to FormatOutputSpy instead,
           for testing purposes.
           
           When a function pointer is set with UT_PTR_SET() during setup() or during any TEST(),
           it's automatically restored after teardown() completes */
        UT_PTR_SET( FormatOutput, FormatOutputSpy );
    }
       
    void teardown()
    {
        /* clean up steps are executed after each TEST */
        FormatOutputSpy_Destroy();
    }
};

TEST( FormatOutputSpy, HelloWorld )
{
    /* Allocate (dynamically) space to hold a buffer of 20 bytes 
       (i.e. 20 characters plus one extra character for null termination).
       This will update the buffer in FormatOutputSpy.c */
    FormatOutputSpy_Create( 20 );

    /* Write "Hello, World\n" as if it was the real FormatOutput() which uses printf().
       Here the FormatOutputSpy() will catch that and redirect it into the buffer in FormatOutputSpy.c */
    FormatOutput( "Hello, World\n" );

    /* compare the buffer with the expected result */
    STRCMP_EQUAL( "Hello, World\n", FormatOutputSpy_GetOutput() );
}

TEST( FormatOutputSpy, LimitTheOutputBufferSize )
{
    /* This test illustrates that the spy only captures the number of characters
       specified in FormatOutputSpy_Create() */
    
    /* Allocate (dynamically) space to hold a buffer of 4 bytes
       (i.e. 4 characters plus one extra character for null termination).
       This will update the buffer in FormatOutputSpy.c */
    FormatOutputSpy_Create( 4 );

    /* Write "Hello, World\n" as if it was the real FormatOutput() which uses printf().
       Here the FormatOutputSpy() will catch that and redirect it into the buffer in FormatOutputSpy.c.
       
       Since the buffer has a size of 4 (plus one extra character for null termination), then only 
       "Hell" was actually written */
    FormatOutput( "Hello, World\n" );

    /* compare the buffer with the expected result */
    STRCMP_EQUAL( "Hell", FormatOutputSpy_GetOutput() );
}
