#ifndef MESSENGER_XMPP_STANZA_H
#define MESSENGER_XMPP_STANZA_H

#include "xmpp_context.h"

#include <vector>
#include <string>

namespace messenger
{
namespace xmpp
{

typedef std::shared_ptr<class XmppStanza> XmppStanzaPtr;
    
class XmppStanza
{
public:
    XmppStanza(xmpp_stanza_t* stanza, bool owner = false);
    virtual ~XmppStanza();
    
    static XmppStanzaPtr NewXmppStanza(XmppContextPtr context,
                                       const std::string& name = std::string(),
                                       const std::string& type = std::string());
    
    xmpp_stanza_t* const GetXmppStanzaT() const { return m_stanza; }

    std::string GetId() const;
    std::string GetName() const;
    std::string GetType() const;
    std::string GetText() const;
    std::string GetAttribute(const std::string& key) const;
    
    void SetId(const std::string& id);
    void SetName(const std::string& name);
    void SetType(const std::string& type);
    void SetText(const std::string& text);
    void SetAttribute(const std::string& key, const std::string& val);
    
    void AddChild(XmppStanzaPtr child);
    
    XmppStanzaPtr GetChildByName(const std::string& name);
    
    XmppStanzaPtr GetFirstChild();
    XmppStanzaPtr GetNextChild();
    
private:
    XmppStanza(const XmppStanza&) = delete;
    XmppStanza& operator=(const XmppStanza&) = delete;
    
private:
    typedef std::vector<XmppStanzaPtr > XmppStanzaPtrVector;
    
    xmpp_stanza_t*      m_stanza;
    bool                m_owner;
    
    XmppStanzaPtrVector m_children;
};

}
}

#endif /* MESSENGER_XMPP_STANZA_H */
