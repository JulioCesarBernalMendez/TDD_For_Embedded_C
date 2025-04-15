/**
 * @file    LightScheduler.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Light Scheduler module header file containing the prototype functions implemented by LightScheduler.c
 * 
 * @version 0.1
 * @date    2025-04-03
 */

#ifndef LIGHTSCHEDULER_H
#define LIGHTSCHEDULER_H

    typedef enum
    {
        NOT_A_DAY = -4,
        EVERYDAY  = -3, WEEKDAY = -2, WEEKEND = -1,
        SUNDAY = 1, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
    } Day;

    enum { LS_OK, LS_TOO_MANY_EVENTS, LS_ID_OUT_OF_BOUNDS, LS_EVENT_DOES_NOT_EXIST };

    void LightScheduler_Create( void );
    void LightScheduler_Destroy( void );
    int LightScheduler_ScheduleTurnOn( int id, Day day, int minuteOfDay );
    int LightScheduler_ScheduleTurnOff( int id, Day day, int minuteOfDay );
    int LightScheduler_ScheduleRemove( int id, Day day, int minuteOfDay );
    void LightScheduler_Randomize( int id, Day day, int minuteOfDay );
    void LightScheduler_WakeUp( void );

#endif
