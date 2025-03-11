/**
 * @file    RuntimeErrorStub.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Stub header file containing the prototypes for the stub functions implemented in RuntimeErrorStub.c
 * 
 *          Code extracted from to the "Test-Development for Embedded C" book by James W. Grenning
 * 
 * @version 0.1
 * @date    2025-03-10
 */

#ifndef RUNTIMEERRORSTUB_H
#define RUNTIMEERRORSTUB_H

    const char* RuntimeErrorStub_GetLastError( void );
    int RuntimeErrorStub_GetLastParameter( void );
    void RuntimeErrorStub_Reset( void );

#endif
