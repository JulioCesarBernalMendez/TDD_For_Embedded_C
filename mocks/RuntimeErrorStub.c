/**
 * @file    RuntimeErrorStub.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Runtime error source file that implements the error stub functions.
 * 
 *          Code extracted from to the "Test-Development for Embedded C" book by James W. Grenning
 * 
 * @version 0.1
 * @date    2025-03-10
 */

#include "RuntimeError.h"
#include "RuntimeErrorStub.h"

static const char *message = "No Error";
static int       parameter = -1;
static const char    *file = 0;
static int            line = -1;

const char *RuntimeErrorStub_GetLastError( void )
{
    return message;
}

int RuntimeErrorStub_GetLastParameter( void )
{
    return parameter;
}

void RuntimeError( const char *m, int p, const char *f, int l )
{
    /* update runtime error message */
    message = m;

    /* update runtime error parameter */
    parameter = p;

    /* specify the source file where the runtime error occurred */
    file = f;

    /* specify the line number of the source file where the runtime error occurred */
    line = l;
}

void RuntimeErrorStub_Reset( void )
{
    /* reset error message and parameter*/
    message = "No Error";
    parameter = -1;
}
