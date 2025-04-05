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
    int event;
    int id;          /* light id to schedule */
    int minuteOfDay; /* minute of the day to schedule the light */
} ScheduledLightEvent;

enum
{
    UNUSED = -1,
    TURN_OFF, TURN_ON
};

static ScheduledLightEvent scheduledEvent;

void LightScheduler_Create( void )
{
    scheduledEvent.id = UNUSED;
}

static void scheduleEvent( int id, Day day, int minuteOfDay, int event )
{
    /* assign the light ID to the scheduled event ID */
    scheduledEvent.id = id;

    /* assign the minute of the day to the scheduled event minute of the day */
    scheduledEvent.minuteOfDay = minuteOfDay;

    /* assign the scheduled event (either to turn on or off the light) */
    scheduledEvent.event = event;
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
    Time time;

    /* get minute of the day and day of the week */
    TimeService_GetTime( &time );

    /* if the scheduled event ID is not UNUSED (i.e. the light has been scheduled to be turned on/off
       via LightScheduler_ScheduleTurnOn()/Off())
       and if the current minute of the day is the same minute scheduled for the light to be turned on/off */
    if ( ( scheduledEvent.id != UNUSED ) && ( time.minuteOfDay == scheduledEvent.minuteOfDay ) )
    {
        /* turn the light on or off as scheduled */
        scheduledEvent.event == TURN_ON ? LightController_On( scheduledEvent.id ) : LightController_Off( scheduledEvent.id );
    }

    /* otherwise do nothing */
}
