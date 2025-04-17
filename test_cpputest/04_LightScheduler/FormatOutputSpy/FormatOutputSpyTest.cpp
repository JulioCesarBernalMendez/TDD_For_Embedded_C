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

TEST( FormatOutputSpy, PrintMultipleTimes )
{
    /* Like the real FormatOutput, the spy can be called multiple times.
       This test shows that the spy appends characters with each FormatOutput call */

    /* Allocate (dynamically) space to hold a buffer of 25 bytes
       (i.e. 25 characters plus one extra character for null termination).
       This will update the buffer in FormatOutputSpy.c */
    FormatOutputSpy_Create( 25 );

    /* Write "Hello" as if it was the real FormatOutput() which uses printf().
       Here the FormatOutputSpy() will catch that and redirect it into the buffer in FormatOutputSpy.c.
       
       Since the buffer is 25 bytes long and 5 bytes are written here, it has enough space */
    FormatOutput( "Hello" );

    /* Write ", World\n" as if it was the real FormatOutput() which uses printf().
       Here the FormatOutputSpy() will catch that and append it into the buffer in FormatOutputSpy.c.
       
       Since the buffer is 25 bytes long and 5 bytes were written before, it has 20 bytes available,
       which is enough to store these other 8 characters */
    FormatOutput( ", World\n" );

    /* compare the buffer with the expected result */
    STRCMP_EQUAL( "Hello, World\n", FormatOutputSpy_GetOutput() );
}

TEST( FormatOutputSpy, PrintMultipleOutputsPastFull )
{
    /* In this test, the spy is called multiple times with more output than the spy can capture.
       This test assures that the output captured is limited to the specified maximum string length */

    /* Allocate (dynamically) space to hold a buffer of 12 bytes
       (i.e. 12 characters plus one extra character for null termination).
       This will update the buffer in FormatOutputSpy.c */
    FormatOutputSpy_Create( 12 );

    /* Write "12345" as if it was the real FormatOutput() which uses printf().
       Here the FormatOutputSpy() will catch that and redirect it into the buffer in FormatOutputSpy.c.
       
       Since the buffer is 12 bytes long and 5 bytes are written here, it has enough space */
    FormatOutput( "12345" );

    /* Write "67890" as if it was the real FormatOutput() which uses printf().
       Here the FormatOutputSpy() will catch that and append it into the buffer in FormatOutputSpy.c.
       
       Since the buffer is 12 bytes long and 5 bytes were written before, it has 7 bytes available,
       which is enough to store these other 5 characters */
    FormatOutput( "67890" );

    /* Write "ABCDEF" as if it was the real FormatOutput() which uses printf().
       Here the FormatOutputSpy() will catch that and append it into the buffer in FormatOutputSpy.c.
       
       Since the buffer is 12 bytes long and up to this point 10 bytes have been written,
       it only has 2 more bytes available for "AB", not enough space to store the remaining 4 characters (CDEF) */
    FormatOutput( "ABCDEF" );

    /* compare the buffer with the expected result */
    STRCMP_EQUAL( "1234567890AB", FormatOutputSpy_GetOutput() );
}
