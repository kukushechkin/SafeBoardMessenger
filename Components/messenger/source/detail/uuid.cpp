#include "uuid.h"
#include "compatibility.h"

#include <random>

namespace messenger
{
namespace detail
{
    
UUID::UUID(const unsigned char bytes[BytesLength])
{
    memcpy(m_bytes, bytes, sizeof(m_bytes));
}

UUID UUID::RandomUUID()
{
    std::random_device rd;
    
    unsigned r1 = rd(),
             r2 = rd(),
             r3 = rd(),
             r4 = rd();
    
    unsigned char bytes[BytesLength];
    memcpy(bytes, &r1, sizeof(int));
    memcpy(bytes + sizeof(int), &r2, sizeof(int));
    memcpy(bytes + 2 * sizeof(int), &r3, sizeof(int));
    memcpy(bytes + 3 * sizeof(int), &r4, sizeof(int));
    
    return UUID(bytes);
}

std::string UUID::ToString() const
{
    char str[40];
    sprintf_s(str, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", m_bytes[0], m_bytes[1], m_bytes[2], m_bytes[3], m_bytes[4], m_bytes[5], m_bytes[6], m_bytes[7], m_bytes[8], m_bytes[9], m_bytes[10], m_bytes[11], m_bytes[12], m_bytes[13], m_bytes[14], m_bytes[15]);
    return std::string(str);
}
    
}
}