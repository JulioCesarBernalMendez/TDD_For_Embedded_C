/**
 * @file    LightControllerSpy.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Light Controller module spy source file that overrides the functions for the Light Controller module (LightController.c).
 * 
 *          Captures the parameters passed from the CUT (Code Under Test. In this case the Light Scheduler module)
 *          so the test can verify that the correct parameters have been passed to the CUT.
 * 
 *          The spy can also feed return values to the CUT just like a test stub.
 * 
 * @version 0.1
 * @date    2025-03-31
 */

#include "LightControllerSpy.h"

/* this is a Light Controller spy function.
   Since there's no specific function for this purpose on production code, it is implemented with the name 'spy' on it */
int LightControllerSpy_GetLastId( void )
{
   /* do the simplest thing to get the NoChangeToLightsDuringInitialization test to pass...
      hard-coding it (we're doing test driven, the TESTS are right!,
      even though production code is not right. Don't worry, production code
      won't be hard-coded and incomplete for long) */
   return LIGHT_ID_UNKNOWN;
}

/* this is a Light Controller spy function.
   Since there's no specific function for this purpose on produciton code, it is implemented with the name 'spy' on it */
int LightControllerSpy_GetLastState( void )
{
   /* do the simplest thing to get the NoChangeToLightsDuringInitialization test to pass...
      hard-coding it (we're doing test driven, the TESTS are right!,
      even though production code is not right. Don't worry, production code
      won't be hard-coded and incomplete for long) */
   return LIGHT_STATE_UNKNOWN;
}
