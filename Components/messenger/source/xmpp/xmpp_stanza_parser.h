#ifndef MESSENGER_XMPP_STANZA_PARSER_H
#define MESSENGER_XMPP_STANZA_PARSER_H

#include "xmpp_stanza.h"
#include "xmpp_stanza_id.h"

#include <messenger/types.h>

namespace messenger
{
namespace xmpp
{

class XmppStanzaParser
{
public:
    XmppStanzaParser(XmppContextPtr context);
    virtual ~XmppStanzaParser();
    
    bool IsActiveUsersResponseStanza(XmppStanzaPtr stanza);
    bool IsMessageStanza(XmppStanzaPtr stanza);
    bool IsMessageStatusStanza(XmppStanzaPtr stanza);
    bool IsMessageErrorStanza(XmppStanzaPtr stanza);
    
    bool ParseActiveUsersResponseStanza(XmppStanzaPtr stanza, UserList& userList);
    bool ParseMessageStanza(XmppStanzaPtr stanza, Message& msg, UserId& senderId);
    bool ParseMessageStatusStanza(XmppStanzaPtr stanza, MessageId& msgId, message_status::Type& status);
    bool ParseMessageErrorStanza(XmppStanzaPtr stanza, MessageId& msgId);
    
private:
    XmppContextPtr m_context;
};
    
}
}

#endif /* MESSENGER_XMPP_STANZA_PARSER_H */
