/**
 * @file    FakeTimeService.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Time Service module fake source file that overrides the functions for the Time Service module (TimeService.c).
 * 
 *          Most of the functions contained here are originally implemented in TimeService.c, 
 *          however since that make it (Time Service) a DOC (Depended On Component) to the
 *          Light Scheduler module (i.e. Light Scheduler module, depends on Time Service module),
 *          it makes the Light Scheduler testing very challenging due to those dependencies.
 *          For that we can use interfaces, test doubles and link-time substitution to manage the problem dependencies.
 * 
 *          Time is a big deal in embedded systems, it's a volatile input and makes testing a challenge.
 *          Waiting for timed events in tests makes the tests take too long, longer than they have to be.
 *          The bottom line is that the tests have to take over the clock.
 * 
 *          Abstracting the clock is important. Real-time operating systems often define nonstandard
 *          time functions, and this can lead to portability problems. To have code that runs in more
 *          than one platform, abstract the clock. The abstraction provides a perfect place to instert
 *          the fake clock.
 * 
 *          The test stub needed for Fake Time Service is kind of the opposite of the Light Controller Spy.
 *          The test is not interested in what the CUT (Code Under Test. In this case the Light Scheduler module)
 *          passes to the Time Service, but is interested in controlling what the Time Service returns to the CUT.
 *          This is known as an indirect input.
 * 
 * @version 0.1
 * @date    2025-03-31
 */

#include "FakeTimeService.h"

/* structure to hold:
   - minute of the day
   - and day of the week */
static Time fakeTime;

void TimeService_Create( void )
{
    fakeTime.minuteOfDay = TIME_UNKNOWN;
    fakeTime.dayOfWeek   = TIME_UNKNOWN;
}

/* Function that controls the information returned to the CUT during the Fake's mission.
   This information is controlled through the interface (TimeService_GetTime())
   of the replaced collaborator (Time Service).
   
   Here, we use a test double for TimeService_GetTime(), which (originally implemented in TimeService.c)
   is substituted during link-time (so that it uses the test version from FakeTimeService.c instead) */
void TimeService_GetTime( Time *time )
{
    time->minuteOfDay = fakeTime.minuteOfDay;
    time->dayOfWeek   = fakeTime.dayOfWeek;
}

/* As explained in the description of this file, time is a volatile input that makes testing a challenge.
   Waiting for timed events takes too long. Here this interface takes over the clock making it possible 
   to set a specific minute of the day */
void FakeTimeService_SetMinute( int minute )
{
    fakeTime.minuteOfDay = minute;
}

/* As explained in the description of this file, time is a volatile input that makes testing a challenge.
   Waiting for timed events takes too long. Here this interface takes over the clock making it possible 
   to set a specific day of the week */
void FakeTimeService_SetDay( int day )
{
    fakeTime.dayOfWeek = day;
}
