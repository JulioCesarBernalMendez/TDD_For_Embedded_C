/**
 * @file    LightController.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Light Controller module header file containing the prototype functions implemented by LightController.c
 * 
 * @version 0.1
 * @date    2025-03-31
 */

#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

    #include <stdint.h>

    enum { MAX_LIGHTS_NUMBER = 32 };

    /* These are the function prototypes for the Light Controller module production source code
       (LightController.c). Because the Light Scheduler module (which depends on the Light Controller module)
       is under test, some of the LightController.c function implementations are overriden with the spy's
       implementations in LightControllerSpy.c (this takes place during the linking stage).
       Therefore there's no src/04_LightScheduler/LightController/LightController.c file in the project,
       simply because the test code "does not need it" (at least for the time being) */
    
    void LightController_Create( void );
    void LightController_On( int id );
    void LightController_Off( int id );

#endif
