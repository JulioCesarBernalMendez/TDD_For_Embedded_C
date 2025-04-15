/**
 * @file    RandomMinute.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Random Minute module header file containing the prototype functions implemented by RandomMinute.c
 * 
 * @version 0.1
 * @date    2025-04-11
 */

#ifndef RANDOMMINUTE_H
#define RANDOMMINUTE_H

    void RandomMinute_Create( int b );

    /* The production code is refactored so that RandomMinute_GetImpl() can be substituted with
       a fake Random Minute (used for testing).
       
       The direct function call (RandomMinute_Get()) needs to be converted to a function pointer (RandomMinute_Get).
       
       The pointer must be extern to avoid multiple definition errors at link time */
    extern int ( *RandomMinute_Get )( void );

#endif
