/**
 * @file    AllCppUTestTests.c
 * @author  Julio Cesar Bernal Mendez
 * @brief   Source file which contains main() and runs the test group specified in TestDummyDriver.cpp
 * 
 * @version 0.1
 * @date    2025-03-28
 */

#include "CommandLineTestRunner.h"

int main( int argc, char **argv )
{
    /* return CommandLineTestRunner::RunAllTests( argc, argv ); */
    return RUN_ALL_TESTS( argc, argv );
}
