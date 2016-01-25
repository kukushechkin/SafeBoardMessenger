#include "xmpp_connection.h"

namespace messenger
{
namespace xmpp
{

XmppConnection::XmppConnection(xmpp_conn_t* conn, bool owner) :
m_conn(conn),
m_owner(owner),
m_isConnected(false)
{
}

XmppConnection::~XmppConnection()
{
    if (m_owner)
    {
        Disconnect();
        xmpp_conn_release(m_conn);
    }
}
    
XmppConnectionPtr XmppConnection::NewXmppConnection(XmppContextPtr context)
{
    xmpp_conn_t* conn = xmpp_conn_new(context->GetXmppCtxT());
    return std::make_shared<XmppConnection>(conn, true);
}
    
XmppContextPtr XmppConnection::GetContext() const
{
    return std::make_shared<xmpp::XmppContext>(xmpp_conn_get_context(m_conn));
}
    
void XmppConnection::SetJid(const std::string& jid)
{
    xmpp_conn_set_jid(m_conn, jid.c_str());
}

void XmppConnection::SetPassword(const std::string& password)
{
    xmpp_conn_set_pass(m_conn, password.c_str());
}

bool XmppConnection::Connect(const std::string& server,
                             unsigned int port,
                             xmpp_conn_handler callback,
                             void * const userdata)
{
    m_isConnected = xmpp_connect_client(m_conn,
                                        server.c_str(),
                                        port,
                                        callback,
                                        userdata) == 0;
    return m_isConnected;
}

void XmppConnection::Disconnect()
{
    if (m_isConnected)
    {
        xmpp_disconnect(m_conn);
        m_isConnected = false;
    }
}
    
void XmppConnection::AddHandler(xmpp_handler handler,
                                const char * const ns,
                                const char * const name,
                                const char * const type,
                                void * const userdata)
{
    xmpp_handler_add(m_conn,
                     handler,
                     ns,
                     name,
                     type,
                     userdata);
}

void XmppConnection::DeleteHandler(xmpp_handler handler)
{
    xmpp_handler_delete(m_conn, handler);
}
    
void XmppConnection::AddIdHandler(xmpp_handler handler,
                                  const char * const id,
                                  void * const userdata)
{
    xmpp_id_handler_add(m_conn,
                        handler,
                        id,
                        userdata);
}

void XmppConnection::DeleteIdHandler(xmpp_handler handler,
                                     const char * const id)
{
    xmpp_id_handler_delete(m_conn, handler, id);
}
    
void XmppConnection::Send(XmppStanzaPtr stanza)
{
    xmpp_send(m_conn, stanza->GetXmppStanzaT());
}
    
}
}
