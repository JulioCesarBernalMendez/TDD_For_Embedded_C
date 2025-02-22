#include "unity_fixture.h"
#include "DumbExample.h"

TEST_GROUP( DumbExample );

TEST_SETUP( DumbExample )
{
}

TEST_TEAR_DOWN( DumbExample )
{
}

/* TEST1 */
TEST( DumbExample, test_AverageThreeBytes_should_AverageMidRangeValue )
{
    TEST_ASSERT_EQUAL_HEX8( 40, AverageThreeBytes( 30, 40, 50 ) );
    TEST_ASSERT_EQUAL_HEX8( 40, AverageThreeBytes( 10, 70, 40 ) );
    TEST_ASSERT_EQUAL_HEX8( 33, AverageThreeBytes( 33, 33, 33 ) );
}

/* TEST2 */
TEST( DumbExample, test_AverageThreeBytes_should_AverageHighValues )
{
    TEST_ASSERT_EQUAL_HEX8( 80, AverageThreeBytes( 70, 80, 90 ) );
    TEST_ASSERT_EQUAL_HEX8( 127, AverageThreeBytes( 127, 127, 127 ) );
    TEST_ASSERT_EQUAL_HEX8( 84, AverageThreeBytes( 0, 126, 126 ) );
}

TEST_GROUP_RUNNER( DumbExample )
{
    RUN_TEST_CASE( DumbExample, test_AverageThreeBytes_should_AverageMidRangeValue );
    RUN_TEST_CASE( DumbExample, test_AverageThreeBytes_should_AverageHighValues );
}
