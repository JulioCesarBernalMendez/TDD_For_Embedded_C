/**
 * @file    LightControllerSpy.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Light Controller module spy source file that overrides the functions for the Light Controller module (LightController.c).
 * 
 *          Most of the functions contained here are originally implemented in LightController.c, 
 *          however since that make it (Light Controller) a DOC (Depended On Component) to the
 *          Light Scheduler module (i.e. Light Scheduler module, depends on Light Controller module),
 *          it makes the Light Scheduler testing very challenging due to those dependencies.
 *          For that we can use interfaces, test doubles and link-time substitution to manage the problem dependencies.
 * 
 *          These functions capture the parameters passed from the CUT (Code Under Test. In this case the Light Scheduler module)
 *          so the test can verify that the correct parameters have been passed to the CUT.
 * 
 *          The spy can also feed return values to the CUT just like a test stub.
 * 
 *          The spy's implementation defines a dead drop, made from file scope data,
 *          where the spy stores intelligence during its covert operation.
 * 
 * @version 0.1
 * @date    2025-03-31
 */

#include "LightControllerSpy.h"

static int lastId;    /* last light ID controlled */
static int lastState; /* last light State controlled */

static int lightStates[ MAX_LIGHTS_NUMBER ]; /* array that contains the light state for all the lights (up to 32 lights) */

/* Function that initializes the spy's dead drop. 

   Here, we use a test double for LightController_Create(), which (originally implemented in LightController.c)
   is substituted during link-time (so that it uses the test version from LightControllerSpy.c instead) */
void LightController_Create( void )
{
   /* during initialization of the Light Controller driver, there are no scheduled events due, so that:
      - the ID of the light that was last controlled has to be LIGHT_ID_UNKNOWN (no light control since initialization).
      - the state of the light that was last controlled has to be LIGHT_STATE_UNKNOWN (the light has not been changed since initialization).
      
      This test double simulates that behavior */

   int i; /* scheduled event index */
   
   /* last light ID controlled and its state are both unknown */
   lastId    = LIGHT_ID_UNKNOWN;
   lastState = LIGHT_STATE_UNKNOWN;

   /************ MULTIPLE-EVENT INITIALIZATION ************/
   /* for all the light IDs (MAX_LIGHTS_NUMBER) */
   for ( i = 0; i < MAX_LIGHTS_NUMBER; i++ )
   {
      /* all their states are unknown */
      lightStates[ i ] = LIGHT_STATE_UNKNOWN;
   }
}

/* Function that intercepts critical information during the spy's mission.
   This critical information is intercepted through the interface (LightController_On())
   of the replaced collaborator (Light Controller).
   
   Here, we use a test double for LightController_On(), which (originally implemented in LightController.c)
   is substituted during link-time (so that it uses the test version from LightControllerSpy.c instead) */
void LightController_On( int id )
{
   /* update both the last light ID and last light state */
   lastId    = id;
   lastState = LIGHT_ON;

   /* if the light ID is within the valid range of light IDs */
   if ( ( id >= 0 ) && ( id < MAX_LIGHTS_NUMBER ) )
   {
      /* update the specified light ID state */
      lightStates[ id ] = LIGHT_ON;
   }
}

/* Function that intercepts critical information during the spy's mission.
   This critical information is intercepted through the interface (LightController_Off())
   of the replaced collaborator (Light Controller).
   
   Here, we use a test double for LightController_Off(), which (originally implemented in LightController.c)
   is substituted during link-time (so that it uses the test version from LightControllerSpy.c instead) */
void LightController_Off( int id )
{
   /* update both the last light ID and last light state */
   lastId    = id;
   lastState = LIGHT_OFF;

   /* if the light ID is within the valid range of light IDs */
   if ( ( id >= 0 ) && ( id < MAX_LIGHTS_NUMBER ) )
   {
      /* update the specified light ID state */
      lightStates[ id ] = LIGHT_OFF;
   }
}

/* this is a Light Controller's spy function.
   Since there's no specific function for this purpose on production code,
   it is implemented with the name 'spy' on it.
   
   The one being spied on (Light Controller) suspects nothing.
   The intelligence is retrieved from the dead drop through secret accessor functions
   after the CUT (Light Scheduler) is exercised */
int LightControllerSpy_GetLightState( int id )
{
   /* if the light ID is out of range */
   if ( ( id < 0 ) || ( id >= MAX_LIGHTS_NUMBER ) )
   {
      /* light ID not valid, return unknown state */
      return LIGHT_STATE_UNKNOWN;
   }
   
   /* else, return the light ID state */
   return lightStates[ id ];
}

/* this is a Light Controller's spy function.
   Since there's no specific function for this purpose on production code,
   it is implemented with the name 'spy' on it.
   
   The one being spied on (Light Controller) suspects nothing.
   The intelligence is retrieved from the dead drop through secret accessor functions
   after the CUT (Light Scheduler) is exercised */
int LightControllerSpy_GetLastId( void )
{
   return lastId;
}

/* this is a Light Controller's spy function.
   Since there's no specific function for this purpose on production code,
   it is implemented with the name 'spy' on it.
   
   The one being spied on (Light Controller) suspects nothing.
   The intelligence is retrieved from the dead drop through secret accessor functions
   after the CUT (Light Scheduler) is exercised */
int LightControllerSpy_GetLastState( void )
{
   return lastState;
}
