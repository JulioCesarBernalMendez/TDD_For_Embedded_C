/**
 * @file    RandomMinute.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Random Minute module source file used by the Light Scheduler module.
 * 
 * @version 0.1
 * @date    2025-04-11
 */

#include <stdlib.h> /* rand() and srand() */
#include <time.h>   /* time() */
#include "RandomMinute.h"

/* Range of random minutes (-bound to +bound).
   This will be updated with the value passed in RandomMinute_Create() */
static int bound = 0;

void RandomMinute_Create( int b )
{
    /* specify the range of possible random minutes */
    bound = b;
    
    /* rand( 10 ); */

    /* Randomize the rand() function using the time function.
       This ensures rand() produces different values every time the program is executed */
    srand( time( NULL ) );
}

/* Now the name of the production code implementation of the random minute generation is changed slightly,
   the reason is because RandomMinute_Get now is a function pointer */
int RandomMinute_GetImpl( void )
{
    /* Generate a random value between -bound and +bound.
    
       rand() % ( 2 * bound + 1 ): generates ( 2 * bound + 1 ) numbers from 0 to bound * 2,
                                   e.g. bound = 5:
                                   it produces rand() % 11, which gives a possibility of 11 values (0 to 10 or 0 to 2 * bound)
       bound - rand() % ( bound * 2 + 1 ): generates a random value between -bound to +bound,
                                           e.g. bound = 5:
                                           it produces 5 - rand() % 11 = 5 - (a value between 0 to 10),
                                           lowest:  5 - 10 = -5
                                           highest: 5 -  0 =  5 */
    return bound - rand() % ( 2 * bound + 1 );
}

/* The function pointer is initialized to point to RandomMinute_GetImpl().

   The purpose of this function pointer is to allow any TEST() or TEST_GROUP() to override the
   default function pointer value (RandomMinute_GetImpl) with a pointer to a fake random minute function.

   This is useful when testing unpredictable production code (e.g. rand()) and helps avoid unpredictable results
   (in future tests, we will want to have control over the returned result by RandomMinute_Get()) )*/
int ( *RandomMinute_Get )( void ) = RandomMinute_GetImpl;
