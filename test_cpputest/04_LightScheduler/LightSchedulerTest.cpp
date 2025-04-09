/**
 * @file    LightSchedulerTest.cpp
 * @author  Julio Cesar Bernal Mendez
 * @brief   Light Scheduler test source file containing the test cases
 *          and test group for the Light Scheduler module.
 *          Light Scheduler depends on two other modules:
 *          - Light Controller
 *          - and Time Service
 * 
 *          The premise is:
 *          - we are developing a module 'Light Scheduler' that depends on 2 other modules
 *          - these other 2 modules are: a Hardware Dependent Code 'Light Controller' and
 *            an Operating System dependent code (Time Service)
 *          - we don't have the Hardware available yet, and we haven't chosen the OS either ...
 *            ... should this stop us from developing the Light Scheduler? HELL NO!, we have TDD
 *          - OK, then how to develop then?
 *            we can treat those 2 areas (Light Controller and Time Service) as DOCs (Dependent On Components)
 *            and define interfaces that meet the CUT (Code Under Test) needs
 *          - these interfaces will allow us to break dependencies on unknowns
 *          - we develop right up to the unknown, defining what we want from the interfaces.
 *            The tests will help us drive the design!
 * 
 * @version 0.1
 * @date    2025-03-31
 */

extern "C"
{
    /* includes for things with C linkage */
    #include "LightScheduler.h"
    #include "LightControllerSpy.h"
    #include "FakeTimeService.h"
}

/* includes for things with C++ linkage */
#include "TestHarness.h"

/* Repeated operations and checks can be extracted into helper functions,
   leaving the tests easier to read */
void setTimeTo( int day, int minuteOfDay )
{
    /* The test takes control of the clock, telling the Fake Time Source (Fake Time Service) that it
       should report that it's "day" at "minuteOfDay" */
    FakeTimeService_SetDay( day );
    FakeTimeService_SetMinute( minuteOfDay );
}

/* Repeated operations and checks can be extracted into helper functions,
   leaving the tests easier to read */
void checkLightState( int id, int state )
{
    /* This test helper now is modified to use LightControllerSpy_GetLightState(),
       this change allows checkLightState() to work in the multiple event case */

    /* the test checks the expected outcome (the time for the scheduled light ID has been reached,
       then light ID should be "id" and state should be "state") */
    if ( id == LIGHT_ID_UNKNOWN )
    {
        LONGS_EQUAL( id, LightControllerSpy_GetLastId() );
        LONGS_EQUAL( state, LightControllerSpy_GetLastState() );
    }
    else
    {
        LONGS_EQUAL( state, LightControllerSpy_GetLightState( id ) );
    }
}


/****************************** LightSchedulerInitAndCleanup TEST GROUP ******************************/
/* Use a new test group.

   The initialization and cleanup tests have very different needs from the other LightScheduler tests.
   The TEST_GROUP() has nothing in it because each test is self-contained.
   There's no duplication to get rid of */
TEST_GROUP( LightSchedulerInitAndCleanup )
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

TEST( LightSchedulerInitAndCleanup, CreateStartsOneMinuteAlarm )
{
    /* This test assures the system is wired properly.

       To test the registration, (Fake) Time Service needs to spy on the call to
       TimeService_SetPeriodicAlarmInSeconds(), which is called during the Light Scheduler
       initialization in LightScheduler_Create(). This lets the test check that the proper
       callback was set */

    /* Initialize the Light Scheduler.
       Upon initialization:
       - there will be no scheduled lights to be turned on/off
       - LightScheduler_Wakeup() will be registered as the alarm callback to be "called" every minute */
    LightScheduler_Create();

    /* check the expected outcome, the registered alarm callback is LightScheduler_Wakeup()
       with a one minute period */
    POINTERS_EQUAL( ( void* ) LightScheduler_Wakeup, ( void* ) FakeTimeService_GetAlarmCallback() );
    LONGS_EQUAL( 60, FakeTimeSource_GetAlarmPeriodInSeconds() );
}

TEST( LightSchedulerInitAndCleanup, DestroyCancelsOneMinuteAlarm )
{
    /* This test assures LightScheduler_Destroy() cancels the periodic alarm
       registered during the Light Scheduler initialization with LightScheduler_Create() */
    LightScheduler_Create();

    /* "destroy" the periodic alarm */
    LightScheduler_Destroy();

    /* check the expected outcome, there's no registered alarm callback and period is 0 */
    POINTERS_EQUAL( NULL, ( void* ) FakeTimeService_GetAlarmCallback() );
    LONGS_EQUAL( 0, FakeTimeSource_GetAlarmPeriodInSeconds() );
}


