#ifndef MESSENGER_XMPP_ENV_H
#define MESSENGER_XMPP_ENV_H

#include <libstrophe/strophe.h>

#include <memory>

namespace messenger
{
namespace xmpp
{

class XmppContext;
    
class XmppEnvironment
{
public:
    XmppEnvironment();
    virtual ~XmppEnvironment();
    
    void Run(std::shared_ptr<XmppContext> context);
    void Stop(std::shared_ptr<XmppContext> context);
};
    
typedef std::shared_ptr<XmppEnvironment> XmppEnvironmentPtr;
    
}
}

#endif /* MESSENGER_XMPP_ENV_H */
