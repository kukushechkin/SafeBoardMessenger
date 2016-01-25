#include "xmpp_env.h"
#include "xmpp_context.h"

namespace messenger
{
namespace xmpp
{

XmppEnvironment::XmppEnvironment()
{
    xmpp_initialize();
}

XmppEnvironment::~XmppEnvironment()
{
    xmpp_shutdown();
}

void XmppEnvironment::Run(std::shared_ptr<XmppContext> context)
{
    xmpp_run(context->GetXmppCtxT());
}

void XmppEnvironment::Stop(std::shared_ptr<XmppContext> context)
{
    xmpp_stop(context->GetXmppCtxT());
}
    
}
}