/****************************** LightScheduler TEST GROUP ******************************/
TEST_GROUP( LightScheduler )
{
    /* define data accessible to test group members here */
 
    void setup()
    {
        /* initialization steps are executed before each TEST */ 
        LightScheduler_Create();
        LightController_Create();
    }
 
    void teardown()
    {
        /* clean up steps are executed after each TEST */
        LightScheduler_Destroy();
    }
};

TEST( LightScheduler, NoChangeToLightsDuringInitialization )
{
    /* A natural first test is to specify what should happen during initialization
       of the Light Scheduler:
       - There are no scheduled events due, so Light Controller functions should not be called
       - The spy is interrogated after the "covert mission" by checking its "secret" test-only
         interface made up of LightControllerSpy_GetLastId() and LightControllerSpy_GetLastState()
       - LightControllerSpy_GetLastId(): returns the ID of the light that was controlled
                                         or LIGHT_ID_UNKNOWN where there has been no light control.
       - LightControllerSpy_GetLastState(): returns LIGHT_OFF, LIGHT_ON or LIGHT_STATE_UNKNOWN
                                            LIGHT_STATE_UNKNOWN means the light has not been changed since initialization.
       - if the mission is "successful", the interrogation should show that no light instructions were given.

       The test checks the expected outcome */
    checkLightState( LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN );
}

TEST( LightScheduler, NoScheduleNothingHappens )
{
    /* The fully implemented Light Scheduler will have to manage a collection of scheduled items.
       Starting with a test case that involves many scheduled events makes for too much code.
       
       A good way to attack this problem is to start with the cases of no scheduled items,
       then one scheduled item, saving the many items case for later.

       This is the no scheduled items tests. 
       
       The "do nothing" tests provide the shortest path to a passing test, all that is needed
       is the interface definitions for the production code calls.
       
       Don't worry that it seems nothing is being tested. The objective here is to get these
       boundary tests right. Later when the full implementation is in place, these tests will
       continue to assure correct behavior of these boundary cases */

    /* set 100 (1:40 am) as the minute of the day and Monday as the day of the week */
    setTimeTo( MONDAY, 100 );

    /* The test simulates a callback to LightScheduler_Wakeup(), like the production TimeService
       would do every minute */
    LightScheduler_Wakeup();

    /* Since no light ID and state have been scheduled, the expected light ID and state
       have to be unknown due to the Light Scheduler's initialization (i.e. Light Controller's
       initialization) in setup() */
    checkLightState( LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN );
}

TEST( LightScheduler, ScheduleOnEverydayNotTimeYet )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets both the last scheduled light ID
       and last scheduled light state as unknowns) */

    /* The test schedules the light with ID 3 to turn ON everyday at the 1200th minute
       (i.e. 1200mins / (60mins / 1hr) = 20hrs = 8pm) */
    LightScheduler_ScheduleTurnOn( 3, EVERYDAY, 1200 );

    /* The test takes control of the clock, telling the Fake Time Source (Fake Time Service) that it
       should report that it's MONDAY at 7:59pm (one minute before the scheduled time above for the
       light ID 3 to turn on everyday) */
    setTimeTo( MONDAY, 1199 );

    /* The test simulates a callback to LightScheduler_Wakeup(), like the production TimeService
       would do every minute */
    LightScheduler_Wakeup();

    /* Finally the test checks the expected outcome (since the time for the scheduled light ID has
       not been reached, then both light ID and state are still unknown, due to the initial state
       of the Light Scheduler) */
    checkLightState( LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN );
}

TEST( LightScheduler, ScheduleOnEverydayItsTime )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets both the last scheduled light ID
       and last scheduled light state as unknowns) */

    /* The test schedules the light with ID 3 to turn ON everyday at the 1200th minute
       (i.e. 1200mins / (60mins / 1hr) = 20hrs = 8pm) */
    LightScheduler_ScheduleTurnOn( 3, EVERYDAY, 1200 );

    /* The test takes control of the clock, telling the Fake Time Source (Fake Time Service) that it
       should report that it's MONDAY at 8:00pm (the exact time as scheduled above for the
       light ID 3 to turn on everyday) */
    setTimeTo( MONDAY, 1200 );

    /* The test simulates a callback to LightScheduler_Wakeup(), like the production TimeService
       would do every minute */
    LightScheduler_Wakeup();
 
    /* Finally the test checks the expected outcome (the time for the scheduled light ID has been reached,
       then light ID should be 3 and state should be ON)  */
    checkLightState( 3, LIGHT_ON );
}

