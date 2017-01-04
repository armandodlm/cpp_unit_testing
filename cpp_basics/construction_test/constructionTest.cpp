#include <gtest/gtest.h>

namespace ConstructionNamespace 
{
    class BaseThing
    {
        public:

            BaseThing():
             m_value( 10 ),
             m_name( "default" ){}
            
            BaseThing(const BaseThing& rhs)
            {
                m_value = rhs.m_value;
                m_name = rhs.m_name;
            }

            BaseThing& operator=(const BaseThing& rhs)
            {
                m_value = rhs.m_value;
                m_name = rhs.m_name;
                   
                return *this;
            }

            virtual void setValue( int newValue )
            {
                m_value = newValue;
            }

            virtual int valueOwned()
            {
                return m_value;
            }

            virtual void setName(std::string newName )
            {
                m_name = newName;
            }
            
            virtual std::string nameOwned()
            {
                return m_name;
            }   

        private:
            std::string m_name;
            int m_value;
    };


    class ExtendedThing : public BaseThing
    {
    
        public:
            
            ExtendedThing():
             m_valueOwnedByExtended(20),
             m_nameOfExtended("extended default"){}


            ExtendedThing(const ExtendedThing& rhs)
            :BaseThing(rhs)
            {
                m_valueOwnedByExtended = rhs.m_valueOwnedByExtended;
                m_nameOfExtended = rhs.m_nameOfExtended;
            }

            ExtendedThing& operator=(const ExtendedThing& rhs)
            {
                BaseThing::operator=( rhs );

                m_valueOwnedByExtended = rhs.m_valueOwnedByExtended;
                m_nameOfExtended = rhs.m_nameOfExtended;
                   
                return *this;
            }

            void setValue( int newValue )
            {
                m_valueOwnedByExtended = newValue;
            }

            int valueOwned()
            {
                return m_valueOwnedByExtended;
            }

            void setName(std::string newName )
            {
                m_nameOfExtended = newName;
            }
            
            std::string nameOwned()
            {
                return m_nameOfExtended; 
            }

        private:
            int m_valueOwnedByExtended;           
            std::string m_nameOfExtended;

    };

}




class ConstructionTestSuite : public ::testing::Test
{
public:

void SetUp(){}
void TearDown() {}

};


TEST_F( ConstructionTestSuite, Base_Class_Owns_Default_Init_Members )
{
    ConstructionNamespace::BaseThing thingOne;
    
    EXPECT_EQ(10, thingOne.valueOwned() );
    EXPECT_EQ("default", thingOne.nameOwned() );
}


TEST_F( ConstructionTestSuite, Extended_Class_Owns_Its_Own_Default_Init_Values )
{
    ConstructionNamespace::ExtendedThing thingOne;
    
    EXPECT_EQ(20, thingOne.valueOwned() );
    EXPECT_EQ("extended default", thingOne.nameOwned() );
}


TEST_F( ConstructionTestSuite, Extended_Has_Access_To_Base_Items )
{
    ConstructionNamespace::ExtendedThing thingOne;
    
    EXPECT_EQ(20, thingOne.valueOwned() );
    EXPECT_EQ("extended default", thingOne.nameOwned() );

    EXPECT_EQ(10, thingOne.BaseThing::valueOwned() );
    EXPECT_EQ("default", thingOne.BaseThing::nameOwned() );
}


TEST_F( ConstructionTestSuite, Both_Classes_Implement_Mutators )
{
    ConstructionNamespace::ExtendedThing thingOne;
    
    thingOne.setValue( 50 );
    thingOne.setName( "Pedro" );
    thingOne.BaseThing::setValue( 100 );
    thingOne.BaseThing::setName( "Pablo" );
    
    EXPECT_EQ(50, thingOne.valueOwned() );
    EXPECT_EQ("Pedro", thingOne.nameOwned() );

    EXPECT_EQ(100, thingOne.BaseThing::valueOwned() );
    EXPECT_EQ("Pablo", thingOne.BaseThing::nameOwned() );
}


TEST_F( ConstructionTestSuite, Custom_Copy_Constructor_Should_Copy_All_Values )
{
    using ConstructionNamespace::ExtendedThing;

    ExtendedThing thingOne;

    thingOne.setValue( 100 );
    thingOne.setName( "Light" );
    thingOne.BaseThing::setValue( 50 );
    thingOne.BaseThing::setName( "Yagami" );

    ExtendedThing thingTwo( thingOne );

    EXPECT_EQ(100, thingTwo.valueOwned() );
    EXPECT_EQ("Light", thingTwo.nameOwned() );
    EXPECT_EQ(50, thingTwo.BaseThing::valueOwned() );
    EXPECT_EQ("Yagami", thingTwo.BaseThing::nameOwned() );
}


TEST_F( ConstructionTestSuite, Custom_Assignment_Operator_Should_Copy_All_Values )
{
    using ConstructionNamespace::ExtendedThing;

    ExtendedThing thingOne;

    thingOne.setValue( 100 );
    thingOne.setName( "Rey" );
    thingOne.BaseThing::setValue( 50 );
    thingOne.BaseThing::setName( "Pembar" );

    ExtendedThing thingTwo;

    thingTwo = thingOne;

    EXPECT_EQ(100, thingTwo.valueOwned() );
    EXPECT_EQ("Rey", thingTwo.nameOwned() );
    EXPECT_EQ(50, thingTwo.BaseThing::valueOwned() );
    EXPECT_EQ("Pembar", thingTwo.BaseThing::nameOwned() );
}

