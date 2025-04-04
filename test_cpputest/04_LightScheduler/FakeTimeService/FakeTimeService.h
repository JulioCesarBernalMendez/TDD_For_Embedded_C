/**
 * @file    FakeTimeService.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   (Fake) Time Service module header file containing the fake prototype functions for
 *          the (Fake) Time Service module (FakeTimeService.c).
 * 
 * @version 0.1
 * @date    2025-04-03
 */

#ifndef FAKETIMESERVICE_H
#define FAKETIMESERVICE_H

    /* the Fake Time Service also overrides some of the functions from the
       (Real) Time Service, this takes place during the linking stage.
       Therefore include the .h header from production code, but those functions will be linked to the fake instead */
    #include "TimeService.h"

    enum { TIME_UNKNOWN = -1 };

    /* some other functions are not part of the Time Service,
      therefore they're implemented inside the Fake Time Service */
    void FakeTimeService_SetMinute( int minute );
    void FakeTimeService_SetDay( int day );

#endif
