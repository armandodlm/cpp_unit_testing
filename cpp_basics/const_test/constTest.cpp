#include "ConsterInterface.h"
#include <gtest/gtest.h>


namespace ConstNamespace
{

class Conster : public ConsterInterface
{

public:
    Conster() :
     m_value( 10 ),
     m_constInvoked( false ),
     m_isReady( false ),
     m_operationSuccessful( false ) {} 

    void constInvokedThroughNonConstMethod() const
    {
        m_constInvoked = true;
    }

    void constInvokedThroughNonConstMethod()
    {
       m_constInvoked = false;
       static_cast<const Conster&>(*this).constInvokedThroughNonConstMethod();
    }

    int getValue() const
    {
        m_constInvoked = true;
        return m_value;   
    }

    int getValue()
    {
        m_constInvoked = false;
        return m_value;
    }

    void setValue(const int value )
    {
        m_constInvoked = false;
        m_operationSuccessful = true;
        m_value = value;
    }       

    void setValue( const int value ) const
    {
        m_constInvoked = true;
        m_operationSuccessful = false;
    }       
    
    void onlyNonOverloaded() const
    {
        m_constInvoked = true;
    }


    bool isReady() const
    {
        // it'll be ready after first invocation
        if( !m_isReady )
        { 
            // do some initial setup that is never repeated
            m_isReady = true;
        }
        return m_isReady;
    }
    
    bool wasConstMethodUsed() const
    {
        return m_constInvoked;
    }

    /*
    // Just for academic reasons:
    Conster& operator=( const Conster& rhs )
    {
        rhs.constInvokedThroughNonConstMethod();
        m_value = rhs.m_value;
        return *this;
    }*/ 

    bool successfulOp() const
    {
        return m_operationSuccessful;
    }

private:
    int m_value;
    mutable bool m_constInvoked;
    mutable bool m_isReady; // using mutable to allow is ready to init it 
    mutable bool m_operationSuccessful;

};

}




class ConsterTestSuite : public ::testing::Test
{
public:

void SetUp() {}
void TearDown() {}

};


TEST_F( ConsterTestSuite, Non_Const_Conster_Uses_Non_Const_Method )
{
    ConstNamespace::Conster c;

    EXPECT_FALSE( c.wasConstMethodUsed() );
    EXPECT_EQ( 10, c.getValue() );
    EXPECT_FALSE( c.wasConstMethodUsed() );
}


TEST_F( ConsterTestSuite, Const_Conster_Uses_Const_Method )
{
    const ConstNamespace::Conster c;

    EXPECT_FALSE( c.wasConstMethodUsed() );
    EXPECT_EQ( 10, c.getValue() );
    EXPECT_TRUE( c.wasConstMethodUsed() );
}


TEST_F( ConsterTestSuite, Non_Const_Conster_Invokes_Non_Const_Method_But_Logic_Accesses_Const_Method )
{
    ConstNamespace::Conster c;
    EXPECT_FALSE( c.wasConstMethodUsed() );
    
    // This should invoke the non-const overload of constInvoked...
    c.constInvokedThroughNonConstMethod();

    EXPECT_TRUE( c.wasConstMethodUsed() );
}


TEST_F( ConsterTestSuite, Const_Conster_Invokes_Const_Method_Which_Contains_Shared_Logic )
{
    const ConstNamespace::Conster c;
    EXPECT_FALSE( c.wasConstMethodUsed() );
    
    c.constInvokedThroughNonConstMethod();

    EXPECT_TRUE( c.wasConstMethodUsed() );
}


TEST_F( ConsterTestSuite, Const_Method_Can_Be_Accessed_By_Non_Const )
{
    ConstNamespace::Conster c;
    EXPECT_FALSE( c.wasConstMethodUsed() );

    c.onlyNonOverloaded();

    EXPECT_TRUE( c.wasConstMethodUsed() );
    
}

TEST_F( ConsterTestSuite, Const_Method_Fails_To_Change_Value )
{
    const ConstNamespace::Conster c;
    EXPECT_FALSE( c.wasConstMethodUsed() );
    EXPECT_EQ( 10, c.getValue() );
    
    c.setValue( 30 );

    EXPECT_EQ( 10, c.getValue() );
    EXPECT_FALSE( c.successfulOp() );
    EXPECT_TRUE( c.wasConstMethodUsed() );
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

