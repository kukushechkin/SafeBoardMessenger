#include "xmpp_stanza.h"

namespace messenger
{
namespace xmpp
{
    
XmppStanza::XmppStanza(xmpp_stanza_t* stanza, bool owner) :
m_stanza(stanza),
m_owner(owner)
{
}

XmppStanza::~XmppStanza()
{
    if (m_owner)
        xmpp_stanza_release(m_stanza);
}

XmppStanzaPtr XmppStanza::NewXmppStanza(XmppContextPtr context,
                                        const std::string& name,
                                        const std::string& type)
{
    xmpp_stanza_t* stanza = xmpp_stanza_new(context->GetXmppCtxT());
    XmppStanzaPtr stanzaObj = std::make_shared<XmppStanza>(stanza, true);
    
    if (!name.empty())
        stanzaObj->SetName(name);
    if (!type.empty())
        stanzaObj->SetType(type);

    return stanzaObj;
}
 
std::string XmppStanza::GetId() const
{
    const char *id = xmpp_stanza_get_id(m_stanza);
    return id ? id : "";
}

std::string XmppStanza::GetName() const
{
    const char *name = xmpp_stanza_get_name(m_stanza);
    return name ? name : "";
}

std::string XmppStanza::GetType() const
{
    const char *type = xmpp_stanza_get_type(m_stanza);
    return type ? type : "";
}

std::string XmppStanza::GetText() const
{
    const char *text = xmpp_stanza_get_text(m_stanza);
    return text ? text : "";
}

std::string XmppStanza::GetAttribute(const std::string& key) const
{
    const char *attribute = xmpp_stanza_get_attribute(m_stanza, key.c_str());
    return attribute ? attribute : "";
}
    
void XmppStanza::SetId(const std::string& id)
{
    xmpp_stanza_set_id(m_stanza, id.c_str());
}

void XmppStanza::SetName(const std::string& name)
{
    xmpp_stanza_set_name(m_stanza, name.c_str());
}

void XmppStanza::SetType(const std::string& type)
{
    xmpp_stanza_set_type(m_stanza, type.c_str());
}

void XmppStanza::SetText(const std::string& text)
{
    xmpp_stanza_set_text(m_stanza, text.c_str());
}

void XmppStanza::SetAttribute(const std::string& key, const std::string& val)
{
    xmpp_stanza_set_attribute(m_stanza, key.c_str(), val.c_str());
}
    
void XmppStanza::AddChild(std::shared_ptr<XmppStanza> child)
{
    xmpp_stanza_add_child(m_stanza, child->GetXmppStanzaT());
    m_children.push_back(child);
}
    
XmppStanzaPtr XmppStanza::GetChildByName(const std::string &name)
{
    xmpp_stanza_t* child = xmpp_stanza_get_child_by_name(m_stanza, name.c_str());
    return child != NULL ? std::make_shared<XmppStanza>(child) : XmppStanzaPtr();
}
 
XmppStanzaPtr XmppStanza::GetFirstChild()
{
    xmpp_stanza_t* child = xmpp_stanza_get_children(m_stanza);
    return child != NULL ? std::make_shared<XmppStanza>(child) : XmppStanzaPtr();
}

XmppStanzaPtr XmppStanza::GetNextChild()
{
    xmpp_stanza_t* child = xmpp_stanza_get_next(m_stanza);
    return child != NULL ? std::make_shared<XmppStanza>(child) : XmppStanzaPtr();
}
    
}
}