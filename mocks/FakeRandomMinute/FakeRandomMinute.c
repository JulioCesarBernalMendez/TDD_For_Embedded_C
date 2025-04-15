/**
 * @file    FakeRandomMinute.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Random Minute module fake source file used to test the randomize feature of the Light Scheduler module.
 * 
 * @version 0.1
 * @date    2025-04-14
 */

#include "FakeRandomMinute.h"

static int seed      = -1;
static int increment = -1;

/* This fake function establishes a not so random minute sequence for the fake random minute generator RandomMinute_Get = FakeRandomMinute_Get():
   - The seed is the first value to be returned by FakeRandomMinute_Get().
     E.g. seed = 10, then FakeRandomMinute_Get() returns 10 the first time is called
   - The increment is added to the seed every time FakeRandomMinute_Get().
     E.g. seed = 10, increment = 5, then:
     - FakeRandomMinute_Get() returns 10 the first time is called
     - FakeRandomMinute_Get() returns 15 the second time is called
     - FakeRandomMinute_Get() returns 20 the third time is called
     - and so on... */
void FakeRandomMinute_SetFirstAndIncrement( int s, int i )
{
    seed = s;
    increment = i;
}

/* read description above */
int FakeRandomMinute_Get( void )
{
    int result = seed;

    seed += increment;

    return result;
}
