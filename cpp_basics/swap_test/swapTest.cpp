#include <gtest/gtest.h>
#include <vector>

namespace
{
bool member_swap_accessed = false;
}


namespace SwapNamespace
{
    
    class HeavyProperty
    {
        public:
            HeavyProperty(): m_heavy_vector( std::vector<int>( 100, 10 ) )
            {
            }
            
            HeavyProperty(int value): m_heavy_vector( std::vector<int>( 100, value ) )
            {
            }

            const std::vector<int>& heavyVector() const
            {
                return m_heavy_vector;
            }

        private:
            std::vector<int> m_heavy_vector;
    };


    class Swapper
    {  
        public:
            Swapper() : m_heavy( new HeavyProperty() )
            {
            }
            
            Swapper(int value ): m_heavy(new HeavyProperty( value ))
            {
            }

            ~Swapper()
            {
                delete m_heavy;
            }

            void swap( Swapper& a )
            {
                member_swap_accessed = true;
                using std::swap;
                
                swap( m_heavy, a.m_heavy );
                
            }

            const std::vector<int>& content() const
            {
                return m_heavy->heavyVector();
            }
            

        private:
            HeavyProperty *m_heavy;
    };
}


namespace std
{
    template<>
    void swap<SwapNamespace::Swapper>(SwapNamespace::Swapper& a, SwapNamespace::Swapper& b) noexcept
    {
        a.swap(b);
    }
}


class SwapperTestSuite : public ::testing::Test
{
public:
    void SetUp(){ member_swap_accessed = false; }
    void TearDown(){}
};


TEST_F( SwapperTestSuite, STD_Swap_Uses_Specialized_Swap  )
{
    using std::swap;

    EXPECT_FALSE( member_swap_accessed );   

    SwapNamespace::Swapper a, b;

    swap(a, b);

    EXPECT_TRUE( member_swap_accessed );   
    
}

TEST_F( SwapperTestSuite, Initializing_With_Number_Sets_All_Elements_To_That_Number )
{
    SwapNamespace::Swapper a( 5 );
    
    for( auto element : a.content() )
    {
        EXPECT_EQ( 5, element );
    }
}


TEST_F( SwapperTestSuite, Swap_Successfully_Swaps_Vector_Inside_Swapper_Instances  )
{
    using std::swap;

    SwapNamespace::Swapper twentySwapper( 20 );
    SwapNamespace::Swapper thirtySwapper( 30 );

    for( auto i = 0; i < twentySwapper.content().size(); i++ )
    {
        EXPECT_EQ( 20, twentySwapper.content()[ i ] );
        EXPECT_EQ( 30, thirtySwapper.content()[ i ] );
    }

    swap(twentySwapper, thirtySwapper);

    for( auto i = 0; i < twentySwapper.content().size(); i++ )
    {
        EXPECT_EQ( 30, twentySwapper.content()[ i ] );
        EXPECT_EQ( 20, thirtySwapper.content()[ i ] );
    }
}
