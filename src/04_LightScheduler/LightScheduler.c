/**
 * @file    LightScheduler.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Light Scheduler module source file that implements the functions for the Light Scheduler module.
 * 
 *          The Light Scheduler (handles the light-scheduling) has transitive dependencies on 2 other modules:
 *          - Light Controller (hardware to turn on/off the lights)
 *          - and Time Service module (OS)
 * 
 * @version 0.1
 * @date    2025-04-03
 */

#include "LightScheduler.h"

typedef struct
{
    int event;       /* light event (ON or OFF) to schedule */
    int id;          /* light id to schedule */
    Day day;         /* day of the week to schedule the light */
    int minuteOfDay; /* minute of the day to schedule the light */
} ScheduledLightEvent;

enum
{
    UNUSED = -1,
    TURN_OFF, TURN_ON
};

static ScheduledLightEvent scheduledEvent;

static void scheduleEvent( int id, Day day, int minuteOfDay, int event )
{
    /* assign the light ID to the scheduled event ID */
    scheduledEvent.id = id;

    /* assight the day of the week to the scheduled event day of the week */
    scheduledEvent.day = day;

    /* assign the minute of the day to the scheduled event minute of the day */
    scheduledEvent.minuteOfDay = minuteOfDay;

    /* assign the scheduled event (either to turn on or off the light) */
    scheduledEvent.event = event;
}

static void operateLight( ScheduledLightEvent *lightEvent )
{
    /* operateLight captures the idea behind the if/else chain */

    /* turn the light on or off as scheduled */
    lightEvent->event == TURN_ON ? LightController_On( scheduledEvent.id ) : LightController_Off( scheduledEvent.id );
}

static int doesLightRespondToday( int today, int scheduledDay )
{
    /* this helper function returns TRUE if the today is the day for the scheduled light to be turned on or off,
       otherwise returns FALSE */

    /* if the light is scheduled to turn on/off everyday (then no matter the day, today the light should be turned on/off) */
    if ( scheduledDay == EVERYDAY )
        return TRUE;

    /* if the light is scheduled to turn on/off today */
    if ( scheduledDay == today )
        return TRUE;

    /* if the light is scheduled to turn on/off on the weekends and today is either saturday or sunday */
    if ( ( scheduledDay == WEEKEND ) && ( ( today == SATURDAY ) || ( today == SUNDAY ) ) )
        return TRUE;

    /* if the light is scheduled to turn on/off and today is either monday to friday */
    if ( ( scheduledDay == WEEKDAY ) && ( ( today >= MONDAY ) && ( today <= FRIDAY ) ) )
        return TRUE;

    /* if none of the previous conditions is satisfied, then today the light is not scheduled to turn on/off */
    return FALSE;
}

static void processEventDueNow( Time *time, ScheduledLightEvent *lightEvent )
{
    /* processEventDueNow is responsible for conditionally triggering a single event.
       This function is all set to be called from a loop when support for multiple events is added */

    int today        = time->dayOfWeek;
    int scheduledDay = lightEvent->day;

    /* if the scheduled event light ID is not UNUSED (i.e. the light has been scheduled to be turned on/off
       via LightScheduler_ScheduleTurnOn()/Off() */
    if ( lightEvent->id == UNUSED )
        return;

    /* if today is not the scheduled day for the light to be turned on/off */
    if ( doesLightRespondToday( today, scheduledDay ) == FALSE )
        return;

    /* if the scheduled minute for the light to be turned on/off is not the same as the current minute of the day */
    if ( lightEvent->minuteOfDay != time->minuteOfDay )
        return;

    /* else: a light was scheduled to turn on or off at this precise time */
    operateLight( lightEvent );
}

void LightScheduler_Create( void )
{
    scheduledEvent.id = UNUSED;
}

void LightScheduler_ScheduleTurnOn( int id, Day day, int minuteOfDay )
{
    /* This function DOES NOT turn on the scheduled light when the time comes.
       That action is for the Light Controller to do, which is called by the
       Light Scheduler wake up */

    /* schedule the event:
       - set the light ID
       - set the day of the week to schedule the event (not implemented yet)
       - set the minute of the day to schedule the event
       - set the type of event as turn the light on */
    scheduleEvent( id, day, minuteOfDay, TURN_ON );
}

void LightScheduler_ScheduleTurnOff( int id, Day day, int minuteOfDay )
{
    /* This function DOES NOT turn offthe scheduled light when the time comes.
       That action is for the Light Controller to do, which is called by the
       Light Scheduler wake up */

    /* schedule the event:
       - set the light ID
       - set the day of the week to schedule the event (not implemented yet)
       - set the minute of the day to schedule the event
       - set the type of event as turn the light off */
    scheduleEvent( id, day, minuteOfDay, TURN_OFF );
}

void LightScheduler_Wakeup( void )
{
    /* This is the function to be provided to the Time Service as a periodic callback function.
       Right now it processes a single event, but later will process each event in the collection
       of scheduled events */

    Time time;

    /* get current minute of the day and day of the week */
    TimeService_GetTime( &time );

    /* schedule the event for the specified time */
    processEventDueNow( &time, &scheduledEvent );
}