TEST( LightScheduler, ScheduleOffEverydayItsTime )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets both the last scheduled light ID
       and last scheduled light state as unknowns) */

    /* The test schedules the light with ID 3 to turn OFF everyday at the 1200th minute
       (i.e. 1200mins / (60mins / 1hr) = 20hrs = 8pm) */
    LightScheduler_ScheduleTurnOff( 3, EVERYDAY, 1200 );

    /* The test takes control of the clock, telling the Fake Time Source (Fake Time Service) that it
       should report that it's MONDAY at 8:00pm (the exact time as scheduled above for the
       light ID 3 to turn off everyday) */
    setTimeTo( MONDAY, 1200 );

    /* The test simulates a callback to LightScheduler_Wakeup(), like the production TimeService
       would do every minute */
    LightScheduler_Wakeup();

    /* Finally the test checks the expected outcome (the time for the scheduled light ID has been reached,
       then light ID should be 3 and state should be OFF)  */
    checkLightState( 3, LIGHT_OFF );
}

TEST( LightScheduler, ScheduleWeekendItsMonday )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets both the last scheduled light ID
       and last scheduled light state as unknowns) */

    /* schedule light with ID 3 to turn on on weekends (saturdays and sundays) at minute 1200 (8pm)*/
    LightScheduler_ScheduleTurnOn( 3, WEEKEND, 1200 );

    /* set the current (fake) time to monday 8pm */
    setTimeTo( MONDAY, 1200 );

    /* callback to Light Scheduler wakeup (this compares the current time to any scheduled events,
       if there's a match then it will turn on or off the specified light ID) */
    LightScheduler_Wakeup();

    /* compare the light ID and its state, they should be unknowns because the scheduled event
       has not been reached (scheduled a light on on weekends for 8pm, but its currently monday 8pm,
       light ID and light state keep their initial states) */
    checkLightState( LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN );
}

TEST( LightScheduler, ScheduleTuesdayButItsMonday )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets both the last scheduled light ID
       and last scheduled light state as unknowns) */

    /* schedule light with ID 3 to turn on on TUESDAY at minute 1200 (8pm)*/
    LightScheduler_ScheduleTurnOn( 3, TUESDAY, 1200 );

    /* set the current (fake) time to monday 8pm */
    setTimeTo( MONDAY, 1200 );

    /* callback to Light Scheduler wakeup (this compares the current time to any scheduled events,
       if there's a match then it will turn on or off the specified light ID) */
    LightScheduler_Wakeup();

    /* compare the light ID and its state, they should be unknowns because the scheduled event
       has not been reached (scheduled a light on on tuesdays for 8pm, but its currently monday 8pm,
       light ID and light state keep their initial states) */
    checkLightState( LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN );
}

TEST( LightScheduler, ScheduleTuesdayAndItsTuesday )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets both the last scheduled light ID
       and last scheduled light state as unknowns) */

    /* schedule light with ID 3 to turn on on TUESDAY at minute 1200 (8pm)*/
    LightScheduler_ScheduleTurnOn( 3, TUESDAY, 1200 );

    /* set the current (fake) time to TUESDAY 8pm */
    setTimeTo( TUESDAY, 1200 );

    /* callback to Light Scheduler wakeup (this compares the current time to any scheduled events,
       if there's a match then it will turn on or off the specified light ID) */
    LightScheduler_Wakeup();

    /* compare the light ID and its state, they should be light ID 3 and light state on because the
       scheduled event has been reached (scheduled light ID 3 on on tuesdays for 8pm. Currently it's tuesday 8pm,
       therefore light ID and light state must have been changed) */
    checkLightState( 3, LIGHT_ON );
}

TEST( LightScheduler, ScheduleWeekendItsFriday )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets both the last scheduled light ID
       and last scheduled light state as unknowns) */

    /* schedule light with ID 3 to turn on on weekends (saturdays and sundays) at minute 1200 (8pm)*/
    LightScheduler_ScheduleTurnOn( 3, WEEKEND, 1200 );

    /* set the current (fake) time to friday 8pm */
    setTimeTo( FRIDAY, 1200 );

    /* callback to Light Scheduler wakeup (this compares the current time to any scheduled events,
       if there's a match then it will turn on or off the specified light ID) */
    LightScheduler_Wakeup();

    /* compare the light ID and its state, they should be unknowns because the scheduled event
       has not been reached (scheduled a light on on weekends, i.e. saturdays and sundays for 8pm,
       but its currently friday 8pm, light ID and light state keep their initial states) */
    checkLightState( LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN );
}

