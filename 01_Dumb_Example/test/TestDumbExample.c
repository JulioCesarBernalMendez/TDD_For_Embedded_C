#include "../../unity/src/unity.h"
#include "../src/DumbExample.h"

void setUp( void ) {}
void tearDown( void ) {}

/* TEST 1 */
void test_AverageThreBytes_should_AverageMidRangeValue( void )
{
    TEST_ASSERT_EQUAL_HEX8( 40, AverageThreeBytes( 30, 40, 50 ) );
    TEST_ASSERT_EQUAL_HEX8( 40, AverageThreeBytes( 10, 70, 40 ) );
    TEST_ASSERT_EQUAL_HEX8( 33, AverageThreeBytes( 33, 33, 33 ) );
}

/* TEST 2 */
void test_AverageThreeBytes_should_AverageHighValues( void )
{
    TEST_ASSERT_EQUAL_HEX8( 80, AverageThreeBytes( 70, 80, 90 ) );
    TEST_ASSERT_EQUAL_HEX8( 127, AverageThreeBytes( 127, 127, 127 ) );
    TEST_ASSERT_EQUAL_HEX8( 84, AverageThreeBytes( 0, 126, 126 ) );
}

int main( void )
{
    UNITY_BEGIN();

    RUN_TEST( test_AverageThreBytes_should_AverageMidRangeValue );
    RUN_TEST( test_AverageThreeBytes_should_AverageHighValues );

    return UNITY_END();
}
