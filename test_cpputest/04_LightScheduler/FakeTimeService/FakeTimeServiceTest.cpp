/**
 * @file    FakeTimeServiceTest.cpp
 * @author  Julio Cesar Bernal Mendez
 * @brief   (Fake) Time Service test source file containing the test cases
 *          and test group for the (Fake) Time Service module used inside the Light Scheduler module.
 
 * @version 0.1
 * @date    2025-04-02
 */

extern "C"
{
    /* includes for things with C linkage */
    #include "FakeTimeService.h"
    #include "LightScheduler.h"
}

/* includes for things with C++ linkage */
#include "TestHarness.h"

TEST_GROUP( FakeTimeService )
{
    /* define data accessible to test group members here */
  
    void setup()
    {
        /* initialization steps are executed before each TEST */
        TimeService_Create();
    }
  
    void teardown()
    {
        /* clean up steps are executed after each TEST */
    }
};

TEST( FakeTimeService, Create )
{
    /* at the moment code reaches here, TimeService_Create() must have already being executed
       due to setup(), then both minute of day and day of the week must be unknown since they 
       have not been set */
    
    Time time;

    /* get both minute of the day and day of the week upon initialization */
    TimeService_GetTime( &time );

    /* verify both minute of the day and day of the week are unknown for what was just explained above */
    LONGS_EQUAL( TIME_UNKNOWN, time.minuteOfDay );
    LONGS_EQUAL( TIME_UNKNOWN, time.dayOfWeek );
}

TEST( FakeTimeService, Set )
{
    Time time;

    /* set 42 as the minute of the day */
    FakeTimeService_SetMinute( 42 );

    /* set Saturday as the day of the week */
    FakeTimeService_SetDay( SATURDAY );

    /* get minute of the day and day of the week */
    TimeService_GetTime( &time );

    /* verify minute of the day is 42 and day of the week is saturday */
    LONGS_EQUAL( 42, time.minuteOfDay );
    LONGS_EQUAL( SATURDAY, time.dayOfWeek );
}
