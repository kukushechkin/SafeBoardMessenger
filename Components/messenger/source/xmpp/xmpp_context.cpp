#include "xmpp_context.h"


namespace messenger
{
namespace xmpp
{
    
XmppContext::XmppContext(xmpp_ctx_t* ctx, bool owner) :
m_ctx(ctx),
m_owner(owner)
{
}

XmppContext::~XmppContext()
{
    if (m_owner)
        xmpp_ctx_free(m_ctx);
}

XmppContextPtr XmppContext::NewXmppContext(XmppEnvironmentPtr env, XmppLoggerPtr logger)
{
    xmpp_log_t* log = !!logger ? logger->GetXmppLogT() : NULL;
    xmpp_ctx_t* ctx = xmpp_ctx_new(NULL, log);
    return std::make_shared<XmppContext>(ctx, true);
}
    
}
}