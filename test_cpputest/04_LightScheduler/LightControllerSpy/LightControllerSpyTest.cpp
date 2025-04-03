/**
 * @file    LightControllerSpyTest.cpp
 * @author  Julio Cesar Bernal Mendez
 * @brief   Light Controller (Spy) test source file containing the test cases
 *          and test group for the Light Controller (Spy) module used inside the Light Scheduler module.
 
 * @version 0.1
 * @date    2025-04-02
 */

extern "C"
{
    /* includes for things with C linkage */
    #include "LightControllerSpy.h"
}

/* includes for things with C++ linkage */
#include "TestHarness.h"
 
TEST_GROUP( LightControllerSpy )
{
    /* define data accessible to test group members here */
 
    void setup()
    {
        /* initialization steps are executed before each TEST */
        LightController_Create();
    }
 
    void teardown()
    {
        /* clean up steps are executed after each TEST */
    }
};

TEST( LightControllerSpy, Create )
{
    /* This test looks exactly the same as NoChangeToLightsDuringInitialization from LightScheduler,
       the reason is because here we're testing the initialization of the Light Controller Spy in isolation,
       while the Light Scheduler tests the interaction between the Light Controller and other modules
       (Time Service, which has not been included yet) */
    LONGS_EQUAL( LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId() );
    LONGS_EQUAL( LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState() );
}
