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

    void LightScheduler_Create( void );
    void LightScheduler_Wakeup( void );
    void LightScheduler_ScheduleTurnOn( int id, Day day, int minuteOfDay );

#endif