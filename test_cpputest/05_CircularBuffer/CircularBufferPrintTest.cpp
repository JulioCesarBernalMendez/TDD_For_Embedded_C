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

        /* Get pointer to the current string buffer (it should read "") */
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
    /* This is the simplest case, printing an empty circular buffer.
       It seems too simple to test, but it is a boundary test that should not crash the system,
       and the simple case helps get the test fixture set up properly. */

    /* At this point both the string buffer (actualOutput) and circular buffer (buffer) are already created and empty (see setup()) */

    /* The expected circular buffer integer values are contained within the '<>' characters (i.e. is empty) */
    expectedOutput = "Circular buffer content:\n<>\n";

    /* "Print" the contents of the circular buffer.
    
       Here CircularBuffer_Print() uses FormatOutput(), but FormatOutput() does not point to the default printf(),
       instead it points to the FormatOutputSpy(), so the circular buffer output will be redirected to 'buffer' instead,
       like this: "Circular buffer content:\n<>\n".
       
       Similarly the circular buffer integer values are contained within the '<>' characters. 
       The characters "Circular buffer content:\n<" and ">\n" are not part of the circular buffer,
       they are part of the string buffer created with FormatOutputSpy_Create().
       
       Notes:
       - 'buffer' is the structure that holds the circular buffer parameters and values
       - 'buffer->values[]' is the actual circular buffer that stores integer values
       - the string used to print the circular buffer contents is actually a pointer to a memory
         location (dynamically) allocated by (in this case) FormatOutputSpy_Create(),
         (see static char *buffer in FormatOutputSpy.c).
         
       Production code uses printf() instead of FormatOutputSpy() to print the circular buffer contents */
    CircularBuffer_Print( buffer );

    /* compare the expected results */
    STRCMP_EQUAL( expectedOutput, actualOutput );
}

TEST( CircularBufferPrint, PrintAfterOneIsPut )
{
    /* This test is another boundary test. It checks that a circular buffer containing a single item
       prints properly */

    /* At this point both the string buffer (actualOutput) and circular buffer (buffer) are already created and empty (see setup()) */

    /* The expected circular buffer integer values are contained within the '<>' characters (i.e. 17) */
    expectedOutput = "Circular buffer content:\n<17>\n";

    /* insert the value 17 into the circular buffer */
    CircularBuffer_Put( buffer, 17 );

    /* "Print" the contents of the circular buffer.
    
       Here CircularBuffer_Print() uses FormatOutput(), but FormatOutput() does not point to the default printf(),
       instead it points to the FormatOutputSpy(), so the circular buffer output will be redirected to 'buffer' instead,
       like this: "Circular buffer content:\n<>\n".
       
       Similarly the circular buffer integer values are contained within the '<>' characters. 
       The characters "Circular buffer content:\n<" and ">\n" are not part of the circular buffer,
       they are part of the string buffer created with FormatOutputSpy_Create().
       
       Notes:
       - 'buffer' is the structure that holds the circular buffer parameters and values
       - 'buffer->values[]' is the actual circular buffer that stores integer values
       - the string used to print the circular buffer contents is actually a pointer to a memory
         location (dynamically) allocated by (in this case) FormatOutputSpy_Create(),
         (see static char *buffer in FormatOutputSpy.c).
         
       Production code uses printf() instead of FormatOutputSpy() to print the circular buffer contents */
    CircularBuffer_Print( buffer );

    /* compare the expected results */
    STRCMP_EQUAL( expectedOutput, actualOutput );
}

