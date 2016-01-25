#ifndef MESSENGER_XMPP_LOGGER_H
#define MESSENGER_XMPP_LOGGER_H

#include "xmpp_env.h"

namespace messenger
{
namespace xmpp
{
 
typedef std::shared_ptr<class XmppLogger> XmppLoggerPtr;
    
class XmppLogger
{
public:
    XmppLogger(xmpp_log_t* logger, bool owner = false);
    virtual ~XmppLogger();
    
    static XmppLoggerPtr DefaultLogger(xmpp_log_level_t level);
    
    xmpp_log_t* const GetXmppLogT() const { return m_logger; }
    
private:
    xmpp_log_t* m_logger;
    bool        m_owner;
};
    
}
}

#endif /* MESSENGER_XMPP_LOGGER_H */
