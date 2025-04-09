/**
 * @file    LightControllerSpy.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Light Controller module spy header file containing the prototype spy functions for
 *          the Light Controller spy module (LightControllerSpy.c).
 * 
 * @version 0.1
 * @date    2025-03-31
 */

#ifndef LIGHTCONTROLLERSPY_H
#define LIGHTCONTROLLERSPY_H

   #include <stdint.h>

   /* the Light Controller Spy also overrides some of the functions from the
      Light Controller, this takes place during the linking stage.
      Therefore include the .h header from production code, but those functions will be linked to the spy instead */
   #include "LightController.h"

   enum
   { 
      LIGHT_ID_UNKNOWN = -1, LIGHT_STATE_UNKNOWN = -1,
      LIGHT_OFF = 0, LIGHT_ON = 1
   };

   /* some other functions are not part of the Light Controller,
      therefore they're implemented inside the Light Controller Spy */
   int LightControllerSpy_GetLightState( int id );
   int LightControllerSpy_GetLastId( void );
   int LightControllerSpy_GetLastState( void );

#endif