TEST( CircularBufferPrint, PrintNotYetWrappedOrFull )
{
    /* This test handles the case when there are a few items in the buffer,
       but it is not full and has not wrapped around to the first slot */

    /* At this point both the string buffer (actualOutput) and circular buffer (buffer) are already created and empty (see setup()) */

    /* The expected circular buffer integer values are contained within the '<>' characters (i.e. 10 20 and 30) */
    expectedOutput = "Circular buffer content:\n<10, 20, 30>\n";

    /* insert the values 10, 20 and 30 into the circular buffer */
    CircularBuffer_Put( buffer, 10 );
    CircularBuffer_Put( buffer, 20 );
    CircularBuffer_Put( buffer, 30 );

    /* "Print" the contents of the circular buffer.
    
       Here CircularBuffer_Print() uses FormatOutput(), but FormatOutput() does not point to the default printf(),
       instead it points to the FormatOutputSpy(), so the circular buffer output will be redirected to 'buffer' instead,
       like this: "Circular buffer content:\n<>\n".
       
       Similarly the circular buffer integer values are contained within the '<>' characters. 
       The characters "Circular buffer content:\n<" and ">\n" are not part of the circular buffer,
       they are part of the string buffer created with FormatOutputSpy_Create().
       
       Notes:
       - 'buffer' is the structure that holds the circular buffer parameters and values
       - 'buffer->values[]' is the actual circular buffer that stores integer values
       - the string used to print the circular buffer contents is actually a pointer to a memory
         location (dynamically) allocated by (in this case) FormatOutputSpy_Create(),
         (see static char *buffer in FormatOutputSpy.c).
         
       Production code uses printf() instead of FormatOutputSpy() to print the circular buffer contents */
    CircularBuffer_Print( buffer );

    /* compare the expected results */
    STRCMP_EQUAL( expectedOutput, actualOutput );
}

TEST( CircularBufferPrint, PrintNotYetWrappedAndIsFull )
{
    /* Here is the test for another boundary case, the buffer is completely full,
       created with a capacity of five, but has not yet wrapped back around to the first location */

    /* Create a circular buffer with a capacity of five */
    CircularBuffer b = CircularBuffer_Create( 5 );

    /* At this point both the string buffer (actualOutput) and the circular buffer with a capacity of 5 (b)
       are already created and empty (see setup()) */

    /* The expected circular buffer integer values are contained within the '<>' characters (i.e. 31 41 59 26 and 53) */
    expectedOutput = "Circular buffer content:\n<31, 41, 59, 26, 53>\n";

    /* insert the values 31, 41, 59, 26 and 53 into the circular buffer */
    CircularBuffer_Put( b, 31 );
    CircularBuffer_Put( b, 41 );
    CircularBuffer_Put( b, 59 );
    CircularBuffer_Put( b, 26 );
    CircularBuffer_Put( b, 53 );

    /* "Print" the contents of the circular buffer.
    
       Here CircularBuffer_Print() uses FormatOutput(), but FormatOutput() does not point to the default printf(),
       instead it points to the FormatOutputSpy(), so the circular buffer output will be redirected to 'b' instead,
       like this: "Circular buffer content:\n<>\n".
       
       Similarly the circular buffer integer values are contained within the '<>' characters. 
       The characters "Circular buffer content:\n<" and ">\n" are not part of the circular buffer,
       they are part of the string buffer created with FormatOutputSpy_Create().
       
       Notes:
       - 'b' is the structure that holds the circular buffer parameters and values
       - 'b->values[]' is the actual circular buffer that stores integer values
       - the string used to print the circular buffer contents is actually a pointer to a memory
         location (dynamically) allocated by (in this case) FormatOutputSpy_Create(),
         (see static char *buffer in FormatOutputSpy.c).
         
       Production code uses printf() instead of FormatOutputSpy() to print the circular buffer contents */
    CircularBuffer_Print( b );

    /* compare the expected results */
    STRCMP_EQUAL( expectedOutput, actualOutput );

    /* Deallocate the circular buffer */
    CircularBuffer_Destroy( b );
}

