#include "xmpp_stanza_parser.h"
#include "../detail/base64.h"

namespace messenger
{
namespace xmpp
{

XmppStanzaParser::XmppStanzaParser(XmppContextPtr context) :
m_context(context)
{
    
}

XmppStanzaParser::~XmppStanzaParser()
{
}
 
bool XmppStanzaParser::IsActiveUsersResponseStanza(XmppStanzaPtr stanza)
{
    return stanza->GetName() == xmpp::stanza_names::Iq &&
        stanza->GetId() == xmpp::stanza_ids::GetActiveUsers;
}

bool XmppStanzaParser::IsMessageStanza(XmppStanzaPtr stanza)
{
    return stanza->GetName() == xmpp::stanza_names::Message &&
        stanza->GetType() == xmpp::stanza_types::Chat;
}
    
bool XmppStanzaParser::IsMessageStatusStanza(XmppStanzaPtr stanza)
{
    return stanza->GetName() == xmpp::stanza_names::Message &&
        stanza->GetType() == xmpp::stanza_types::Status;
}

bool XmppStanzaParser::IsMessageErrorStanza(XmppStanzaPtr stanza)
{
    return stanza->GetName() == xmpp::stanza_names::Message &&
        stanza->GetType() == xmpp::stanza_types::Error;
}
    
bool XmppStanzaParser::ParseActiveUsersResponseStanza(XmppStanzaPtr stanza, UserList& userList)
{
    bool result = false;
    if (IsActiveUsersResponseStanza(stanza))
    {
        xmpp::XmppStanzaPtr query = stanza->GetChildByName(xmpp::stanza_names::Query);
        if (!!query)
        {
            for (xmpp::XmppStanzaPtr item = query->GetFirstChild(); !!item; item = item->GetNextChild())
            {
                User user;
                user.identifier = item->GetAttribute(xmpp::stanza_attribute_keys::Jid);
                // TODO: read security policy
                userList.push_back(user);
            }
        }
        
        result = true;
    }
    return result;
}

bool XmppStanzaParser::ParseMessageStanza(XmppStanzaPtr stanza, Message& msg, UserId& senderId)
{
    bool result = false;
    if (IsMessageStanza(stanza))
    {
        msg.identifier = stanza->GetId();
        msg.time = std::stol(stanza->GetAttribute(stanza_attribute_keys::Time));
        
        XmppStanzaPtr contentStanza = stanza->GetChildByName(stanza_names::Content);
        msg.content.type = static_cast<message_content_type::Type>(std::stoi(contentStanza->GetAttribute(stanza_attribute_keys::ContentType)));
        msg.content.encrypted = static_cast<bool>(std::stoi(contentStanza->GetAttribute(stanza_attribute_keys::Encrypted)));
        
        std::string base64Data = contentStanza->GetText();
        msg.content.data = detail::Base64::Decode(base64Data);
        
        senderId = stanza->GetAttribute(xmpp::stanza_attribute_keys::From);
        std::string::size_type pos = senderId.find('/');
        senderId.erase(pos);
        
        result = true;
    }
    return result;
}

bool XmppStanzaParser::ParseMessageStatusStanza(XmppStanzaPtr stanza, MessageId& msgId, message_status::Type& status)
{
    bool result = false;
    if (IsMessageStatusStanza(stanza))
    {
        msgId = stanza->GetId();
        status = static_cast<message_status::Type>(std::stoi(stanza->GetAttribute(stanza_attribute_keys::Status)));
        
        result = true;
    }
    return result;
}

bool XmppStanzaParser::ParseMessageErrorStanza(XmppStanzaPtr stanza, MessageId& msgId)
{
    bool result = false;
    if (IsMessageErrorStanza(stanza))
    {
        msgId = stanza->GetId();
        
        result = true;
    }
    return result;
}
    
}
}
