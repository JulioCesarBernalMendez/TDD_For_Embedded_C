/**
 * @file    RuntimeError.h
 * @author  Julio Cesar Bernal Mendez
 * @brief   Header file containing:
 *          - RuntimeError() function prototype: captures and checks the last error. Stub implemented in RuntimeErrorStub.c
 *          - RUNTIME_ERROR() macro: logs an error recording the message provided as well as the file and line number.
 * 
 *          Code extracted from the "Test-Development for Embedded C" book by James W. Grenning
 * 
 * @version 0.1
 * @date    2025-03-10
 */

#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H

    void RuntimeError( const char *message, int parameter, const char *file, int line );

    #define RUNTIME_ERROR( description, parameter )    RuntimeError( description, parameter, __FILE__, __LINE__ )

#endif
