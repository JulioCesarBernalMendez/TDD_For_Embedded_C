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
#include "LightController.h"
#include "TimeService.h"
#include "common.h"
#include "RandomMinute.h"

typedef struct
{
    int event;         /* light event (ON or OFF) to schedule */
    int id;            /* light id to schedule */
    Day day;           /* day of the week to schedule the light */
    int minuteOfDay;   /* minute of the day to schedule the light */

    int randomize;     /* randomization feature of the light scheduler (ON or OFF) */
    int randomMinutes; /* random minutes offset used to schedule a light event before/after the scheduled time */
} ScheduledLightEvent;

enum
{
    TURN_OFF,  TURN_ON,
    RANDOM_ON, RANDOM_OFF
};

enum
{
    UNUSED = -1,
    MAX_EVENTS_NUMBER = 128 /* max number of possible scheduled events */
};

static ScheduledLightEvent scheduledEvents[ MAX_EVENTS_NUMBER ]; /* slots for scheduled events (up to 128 slots) */

static int scheduleEvent( int id, Day day, int minuteOfDay, int event )
{
    /* Now scheduleEvent() handles multiple-event "schedulization" */

    int i; /* scheduled event index */

    /* if the light ID is out of range */
    if ( ( id < 0 ) || ( id >= MAX_LIGHTS_NUMBER ) )
    {
        /* light ID not valid, return id out of bounds */
        return LS_ID_OUT_OF_BOUNDS;
    }

    /************ MULTIPLE-EVENT SCHEDULIZATION ************/
    /* loop through the array of scheduled event slots */
    for ( i = 0; i < MAX_EVENTS_NUMBER; i++ )
    {
        /* find an available slot */
        if ( scheduledEvents[ i ].id == UNUSED )
        {
            /* assign the light ID to the scheduled event ID */
            scheduledEvents[ i ].id = id;

            /* assign the day of the week to the scheduled event day of the week */
            scheduledEvents[ i ].day = day;

            /* assign the minute of the day to the scheduled event minute of the day */
            scheduledEvents[ i ].minuteOfDay = minuteOfDay;

            /* assign the scheduled event (either to turn on or off the light) */
            scheduledEvents[ i ].event = event;

            /* disable randomization of the scheduled event (light will be turned on/off)
               at the exact scheduled minute (unless the event is randomized by LightScheduler_Randomize()) */
            scheduledEvents[ i ].randomize = RANDOM_OFF;

            /* set to 0 the random minutes of the Light Scheduler (this value will be updated every time
               LightScheduler_Randomize() is called

               Note: LightScheduler_WakeUp() also calls LightScheduler_Randomize() if the time for the
                     scheduled event has been reached */
            scheduledEvents[ i ].randomMinutes = 0;

            /* break out of the loop. The event slot has now been scheduled,
               there's no reason to keep looping */
            return LS_OK;
        }
    }

    /* if code reaches here it means there were no available scheduled event slots */
    return LS_TOO_MANY_EVENTS;
}

static void operateLight( ScheduledLightEvent *lightEvent )
{
    /* turn the light on or off as scheduled */
    lightEvent->event == TURN_ON ? LightController_On( lightEvent->id ) : LightController_Off( lightEvent->id );

    /* if randomization of the scheduled event is enabled */
    if ( lightEvent->randomize == RANDOM_ON )
    {
        /* get a new random minute offset for the Light Scheduler */
        lightEvent->randomMinutes = RandomMinute_Get();
    }
    /* if randomization is not enabled */
    else
    {
        /* set to 0 the random minute */
        lightEvent->randomMinutes = 0;
    }
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

    /* simplify current day and current minute */
    int today  = time->dayOfWeek;
    int minute = time->minuteOfDay;

    /* simplify scheduled event parameters */
    int scheduledDay           = lightEvent->day;
    int scheduledMinute        = lightEvent->minuteOfDay;
    int scheduledRandomMinutes = lightEvent->randomMinutes;

    /* if the scheduled event light ID is not UNUSED (i.e. the light has been scheduled to be turned on/off
       via LightScheduler_ScheduleTurnOn()/Off() */
    if ( lightEvent->id == UNUSED )
        return;

    /* if today is not the scheduled day for the light to be turned on/off */
    if ( doesLightRespondToday( today, scheduledDay ) == FALSE )
        return;

    /* if the scheduled minute for the light to be turned on/off (taking into acount the random minutes offset)
       is not the same as the current minute of the day */
    if ( ( scheduledMinute + scheduledRandomMinutes ) != minute ) 
        return;

    /* else: a light was scheduled to turn on or off at this precise time */
    operateLight( lightEvent );
}