TEST( LightScheduler, ScheduleWeekendItsSaturday )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets both the last scheduled light ID
       and last scheduled light state as unknowns) */

    /* schedule light with ID 3 to turn on on weekends (saturdays and sundays) at minute 1200 (8pm)*/
    LightScheduler_ScheduleTurnOn( 3, WEEKEND, 1200 );

    /* set the current (fake) time to saturday 8pm */
    setTimeTo( SATURDAY, 1200 );

    /* callback to Light Scheduler wakeup (this compares the current time to any scheduled events,
       if there's a match then it will turn on or off the specified light ID) */
    LightScheduler_Wakeup();

    /* compare the light ID and its state, they should be light ID 3 and light state on because the
       scheduled event has been reached (scheduled light ID 3 on on weekends (saturdays and sundays) for 8pm.
       Currently it's saturday 8pm, therefore light ID and light state must have been changed) */
    checkLightState( 3, LIGHT_ON );
}

TEST( LightScheduler, ScheduleWeekendItsSunday )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets both the last scheduled light ID
       and last scheduled light state as unknowns) */

    /* schedule light with ID 3 to turn on on weekends (saturdays and sundays) at minute 1200 (8pm)*/
    LightScheduler_ScheduleTurnOn( 3, WEEKEND, 1200 );

    /* set the current (fake) time to sunday 8pm */
    setTimeTo( SUNDAY, 1200 );

    /* callback to Light Scheduler wakeup (this compares the current time to any scheduled events,
       if there's a match then it will turn on or off the specified light ID) */
    LightScheduler_Wakeup();

    /* compare the light ID and its state, they should be light ID 3 and light state on because the
       scheduled event has been reached (scheduled light ID 3 on on weekends (saturdays and sundays) for 8pm.
       Currently it's sunday 8pm, therefore light ID and light state must have been changed) */
    checkLightState( 3, LIGHT_ON );
}

TEST( LightScheduler, ScheduleTwoEventsAtTheSameTime )
{
    /* after initialization of the Light Scheduler there are no scheduled events
       (setup() calls LightController_Create() which sets the last scheduled light ID
       and last scheduled light state as unknowns */

    /* Schedule light with ID 3 to turn on on sunday at minute 1200 (8pm).
       This will use a slot from the scheduledEvents[] array */
    LightScheduler_ScheduleTurnOn( 3, SUNDAY, 1200 );

    /* Schedule light with ID 12 to turn on on sunday at minute 1200 (8pm).
       This will use a slot from the scheduledEvents[] array */
    LightScheduler_ScheduleTurnOn( 12, SUNDAY, 1200 );

    /* set the current (fake) time to sunday 8pm */
    setTimeTo( SUNDAY, 1200 );

    /* callback to Light Scheduler wakeup (this compares the current time to any scheduled events,
       if there's a match then it will turn on or off the specified light ID(s)) */
    LightScheduler_Wakeup();

    /* compare the light ID and its state, they should be light ID 3 and light state on because the
       scheduled event has been reached (scheduled light ID 3 on on sunday for 8pm.
       Currently it's sunday 8pm, therefore light ID and light state must have been changed) */
    checkLightState( 3, LIGHT_ON );

    /* compare the light ID and its state, they should be light ID 12 and light state on because the
       scheduled event has been reached (scheduled light ID 12 on on sunday for 8pm.
       Currently it's sunday 8pm, therefore light ID and light state must have been changed) */
    checkLightState( 12, LIGHT_ON );
}

TEST( LightScheduler, RejectsTooManyEvents )
{
    int i; /* scheduled event index */

    /* for all the possible number of scheduled events (MAX_EVENTS_NUMBER) */
    for ( i = 0; i < 128; i++ )
    {
        /* schedule a light to turn on */
        LONGS_EQUAL( LS_OK, LightScheduler_ScheduleTurnOn( 6, MONDAY, 600 + i ) );
    }

    /* Schedule a light to turn on. At this point there are no more available scheduled events,
       so the expected result is too many events */
    LONGS_EQUAL( LS_TOO_MANY_EVENTS, LightScheduler_ScheduleTurnOn( 6, MONDAY, 600 + i ) );
}

TEST( LightScheduler, RemoveRecyclesScheduledSlot )
{
    int i; /* scheduled event index */

    /* for all the possible number of scheduled events (MAX_EVENTS_NUMBER) */
    for ( i = 0; i < 128; i++ )
    {
        /* schedule a light to turn on */
        LONGS_EQUAL( LS_OK, LightScheduler_ScheduleTurnOn( 6, MONDAY, 600 + i ) );
    }

    /* At this point there are no more available scheduled events (i.e. too many events) */

    /* remove the scheduled light ID 6 to turn on on MONDAY at minite 600 */
    LightScheduler_ScheduleRemove( 6, MONDAY, 600 );

    /* Schedule a light to turn on. This should not be a problem since there is
       one slot available to schedule this event, so the expected result is OK */
    LONGS_EQUAL( LS_OK, LightScheduler_ScheduleTurnOn( 13, MONDAY, 1000 ) );
}
