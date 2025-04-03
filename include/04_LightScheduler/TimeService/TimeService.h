/**
 * @file    TimeService.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Time Service module header file containing the prototype functions implemented by TimeService.c
 * 
 * @version 0.1
 * @date    2025-03-31
 */

#ifndef TIMESERVICE_H
#define TIMESERVICE_H

    typedef struct Time
    {
        int minuteOfDay;
        int dayOfWeek;
    } Time;

    /* These are the function prototypes for the Time Service module production source code
       (TimeService.c). Because the Light Scheduler module (which depends on the Time Service module)
       is under test, some of the TimeService.c function implementations are overriden with the fake's
       implementations in FakeTimeService.c (this takes place during the linking stage).
       Therefore there's no src/04_LightScheduler/TimeService/TimeService.c file in the project,
       simply because the test code "does not need it" (at least for the time being) */

    void TimeService_Create( void );
    void TimeService_GetTime( Time *time );
 
#endif
