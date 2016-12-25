#include <iostream>
#include <gtest/gtest.h>

class unit_test : public ::testing::Test
{

    void SetUp()
    {
    }       

    void TearDown()
    {
    }       

};


TEST_F( unit_test, EmptyTest )
{
}
