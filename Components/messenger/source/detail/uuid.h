#ifndef MESSENGER_UUID_H
#define MESSENGER_UUID_H

#include <string>

namespace messenger
{
namespace detail
{
    
class UUID
{
public:
    static const size_t BytesLength = 16;
    
    explicit UUID(const unsigned char bytes[BytesLength]);
    static UUID RandomUUID();
    
    std::string ToString() const;
    
private:
    unsigned char m_bytes[BytesLength];
};
    
}
}

#endif /* MESSENGER_UUID_H */