TEST( CircularBufferPrint, PrintOldToNewWhenWrappedAndFull )
{
    /* This test deals with the situation after wrap-around has occurred */

    /* Create a circular buffer with a capacity of five */
    CircularBuffer b = CircularBuffer_Create( 5 );

    /* At this point both the string buffer (actualOutput) and the circular buffer with a capacity of 5 (b)
       are already created and empty (see setup()) */

    /* The expected circular buffer integer values are contained within the '<>' characters (i.e. 201 202 203 204 and 999) */
    expectedOutput = "Circular buffer content:\n<201, 202, 203, 204, 999>\n";

    /* insert the values 200, 201, 202, 203 and 204 into the circular buffer */
    CircularBuffer_Put( b, 200 );
    CircularBuffer_Put( b, 201 );
    CircularBuffer_Put( b, 202 );
    CircularBuffer_Put( b, 203 );
    CircularBuffer_Put( b, 204 );

    /* The circular buffer is already full at this point: 200, 201, 202, 203 and 204.
       No more values can be written to it */

    /* Get a value from the circular buffer.
       
       The circular buffer is a FIFO structure, the oldest value introduced is the first value to read.
       in this case the value obtained is 200.
       
       After this operation the circular buffer will have an empty space (at the position of the 200 value just read).

       Note: Actually the value 200 still exists in memory (this can be confirmed by reading b->values[0]).
             What happens is that the outdex (position at next value to read) "pointer" of the circular buffer points
             to the next position (i.e. b->outdex++). But since the number of elements is reduced by one (i.e. b->count--),
             when the circular buffer values are printed with CircularBuffer_Print() only "b->count" values will be read
             starting at b->outdex++ (i.e. the value at the position (b->values[outdex]) just obtained by CircularBuffer_Get()
             will be ignored as it is not part of the circular buffer anymore) */
    CircularBuffer_Get( b );

    /* Insert one value into the buffer.
    
       Earlier the circular buffer was:
       200, 201, 202, 203 and 204 (count = 5, i.e. buffer full)
       |-> outdex (next position to read from)
       |-> index (next position to instert at).
                                       
       Then one element was read (200), the circular buffer is (notice is the same, what changed were outdex and count):
       200, 201, 202, 203 and 204 (count = 4, i.e. buffer has one empty slot)
       |     |-> outdex (next position to read from)
       |-> index (next position to insert at).
       
       This means that this value (999) insterted here will replace the value (200) just obtained by CircularBuffer_Get():
       999, 201, 202, 203 and 204 (count = 5, i.e. buffer full)
            |-> outdex
            |-> index  */
    CircularBuffer_Put( b, 999 );

    /* Note: at this point the circular buffer has the next elements in order,
             201, 202, 203, 204 and 999
             
             Why not 999, 201, 202, 203 and 204 as shown in the comment above?
             Because the circular buffer is a FIFO structure, it will start reading from 'b->index' (i.e. value 201)
             'count' values (i.e. 5 values) like this: 201, 202, 203, 204 and 999 */

    /* "Print" the contents of the circular buffer.
    
       Here CircularBuffer_Print() uses FormatOutput(), but FormatOutput() does not point to the default printf(),
       instead it points to the FormatOutputSpy(), so the circular buffer output will be redirected to 'b' instead,
       like this: "Circular buffer content:\n<>\n".
       
       Similarly the circular buffer integer values are contained within the '<>' characters. 
       The characters "Circular buffer content:\n<" and ">\n" are not part of the circular buffer,
       they are part of the string buffer created with FormatOutputSpy_Create().
       
       Notes:
       - 'b' is the structure that holds the circular buffer parameters and values
       - 'b->values[]' is the actual circular buffer that stores integer values
       - the string used to print the circular buffer contents is actually a pointer to a memory
         location (dynamically) allocated by (in this case) FormatOutputSpy_Create(),
         (see static char *buffer in FormatOutputSpy.c).
         
       Production code uses printf() instead of FormatOutputSpy() to print the circular buffer contents */
    CircularBuffer_Print( b );

    /* compare the expected results */
    STRCMP_EQUAL( expectedOutput, actualOutput );

    /* Deallocate the circular buffer */
    CircularBuffer_Destroy( b );
}
