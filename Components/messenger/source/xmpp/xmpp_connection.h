#ifndef MESSENGER_XMPP_CONNECTION_H
#define MESSENGER_XMPP_CONNECTION_H

#include "xmpp_context.h"
#include "xmpp_stanza.h"

namespace messenger
{
namespace xmpp
{

typedef std::shared_ptr<class XmppConnection> XmppConnectionPtr;
    
class XmppConnection
{
public:
    XmppConnection(xmpp_conn_t* conn, bool owner = false);
    virtual ~XmppConnection();
    
    static XmppConnectionPtr NewXmppConnection(XmppContextPtr context);
    
    xmpp_conn_t* const GetXmppConnT() const { return m_conn; }
    XmppContextPtr GetContext() const;
    
    void SetJid(const std::string& jid);
    void SetPassword(const std::string& password);
    
    bool Connect(const std::string& server,
                 unsigned int port,
                 xmpp_conn_handler callback,
                 void * const userdata);
    void Disconnect();
    bool IsConnected() const { return m_isConnected; }
    
    void AddHandler(xmpp_handler handler,
                    const char * const ns,
                    const char * const name,
                    const char * const type,
                    void * const userdata);
    void DeleteHandler(xmpp_handler handler);
    
    void AddIdHandler(xmpp_handler handler,
                      const char * const id,
                      void * const userdata);
    void DeleteIdHandler(xmpp_handler handler,
                         const char * const id);
    
    void Send(XmppStanzaPtr stanza);
    
private:
    XmppConnection(const XmppConnection&) = delete;
    XmppConnection& operator=(const XmppConnection&) = delete;
    
private:
    xmpp_conn_t* m_conn;
    bool         m_owner;
    
    bool         m_isConnected;
};

}
}

#endif /* MESSENGER_XMPP_CONNECTION_H */
