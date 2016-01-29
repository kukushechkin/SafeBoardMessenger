#ifndef MESSENGER_IMPL_H
#define MESSENGER_IMPL_H

#include <queue>
#include <mutex>
#include <unordered_set>
#include <thread>

#include <messenger/messenger.h>

#include "xmpp/xmpp_env.h"
#include "xmpp/xmpp_logger.h"
#include "xmpp/xmpp_context.h"
#include "xmpp/xmpp_connection.h"

#include "detail/operation_queue.h"

namespace messenger
{

class MessengerImpl : public IMessenger
{
public:
	MessengerImpl(const MessengerSettings& settings);
	~MessengerImpl();
	
public:
	// IMessenger
	void Login(const UserId& userId,
               const std::string& password,
               const SecurityPolicy& securityPolicy,
               ILoginCallback* callback) override;
	void Disconnect() throw() override;
	
	void RequestActiveUsers(IRequestUsersCallback *callback) override;
	
	Message SendMessage(const UserId& recepientId, const MessageContent& msgData) override;
	void SendMessageSeen(const UserId& userId, const MessageId& msgId) override;
	
	void RegisterObserver(IMessagesObserver* observer) override;
	void UnregisterObserver(IMessagesObserver* observer) override;

private:
	static void ConnectionHandler(xmpp_conn_t * const conn,
                                  const xmpp_conn_event_t status,
                                  const int error,
                                  xmpp_stream_error_t * const stream_error,
                                  void * const userdata);
	static int MessageHandler(xmpp_conn_t * const conn,
                              xmpp_stanza_t * const stanza,
                              void * const userdata);

    void SendMessageStatus(const UserId& userId, const MessageId& msgId, message_status::Type status);
    
	void NotifyMessageStatus(const MessageId& msgId, message_status::Type status);
	void NotifyNewMessage(const UserId& senderId, const Message& msg);
	
private:
    struct LoginParams
    {
        ILoginCallback* callback;
        SecurityPolicy  securityPolicy;
    };
    typedef std::shared_ptr<LoginParams> LoginParamsPtr;
    
private:
	MessengerSettings                      m_settings;

	std::unordered_set<IMessagesObserver*> m_observers;
	std::mutex                             m_observersMutex;

    xmpp::XmppEnvironmentPtr               m_xmpp;
    xmpp::XmppLoggerPtr                    m_logger;
	xmpp::XmppContextPtr                   m_context;
	xmpp::XmppConnectionPtr                m_connection;
	std::thread                            m_runLoop;
    
    detail::OperationQueue                 m_operationQueue;
    
    LoginParamsPtr                         m_loginParams;
};

}

#endif /* MESSENGER_IMPL_H */
