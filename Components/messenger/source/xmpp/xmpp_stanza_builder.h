#ifndef MESSENGER_XMPP_STANZA_BUILDER_H
#define MESSENGER_XMPP_STANZA_BUILDER_H

#include "xmpp_stanza.h"
#include "xmpp_stanza_id.h"

#include <messenger/types.h>

namespace messenger
{
namespace xmpp
{
 
class XmppStanzaBuilder
{
public:
    XmppStanzaBuilder(XmppContextPtr context);
    virtual ~XmppStanzaBuilder();
    
    XmppStanzaPtr CreatePresenceStanza();
    XmppStanzaPtr CreateRequestActiveUsersStanza();
    XmppStanzaPtr CreateSetPublicKeyStanza(const SecPublicKey& pubKey);
    XmppStanzaPtr CreateMessageStanza(const UserId& recepientId, const Message& msg);
    XmppStanzaPtr CreateMessageStatusStanza(const UserId& userId, const MessageId& msgId, message_status::Type status);
    
private:
    XmppContextPtr m_context;
};
    
}
}

#endif /* MESSENGER_XMPP_STANZA_BUILDER_H */
