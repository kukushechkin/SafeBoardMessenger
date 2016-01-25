#include "xmpp_logger.h"

namespace messenger
{
namespace xmpp
{

XmppLogger::XmppLogger(xmpp_log_t* logger, bool owner) :
m_logger(logger),
m_owner(owner)
{
}

XmppLogger::~XmppLogger()
{
    // Do nothing
}

XmppLoggerPtr XmppLogger::DefaultLogger(xmpp_log_level_t level)
{
    xmpp_log_t* logger = xmpp_get_default_logger(level);
    return std::make_shared<XmppLogger>(logger, true);
}
    
}
}