void LightScheduler_Create( void )
{
    /* Now LightScheduler_Create() handles multiple-scheduled event initializations */

    int i; /* scheduled event index */

    /************ MULTIPLE-EVENT INITIALIZATION ************/
    /* for all the scheduled event slots */
    for ( i = 0; i < MAX_EVENTS_NUMBER; i++ )
    {
        /* there are no scheduled events (lights to be turned on/off).
           So ALL the slots for a scheduled event is unused and available */
        scheduledEvents[ i ].id = UNUSED;
    }

    /* Register the alarm callback function to be called "every minute".
       In this case LightScheduler_WakeUp() */
    TimeService_SetPeriodicAlarmInSeconds( 60, LightScheduler_WakeUp );
}

void LightScheduler_Destroy( void )
{
    /* unregister the alarm callback function previously registered with TimeService_SetPeriodicAlarmInSeconds()
       which is called in LightScheduler_Create() */
    TimeService_CancelPeriodicAlarmInSeconds( 60, LightScheduler_WakeUp );
}

int LightScheduler_ScheduleTurnOn( int id, Day day, int minuteOfDay )
{
    /* This function DOES NOT turn on the scheduled light when the time comes.
       That action is for the Light Controller to do, which is called by the
       Light Scheduler wake up */

    /* schedule the event:
       - set the light ID
       - set the day of the week to schedule the event
       - set the minute of the day to schedule the event
       - set the type of event as turn the light on
       - disable randomization of the scheduled event
       - set to 0 the random minute (random minute is used when randomization is enabled) */
    return scheduleEvent( id, day, minuteOfDay, TURN_ON );
}

int LightScheduler_ScheduleTurnOff( int id, Day day, int minuteOfDay )
{
    /* This function DOES NOT turn off the scheduled light when the time comes.
       That action is for the Light Controller to do, which is called by the
       Light Scheduler wake up */

    /* schedule the event:
       - set the light ID
       - set the day of the week to schedule the event
       - set the minute of the day to schedule the event
       - set the type of event as turn the light off
       - disable randomization of the scheduled event
       - set to 0 the random minute (random minute is used when randomization is enabled) */
    return scheduleEvent( id, day, minuteOfDay, TURN_OFF );
}

int LightScheduler_ScheduleRemove( int id, Day day, int minuteOfDay )
{
    int i; /* scheduled event index */

    /* if the light ID is out of range */
    if ( ( id < 0 ) || ( id >= MAX_LIGHTS_NUMBER ) )
    {
        /* light ID not valid, return id out of bounds */
        return LS_ID_OUT_OF_BOUNDS;
    }

    /************ MULTIPLE-EVENT REMOVAL ************/
    /* loop through the array of scheduled event slots */
    for ( i = 0; i < MAX_EVENTS_NUMBER; i++ )
    {
        /* if the scheduled event to remove exists */
        if ( scheduledEvents[ i ].id  == id
          && scheduledEvents[ i ].day == day
          && scheduledEvents[ i ].minuteOfDay == minuteOfDay )
        {
            /* remove it (make the slot available again) */
            scheduledEvents[ i ].id = UNUSED;

            /* break out of the loop. The event slot has now been removed,
               there's no reason to keep looping */
            return LS_OK;
        }
    }

    /* if code reaches here it means the event to remove does not exist */
    return LS_EVENT_DOES_NOT_EXIST;
}

void LightScheduler_Randomize( int id, Day day, int minuteOfDay )
{  
    int i; /* scheduled event index */

    /* pointer to a scheduled event */
    ScheduledLightEvent *e;

    /* search for the specified event in the scheduled event slots */
    for ( i = 0; i < MAX_EVENTS_NUMBER; i++ )
    {
        /* update the scheduled event pointer to point to the current index in the scheduled events array */
        e = &scheduledEvents[ i ];

        /* if the specified event exists */
        if ( ( e->id == id ) && ( e->day == day ) && ( e->minuteOfDay == minuteOfDay ) )
        {
            /* enable randomization of the scheduled event */
            e->randomize = RANDOM_ON;

            /* get a random minute */
            e->randomMinutes = RandomMinute_Get();
        }
    }
}

void LightScheduler_WakeUp( void )
{
    /* This is the function to be provided to the Time Service as a periodic callback function */

    /* Now LightScheduler_WakeUp() handles multiple-event proceessing */

    int i; /* scheduled event index */
    Time time; /* struct to hold current time (day of the week and minute of the day) */

    /* get current minute of the day and day of the week */
    TimeService_GetTime( &time );

    /************ MULTIPLE-EVENT PROCESSING ************/
    /* for all the scheduled event slots */
    for ( i = 0; i < MAX_EVENTS_NUMBER; i++ )
    {
        /* process the scheduled event (if any) */
        processEventDueNow( &time, &scheduledEvents[ i ] );
    }
}
