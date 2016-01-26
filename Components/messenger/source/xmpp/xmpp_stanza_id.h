#ifndef MESSENGER_XMPP_STANZA_ID_H
#define MESSENGER_XMPP_STANZA_ID_H

#include <string>

namespace messenger
{
namespace xmpp
{

namespace stanza_names
{
    extern const std::string Presence;
    extern const std::string Iq;
    extern const std::string Message;
    extern const std::string Query;
    extern const std::string Content;
}

namespace stanza_ids
{
    extern const std::string GetActiveUsers;
    extern const std::string PublicKey;
}

namespace stanza_types
{
    extern const std::string Get;
    extern const std::string Set;
    extern const std::string Chat;
    extern const std::string Status;
    extern const std::string Error;
}
    
namespace stanza_attribute_keys
{
    extern const std::string Jid;
    extern const std::string To;
    extern const std::string From;
    extern const std::string ContentType;
    extern const std::string Time;
    extern const std::string Status;
    extern const std::string Encrypted;
    extern const std::string Key;
}
    
}
}

#endif /* MESSENGER_XMPP_STANZA_ID_H */
