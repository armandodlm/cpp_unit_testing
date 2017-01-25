#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <string>
#include <unordered_map>

namespace
{
static const size_t MaxCodeLength{4};

static const std::unordered_map< char, std::string > encodings{
    { 'b', "1" },{ 'f', "1" },{ 'p', "1" },{ 'v', "1" }, 
    { 'c', "2" },{ 'g', "2" },{ 'j', "2" },{ 'k', "2" },{ 'q', "2" },{ 's', "2" },{ 'x', "2" },
                 { 'z', "2" },
    { 'd', "3" },{ 't', "3" },
    { 'l', "4" }, 
    { 'm', "5" }, { 'n', "5" },
    { 'r', "6" }
};
}

class Soundex
{
    public:
        std::string encode(const std::string& word) const
        {
            return zeroPad( head( word ) + encodeDigits( tail( word ) ) );
        }

    private:
    
        std::string head( const std::string& word ) const
        {
            return word.substr(0,1);
        }

        std::string tail( const std::string& word ) const
        {
            return word.substr(1);
        }

        std::string encodeDigits( const std::string& word) const
        {
            std::string encoded; 

            for( auto letter : word )
            {
                if( isComplete( encoded ) ) break;
                encoded += encodeDigit( letter );
            }
            return encoded;
        }

        bool isComplete( std::string word ) const
        {
            return word.length() == MaxCodeLength - 1;
        }


        std::string encodeDigit( const char letter ) const
        {
            auto encoded = encodings.find( letter );
            return encoded == encodings.end() ? "" : encoded->second;
        }


        std::string zeroPad( const std::string& word ) const 
        {
            auto zeroes = MaxCodeLength - word.length();
            return word + std::string( zeroes, '0' ); 
        }


};

#endif
