#include "ConsterInterface.h"
#include <gtest/gtest.h>

namespace
{
bool s_operationSuccessful = false;
bool s_constInvoked = false;
int s_accessCounter = 0;
}

namespace ConstNamespace
{

class Conster : public ConsterInterface
{

public:

    Conster() : m_value( 10 ), m_isReady( false ){}

    void constInvokedThroughNonConstMethod() const
    {
        s_constInvoked = true;
    }

    void constInvokedThroughNonConstMethod()
    {
        static_cast<const Conster&>(*this).constInvokedThroughNonConstMethod();
    }

    int getValue() const
    {
        s_constInvoked = true;
        return m_value;   
    }

    int getValue()
    {
        s_constInvoked = false;
        return m_value;
    }

    void setValue(const int value )
    {
        s_constInvoked = false;
        s_operationSuccessful = true;
        m_value = value;
    }       

    void setValue( const int value ) const
    {
        s_constInvoked = true;
        s_operationSuccessful = false;
    }       
    
    void nonOverloaded() const
    {
        s_constInvoked = true;
    }


    bool isReady() const
    {
        s_constInvoked = true;
        // it'll be ready after first invocation
        if( !m_isReady )
        {
            s_accessCounter++;
            // do some initial setup that is never repeated
            m_isReady = true;
        }
        return m_isReady;
    }


    Conster& operator=( const Conster& rhs )
    {
        // Just for academic reasons:
        rhs.constInvokedThroughNonConstMethod();
        m_value = rhs.m_value;
        return *this;
    } 


private:
    int m_value;
    mutable bool m_isReady; // using mutable to allow isReady to init it 

};

}




class ConsterTestSuite : public ::testing::Test
{
public:

void SetUp()
{
    s_operationSuccessful = false;
    s_constInvoked = false;
}

void TearDown() {}

};


TEST_F( ConsterTestSuite, Non_Const_Conster_Uses_Non_Const_Method )
{
    ConstNamespace::Conster c;

    EXPECT_FALSE( s_constInvoked );
    EXPECT_EQ( 10, c.getValue() );
    EXPECT_FALSE( s_constInvoked );
}


TEST_F( ConsterTestSuite, Const_Conster_Uses_Const_Method )
{
    const ConstNamespace::Conster c;

    EXPECT_FALSE( s_constInvoked );
    EXPECT_EQ( 10, c.getValue() );
    EXPECT_TRUE( s_constInvoked );
}


TEST_F( ConsterTestSuite, Non_Const_Conster_Invokes_Non_Const_Method_But_Logic_Accesses_Const_Method )
{
    ConstNamespace::Conster c;
    EXPECT_FALSE( s_constInvoked );
    
    // This should invoke the non-const overload of constInvoked...
    c.constInvokedThroughNonConstMethod();

    EXPECT_TRUE( s_constInvoked );
}


TEST_F( ConsterTestSuite, Const_Conster_Invokes_Const_Method_Which_Contains_Shared_Logic )
{
    const ConstNamespace::Conster c;
    EXPECT_FALSE( s_constInvoked );
    
    c.constInvokedThroughNonConstMethod();

    EXPECT_TRUE( s_constInvoked );
}


TEST_F( ConsterTestSuite, Const_Method_Can_Be_Accessed_By_Non_Const )
{
    ConstNamespace::Conster c;
    EXPECT_FALSE( s_constInvoked );

    c.nonOverloaded();

    EXPECT_TRUE( s_constInvoked );
    
}

TEST_F( ConsterTestSuite, Const_Method_Fails_To_Change_Value )
{
    const ConstNamespace::Conster c;
    EXPECT_FALSE( s_constInvoked );
    EXPECT_EQ( 10, c.getValue() );
    
    c.setValue( 30 );

    EXPECT_EQ( 10, c.getValue() );
    EXPECT_FALSE( s_operationSuccessful );
    EXPECT_TRUE( s_constInvoked );
}


TEST_F( ConsterTestSuite, Non_Const_Conster_Passed_As_const_Reference_Uses_Const_Methods )
{
    EXPECT_FALSE( s_constInvoked );
 
    ConstNamespace::Conster c;   
    ConstNamespace::Conster c2;

    // c2 should be received as a const ref
    c = c2;

    EXPECT_TRUE( s_constInvoked );
}


TEST_F( ConsterTestSuite, isReady_Changes_Mutable_Const_Member_Only_Once )
{
    ConstNamespace::Conster c;

    EXPECT_FALSE( s_constInvoked );

    EXPECT_EQ(0, s_accessCounter );
    EXPECT_TRUE( c.isReady() );
    EXPECT_EQ(1, s_accessCounter );
    EXPECT_TRUE( c.isReady() );
    EXPECT_EQ(1, s_accessCounter );
    EXPECT_TRUE( c.isReady() );
    EXPECT_EQ(1, s_accessCounter );

    EXPECT_TRUE( s_constInvoked );
}

/*

Notes:
    A const instance of an object can only use const methods
        i.e. void doStuff() const

    A const instance of an object can be const_cast to a reference of a non-const object of the same type
        const_cast<Conster &>( constInstance ) -> Returns a reference to the non-const form of constInstance


    A const method can be accessed by both const and non-const instances

    We can overload methods by just changing their const-ness 
        i.e. 
            void doStuff() const != void doStuff()
    
    When an overloaded (by const) method is invoked a non-const instance will invoke the non-const method and viceversa  


    When passing a value as a const reference, the constness of the object will make it use the const methods if availalbe as overloads

    When a const and non-const method have the same implementation we can cast the this ref to const and invoke the const method
        instead of duplicating the logic
        
            static_cast< const type& >( *this ).doStuff(); // which will invoke the const version


    When a const method needs to modify a private member (and meets logical constness) you can use the "mutable" prefix
        private:
            mutable bool member;

    The compilers check for bitwise constness, mutable allows you to preserve logical, not bitwise constness

*/

