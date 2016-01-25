#ifndef MESSENGER_XMPP_CONTEXT_H
#define MESSENGER_XMPP_CONTEXT_H

#include "xmpp_env.h"
#include "xmpp_logger.h"

namespace messenger
{
namespace xmpp
{
 
typedef std::shared_ptr<class XmppContext> XmppContextPtr;
    
class XmppContext
{
public:
    XmppContext(xmpp_ctx_t* ctx, bool owner = false);
    virtual ~XmppContext();

    static XmppContextPtr NewXmppContext(XmppEnvironmentPtr env, XmppLoggerPtr logger);
    
    xmpp_ctx_t* const GetXmppCtxT() const { return m_ctx; }
    
private:
    XmppContext(const XmppContext&) = delete;
    XmppContext& operator=(const XmppContext&) = delete;
   
private:
    xmpp_ctx_t* m_ctx;
    bool        m_owner;
};
   
}
}

#endif /* MESSENGER_XMPP_CONTEXT_H */
