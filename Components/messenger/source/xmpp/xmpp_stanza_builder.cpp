#include "xmpp_stanza_builder.h"
#include "../detail/base64.h"

namespace messenger
{
namespace xmpp
{

XmppStanzaBuilder::XmppStanzaBuilder(XmppContextPtr context) :
m_context(context)
{
}

XmppStanzaBuilder::~XmppStanzaBuilder()
{
}
    
XmppStanzaPtr XmppStanzaBuilder::CreatePresenceStanza()
{
    return XmppStanza::NewXmppStanza(m_context, stanza_names::Presence);
}
    
XmppStanzaPtr XmppStanzaBuilder::CreateRequestActiveUsersStanza()
{
    XmppStanzaPtr stanza = XmppStanza::NewXmppStanza(m_context,
                                                     stanza_names::Iq,
                                                     stanza_types::Get);
    stanza->SetId(stanza_ids::GetActiveUsers);
    return stanza;
}
    
XmppStanzaPtr XmppStanzaBuilder::CreateSetPublicKeyStanza(const SecPublicKey& pubKey)
{
    XmppStanzaPtr stanza = XmppStanza::NewXmppStanza(m_context,
                                                     stanza_names::Iq,
                                                     stanza_types::Set);
    stanza->SetId(stanza_ids::PublicKey);
    
    std::string base64PubKey = detail::Base64::Encode(pubKey);
    stanza->SetAttribute(stanza_attribute_keys::Key, base64PubKey);
    return stanza;
}
    
XmppStanzaPtr XmppStanzaBuilder::CreateMessageStanza(const UserId& recepientId, const Message& msg)
{
    XmppStanzaPtr msgStanza = XmppStanza::NewXmppStanza(m_context,
                                                        stanza_names::Message,
                                                        stanza_types::Chat);
    msgStanza->SetId(msg.identifier);
    msgStanza->SetAttribute(stanza_attribute_keys::To, recepientId);
    msgStanza->SetAttribute(stanza_attribute_keys::Time, std::to_string(msg.time));
    
    XmppStanzaPtr contentStanza = XmppStanza::NewXmppStanza(m_context, stanza_names::Content);
    contentStanza->SetAttribute(stanza_attribute_keys::ContentType, std::to_string(static_cast<int>(msg.content.type)));
    contentStanza->SetAttribute(stanza_attribute_keys::Encrypted, std::to_string(static_cast<int>(msg.content.encrypted)));
    
    XmppStanzaPtr dataStanza = XmppStanza::NewXmppStanza(m_context);
    
    std::string base64Data = detail::Base64::Encode(msg.content.data);
    dataStanza->SetText(base64Data);
    
    contentStanza->AddChild(dataStanza);
    msgStanza->AddChild(contentStanza);
    
    return msgStanza;
}
    
XmppStanzaPtr XmppStanzaBuilder::CreateMessageStatusStanza(const UserId& userId, const MessageId& msgId, message_status::Type status)
{
    XmppStanzaPtr statusStanza = XmppStanza::NewXmppStanza(m_context,
                                                           stanza_names::Message,
                                                           stanza_types::Status);
    statusStanza->SetId(msgId);
    statusStanza->SetAttribute(stanza_attribute_keys::To, userId);
    statusStanza->SetAttribute(stanza_attribute_keys::Status, std::to_string(static_cast<int>(status)));
    return statusStanza;
}
    
}
}