/**
 * @file    CircularBufferPrintTest.cpp
 * @author  Julio Cesar Bernal Mendez
 * @brief   Circular Buffer Print test file
 * 
 * @version 0.1
 * @date    2025-04-18
 */

extern "C"
{
    /* includes for things with C linkage */
    #include "CircularBuffer.h"
    #include "Utils.h" /* FormatOutput() */
    #include "FormatOutputSpy.h" /* FormatOutputSpy() */
}
  
/* includes for things with C++ linkage */
#include "TestHarness.h"
  
TEST_GROUP( CircularBufferPrint )
{
    /* define data accessible to test group members here */

    CircularBuffer buffer;      /* circular buffer */
    const char *expectedOutput; /* expected circular buffer output */
    const char *actualOutput;   /* current circular buffer output */

    void setup()
    {
        /* initialization steps are executed before each TEST */

        /* Update FormatOutput (which points to printf() by default) to point to FormatOutputSpy instead,
           for testing purposes.
           
           When a function pointer is set with UT_PTR_SET() during setup() or during any TEST(),
           it's automatically restored after teardown() completes */
        UT_PTR_SET( FormatOutput, FormatOutputSpy );

        /* Allocate (dynamically) space to hold a buffer of 100 bytes 
           (i.e. 100 characters plus one extra character for null termination).
           This will update the buffer in FormatOutputSpy.c.
           
           This string buffer will be used to store the output of the circuler buffer */
        FormatOutputSpy_Create( 100 );

        /* Allocate (dynamically) space to hold a buffer of 10 integer values */
        buffer = CircularBuffer_Create( 10 );

        /* Get current string buffer (it should read "") */
        actualOutput = FormatOutputSpy_GetOutput();
    }

    void teardown()
    {
        /* clean up steps are executed after each TEST */

        /* Deallocate both the string buffer and the circular buffer */
        FormatOutputSpy_Destroy();
        CircularBuffer_Destroy( buffer );
    }
};
 
TEST( CircularBufferPrint, PrintEmpty )
{
    /* At this point both the string buffer and circular buffer are already created and empty */

    /* The expected circular buffer integer values are contained within the '<>' characters (i.e. is empty) */
    expectedOutput = "Circular buffer content:\n<>\n";

    /* "Print" the contents of the circular buffer.
    
       Here CircularBuffer_Print() uses FormatOutput(), but FormatOutput() does not point to the default printf(),
       instead it points to the FormatOutputSpy(), so the circular buffer output will be redirected to 'buffer' instead
       like this: "Circular buffer content:\n<>\n".
       
       Similarly the circular buffer integer values are contained within the '<>' characters. 
       The characters "Circular buffer content:\n<" and ">\n" are not part of the circular buffer,
       they are part of the string buffer created with FormatOutputSpy_Create() */
    CircularBuffer_Print( buffer );

    /* compare the expected results */
    STRCMP_EQUAL( expectedOutput, actualOutput );
}
