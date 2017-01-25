#include <gmock/gmock.h>
#include "soundex.h"

using ::testing::Eq;

class SoundexTestSuite : public ::testing::Test
{
public:
    Soundex soundex;
};


TEST_F( SoundexTestSuite, Retains_Single_Letter_Of_OneLetter_Word )
{
    ASSERT_THAT( soundex.encode("A") , Eq("A000") );
}


TEST_F( SoundexTestSuite, Replaces_Consonants_With_Appropriate_Digits )
{
    ASSERT_THAT( soundex.encode("Ax"), Eq("A200") );
}


TEST_F( SoundexTestSuite, Ignores_Non_Alphanumeric_Character )
{
    ASSERT_THAT( soundex.encode("A#"), Eq("A000") );
}


TEST_F( SoundexTestSuite, Encodes_Tail_Of_Word )
{
    ASSERT_THAT( soundex.encode("Almc"), Eq("A452") );
}


TEST_F( SoundexTestSuite, Encodes_Tail_Of_Word_Longer_Than_Three )
{
    ASSERT_THAT( soundex.encode("Almcr").length() , Eq(4) );
}


TEST_F( SoundexTestSuite, Drops_Vowel_Like_Letters )
{
    ASSERT_THAT( soundex.encode("Baeiouhycdl") , Eq("B234") );
}
