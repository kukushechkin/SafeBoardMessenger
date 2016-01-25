#ifndef MESSENGER_BASE64_H
#define MESSENGER_BASE64_H

#include <messenger/types.h>


namespace messenger
{
namespace detail
{

class Base64
{
public:
    static std::string Encode(const Data& data);
    static Data Decode(const std::string& encodedString);
    
private:
    static bool IsBase64Char(char ch);
    
private:
    static const std::string Base64Characters;
};
    
}
}

#endif /* MESSENGER_BASE64_H */
