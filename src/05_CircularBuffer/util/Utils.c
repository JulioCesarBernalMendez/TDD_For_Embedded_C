/**
 * @file    Utils.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Source file that contains the function pointer implementation used to replace the value pointed to by
 *          FormatOutput function pointer (which by default uses printf()) with FormatOutputSpy for testing purposes.
 * 
 *          This is a function pointer-based test seam through a utility module to isolate and redirect calls to
 *          printf() for testing purposes — without touching the production code.
 * 
 *          Unlike the RandomMinute_Get approach used in RandomMinute.c, here an intermediary "object" is used
 *          (Utils.h/Utils.c) to define and implement the fake FormatOutput(), FormatOutputSpy().
 *          
 *          Why do we want to replace FormatOutput with FormatOutputSpy?
 *          When a system has a printed output, it is usually manually inspected.
 *          Printed output is tedious to verify, so we don't want to reinspect the output as often as we should.
 *          We will never get totally away from manually inspecting printed output, but its possible to eliminate
 *          the re-inspections by locking in the desired behavior.
 *          ("Locking in" means we've tested, verified, and maybe even automated enough of the process that we're
 *          confident it's working as intended every time).
 * 
 * @version 0.1
 * @date    2025-04-16
 */

#include "Utils.h"
#include <stdio.h>

/*
static int FormatOutput_Impl( const char *format, ... )
{
    // snip
}

int ( *FormatOutput )( const char *format, ... ) = FormatOutput_Impl;
*/

/* The function pointer is initialized to point to the "production" printf().

   The purpose of this function pointer is to allow any TEST() or TEST_GROUP() to override the
   default function pointer value (printf) with a pointer to a fake printf (i.e. FormatOutputSpy()) */
int ( *FormatOutput )( const char *format, ... ) = printf;
