/**
 * @file    LightSchedulerTest.cpp
 * @author  Julio Cesar Bernal Mendez
 * @brief   Light Scheduler test source file containing the test cases
 *          and test group for the Light Scheduler module.
 *          Light Scheduler depends on two other modules:
 *          - Light Controller
 *          - and Time Service
 * 
 *          The premise is:
 *          - we are developing a module 'Light Scheduler' that depends on 2 other modules
 *          - these other 2 modules are: a Hardware Dependent Code 'Light Controller' and
 *            an Operating System dependent code (Time Service)
 *          - we don't have the Hardware available yet, and we haven't chosen the OS either ...
 *            ... should this stop us from developing the Light Scheduler? HELL NO!, we have TDD
 *          - OK, then how to develop then?
 *            we can treat those 2 areas (Light Controller and Time Service) as DOCs (Dependent On Components)
 *            and define interfaces that meet the CUT (Code Under Test) needs
 *          - these interfaces will allow us to break dependencies on unknowns
 *          - we develop right up to the unknown, defining what we want from the interfaces.
 *            The tests will help us drive the design!
 * 
 * @version 0.1
 * @date    2025-03-31
 */

extern "C"
{
    /* includes for things with C linkage */
    #include "LightControllerSpy.h"
}
 
/* includes for things with C++ linkage */
#include "TestHarness.h"
 
TEST_GROUP( LightScheduler )
{
    /* define data accessible to test group members here */
 
    void setup()
    {
        /* initialization steps are executed before each TEST */ 
    }
 
    void teardown()
    {
        /* clean up steps are executed after each TEST */
    }
};

TEST( LightScheduler, NoChangeToLightsDuringInitialization )
{
    /* A natural first test is to specify what should happen during initialization
       of the Light Scheduler:
       - There are no scheduled events due, so Light Controller functions should not be called
       - The spy is interrogated after the "covert mission" by checking its "secret" test-only
         interface made up of LightControllerSpy_GetLastId() and
         LightControllerSpy_GetLastState()
       - LightControllerSpy_GetLastId(): returns the ID of the light that was
                                                        controlled or LIGHT_ID_UNKNOWN where
                                                        there has been no light control
       - LightControllerSpy_GetLastState(): returns LIGHT_OFF, LIGHT_ON or LIGHT_STATE_UNKNOWN.
                                                           LIGHT_STATE_UNKNOWN means the light has not been
                                                           changed since initialization
       - if the mission is "successful", the interrogation should show that no light instructions were given

       The test checks the expected outcome */

    LONGS_EQUAL( LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId() );
    LONGS_EQUAL( LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState() );
}
