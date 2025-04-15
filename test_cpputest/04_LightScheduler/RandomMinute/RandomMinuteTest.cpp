/**
 * @file    RandomMinuteTest.cpp
 * @author  Julio Cesar Bernal Mendez
 * @brief   Random Minute test source file containing the test cases
 *          and test group for the Random Minute module used inside the Light Scheduler module.
 
 * @version 0.1
 * @date    2025-04-11
 */

extern "C"
{
    /* includes for things with C linkage */
    #include <stdio.h>  /* includes printf() function prototype */
    #include <string.h> /* includes memset() function prototype */
    #include "RandomMinute.h" 
}
 
/* includes for things with C++ linkage */
#include "TestHarness.h"

enum { BOUND = 30 }; /* range of possible random values, in this case -30 to +30 */

TEST_GROUP( RandomMinute )
{
    /* define data accessible to test group members here */

    int minute; /* random minute used in multiple tests */
   
    void setup()
    {
        /* initialization steps are executed before each TEST */

        /* initialize the Random Minute module to generate random values between -BOUND and +BOUND  */
        RandomMinute_Create( BOUND );

        /* Use a constant seed for rand() every time a TEST() executes.
           This keeps the random number generator from potentially causing random failures.
           
           This is done after RandomMinute_Create() to undo any seeding done in RandomMinute_Create() */
        srand( 1 );
    }
   
    void teardown()
    {
        /* clean up steps are executed after each TEST */
    }

    /* helper function used in multiple tests */
    void AssertMinuteIsInRange()
    {
        /* if minute is out of bounds */
        if ( ( minute < -BOUND ) || ( minute > BOUND ) )
        {
            /* print the out of range minute and fail the test */
            printf( "bad minute value: %d\n", minute );
            FAIL( "Minute out of range" );
        }
    }
};

TEST( RandomMinute, GetIsInRange )
{
    /* this test makes sure that no random generated values
       are outside the specified range */

    int i; /* counter */

    /* repeat 100 times */
    for ( i = 0; i < 100; i++ )
    {
        /* get a random value between -BOUND and +BOUND */
        minute = RandomMinute_Get();

        /* check that minute is not out of range */
        AssertMinuteIsInRange();
    }
}

TEST( RandomMinute, AllValuesPossible )
{
    /* array long enough to hold all possible random values (between -BOUND to +BOUND) */
    int hit[ 2* BOUND + 1 ];

    /* counter */
    int i;

    /* initialize the hit array to 0 */
    memset( hit, 0, sizeof( hit ) );

    /* Repeat 225 times.
       This value of 225 came after a little experimentation. It ensures that every
       possible random value (between -BOUND to +BOUND) is obtained at least once */ 
    for ( i = 0; i < 225; i++ )
    {
        /* get a random minute between -BOUND to +BOUND */
        minute = RandomMinute_Get();

        /* check that minute is not out of range */
        AssertMinuteIsInRange();

        /* Every number between -BOUND to +BOUND has a counter in the hit array.
           This line of code increases the respective random number counter by one.
           "+ BOUND" here is an offset so that the lowest random value -BOUND is assigned index 0 in the array */
        ++hit[ minute + BOUND ];
    }

    /* for all the elements in the array */
    for ( i = 0; i < ( 2 * BOUND + 1 ); i++ )
    {
        /* check that the respective counter number is greater than 0
          (i.e. every possible random number has been obtained at least once) */
        CHECK( hit[ i ] > 0 );
    }
}
