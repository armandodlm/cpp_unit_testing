#include <iostream>

/*
    Notes:
        
        A static Instance() will allow us to use a local static to init the instance once.
        Returning a reference (Singleton&) prevents us from allocating memory in the heap. 

        Need to delete or make private items that would allow user to make copies of singleton etc.
*/


namespace StaticTest
{
    namespace Inspection
    {
    int construction_count = 0;
    }

    class Singleton
    {
        public:

            Singleton& operator=(const Singleton&) = delete;
            Singleton(const Singleton&) = delete;

            static Singleton& Instance()
            {
                static Singleton instance;
                return instance;
            }
        
            int getCount()
            {
                return  Inspection::construction_count;
            }

        private:
            Singleton()
            {
               Inspection::construction_count++; 
            }
    };

}

#include <gtest/gtest.h>


class StaticTestTestSuite : public ::testing::Test
{
public:
    void SetUp() {}
    void TearDown() {}
};

TEST_F( StaticTestTestSuite, Invoking_Instance_Only_Constructs_Instance_Once )
{
    
    EXPECT_EQ( 0, StaticTest::Inspection::construction_count );


    EXPECT_EQ( 1, StaticTest::Singleton::Instance().getCount() );
    EXPECT_EQ( 1, StaticTest::Singleton::Instance().getCount() );
    EXPECT_EQ( 1, StaticTest::Singleton::Instance().getCount() );
    EXPECT_EQ( 1, StaticTest::Singleton::Instance().getCount() );
    EXPECT_EQ( 1, StaticTest::Singleton::Instance().getCount() );
}
