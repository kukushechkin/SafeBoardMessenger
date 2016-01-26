#include "xmpp_stanza_id.h"


namespace messenger
{
namespace xmpp
{

namespace stanza_names
{
    const std::string Presence = "presence";
    const std::string Iq = "iq";
    const std::string Message = "message";
    const std::string Query = "query";
    const std::string Content = "content";
}

namespace stanza_ids
{
    const std::string GetActiveUsers = "get-active-users";
    const std::string PublicKey = "public-key";
}
    
namespace stanza_types
{
    const std::string Get = "get";
    const std::string Set = "set";
    const std::string Chat = "chat";
    const std::string Status = "status";
    const std::string Error = "error";
}

namespace stanza_attribute_keys
{
    const std::string Jid = "jid";
    const std::string To = "to";
    const std::string From = "from";
    const std::string ContentType = "content_type";
    const std::string Time = "time";
    const std::string Status = "status";
    const std::string Encrypted = "encrypted";
    const std::string Key = "key";
}
    
}
}