#include "xmpp_logger.h"

#include <android/log.h>

namespace messenger
{
namespace xmpp
{

const int LogPriorityMap[]
{
	ANDROID_LOG_DEBUG,
	ANDROID_LOG_INFO,
	ANDROID_LOG_WARN,
	ANDROID_LOG_ERROR
};

static void xmpp_android_logger(void * const userdata,
	const xmpp_log_level_t level,
	const char * const area,
	const char * const msg)
{
	xmpp_log_level_t filter_level = *(xmpp_log_level_t*)userdata;
	if (level >= filter_level)
	{
		__android_log_print(LogPriorityMap[level], "xmpp", "%s %s", area, msg);
	}
}

xmpp_log_level_t LogLevel = XMPP_LEVEL_DEBUG;
xmpp_log_t AndroidLogger = { &xmpp_android_logger, &LogLevel };

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
	LogLevel = level;
	return std::make_shared<XmppLogger>(&AndroidLogger, true);
}
    
}
}
