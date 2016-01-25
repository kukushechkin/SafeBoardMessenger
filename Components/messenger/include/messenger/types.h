#ifndef MESSENGER_TYPES_H
#define MESSENGER_TYPES_H

#include <ctime>
#include <string>
#include <vector>

namespace messenger
{
    
namespace operation_result
{
    enum Type
    {
        Ok,
        AuthError,
        NetworkError,
        InternalError
    };
}
   
namespace message_status
{
    enum Type
    {
        Sending,
        Sent,
        FailedToSend,
        Delivered,
        Seen
    };
}
    
namespace message_content_type
{
    enum Type
    {
        Text,
        Image,
        Video
    };
}
    
namespace encryption_algorithm
{
    enum Type
    {
        None,
        RSA_1024
    };
}
    
typedef std::vector<unsigned char> Data;
typedef std::vector<unsigned char> SecPublicKey;
typedef std::string MessageId;
typedef std::string UserId;

struct MessageContent
{
    message_content_type::Type type;
    bool                       encrypted;
    Data                       data;
    
    MessageContent() : type(message_content_type::Text), encrypted(false) {}
};
    
struct Message
{
    MessageId      identifier;
    std::time_t    time;
    MessageContent content;
    
    Message() : time(0) {}
};

struct SecurityPolicy
{
    encryption_algorithm::Type encryptionAlgo;
    SecPublicKey               encryptionPubKey;
    
    SecurityPolicy() : encryptionAlgo(encryption_algorithm::None) {}
};

struct User
{
    UserId         identifier;
    SecurityPolicy securityPolicy;
};

typedef std::vector<User> UserList;
    
}

#endif /* MESSENGER_TYPES_H */
