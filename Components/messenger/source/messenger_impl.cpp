#include "messenger_impl.h"
#include "xmpp/xmpp_stanza_builder.h"
#include "xmpp/xmpp_stanza_parser.h"
#include "detail/uuid.h"

#include <cassert>

namespace messenger
{
	
MessengerImpl::MessengerImpl(const MessengerSettings& settings)
: m_settings(settings)
{
    m_xmpp = std::make_shared<xmpp::XmppEnvironment>();
    
#ifdef _DEBUG
    m_logger = xmpp::XmppLogger::DefaultLogger(XMPP_LEVEL_DEBUG);
#endif // _DEBUG

    m_context = xmpp::XmppContext::NewXmppContext(m_xmpp, m_logger);
    m_connection = xmpp::XmppConnection::NewXmppConnection(m_context);
}

MessengerImpl::~MessengerImpl()
{
}

void MessengerImpl::Login(const UserId& userId,
                          const std::string& password,
                          const SecurityPolicy& securityPolicy,
                          ILoginCallback* callback)
{
	if (!callback)
	{
		throw std::invalid_argument("callback must not be null");
	}
	Disconnect();

    m_loginParams.reset(new LoginParams {
        callback,
        securityPolicy
    });
    
    m_connection->SetJid(userId);
    m_connection->SetPassword(password);
    if (!m_connection->Connect(m_settings.serverUrl,
                               m_settings.serverPort,
                               &ConnectionHandler,
                               this))
    {
        throw std::runtime_error("Failed to start connection");
    }
    
    m_connection->AddHandler(&MessengerImpl::MessageHandler,
                             NULL,
                             xmpp::stanza_names::Message.c_str(),
                             NULL,
                             this);
    
	std::thread runLoop(std::thread([&]
	{
        m_xmpp->Run(m_context);
    }));
	m_runLoop.swap(runLoop);
}

void MessengerImpl::Disconnect() throw()
{
    try {
        if (m_connection->IsConnected())
        {
            m_connection->DeleteHandler(&MessengerImpl::MessageHandler);
            m_connection->Disconnect();
            m_xmpp->Stop(m_context);

            m_runLoop.join();
        }
    }
    catch(...)
    {
    }
}

void MessengerImpl::RequestActiveUsers(IRequestUsersCallback *callback)
{
	if (!callback)
	{
		throw std::invalid_argument("callback must not be null");
	}
	
    xmpp::XmppStanzaPtr iq = xmpp::XmppStanzaBuilder(m_context).CreateRequestActiveUsersStanza();
    
    m_connection->AddIdHandler(&MessageHandler,
                               xmpp::stanza_ids::GetActiveUsers.c_str(),
                               callback);
    m_connection->Send(iq);
}

Message MessengerImpl::SendMessage(const UserId& recepientId, const MessageContent& msgData)
{
    Message msg;
    msg.content = msgData;
    msg.time = std::time(NULL);
    msg.identifier = detail::UUID::RandomUUID().ToString();
    
    xmpp::XmppStanzaPtr message = xmpp::XmppStanzaBuilder(m_context).CreateMessageStanza(recepientId, msg);
    m_connection->Send(message);
    
    NotifyMessageStatus(msg.identifier, message_status::Sending);
    
    return msg;
}

void MessengerImpl::SendMessageSeen(const UserId& userId, const MessageId& msgId)
{
    SendMessageStatus(userId, msgId, message_status::Seen);
}
    
void MessengerImpl::SendMessageStatus(const UserId& userId, const MessageId& msgId, message_status::Type status)
{
    xmpp::XmppStanzaPtr messageStatus = xmpp::XmppStanzaBuilder(m_context).CreateMessageStatusStanza(userId, msgId, status);
    m_connection->Send(messageStatus);
}
    
void MessengerImpl::RegisterObserver(IMessagesObserver* observer)
{
	std::unique_lock<std::mutex> lock(m_observersMutex);
	m_observers.insert(observer);
}

void MessengerImpl::UnregisterObserver(IMessagesObserver* observer)
{
	std::unique_lock<std::mutex> lock(m_observersMutex);
	m_observers.erase(observer);
}

void MessengerImpl::NotifyMessageStatus(const MessageId& msgId, message_status::Type status)
{
	std::unique_lock<std::mutex> lock(m_observersMutex);
	std::for_each(m_observers.begin(), m_observers.end(), [&](IMessagesObserver* observer)
	{
		observer->OnMessageStatusChanged(msgId, status);
	});
}

void MessengerImpl::NotifyNewMessage(const UserId& senderId, const Message& msg)
{
	std::unique_lock<std::mutex> lock(m_observersMutex);
	std::for_each(m_observers.begin(), m_observers.end(), [&](IMessagesObserver* observer)
	{
		observer->OnMessageReceived(senderId, msg);
	});
}
    
void MessengerImpl::ConnectionHandler(xmpp_conn_t * const conn,
                                      const xmpp_conn_event_t status,
                                      const int error,
                                      xmpp_stream_error_t * const stream_error,
                                      void * const userdata)
{
	MessengerImpl* self = reinterpret_cast<MessengerImpl*>(userdata);
    xmpp::XmppConnectionPtr connection = std::make_shared<xmpp::XmppConnection>(conn);
    xmpp::XmppContextPtr context = connection->GetContext();
    
    if (!!self->m_loginParams)
    {
        LoginParams& loginParams = *self->m_loginParams;
        switch (status)
        {
        case XMPP_CONN_CONNECT:
            {
                if (loginParams.securityPolicy.encryptionAlgo != encryption_algorithm::None)
                {
                    xmpp::XmppStanzaPtr iq = xmpp::XmppStanzaBuilder(context).CreateSetPublicKeyStanza(loginParams.securityPolicy.encryptionPubKey);
                    connection->Send(iq);
                }
                
                xmpp::XmppStanzaPtr presence = xmpp::XmppStanzaBuilder(context).CreatePresenceStanza();
                connection->Send(presence);

                loginParams.callback->OnOperationResult(operation_result::Ok);
            }
            break;
        case XMPP_CONN_FAIL:
            {
                loginParams.callback->OnOperationResult(operation_result::NetworkError);
            }
            break;
        case XMPP_CONN_DISCONNECT:
            {
                loginParams.callback->OnOperationResult(operation_result::NetworkError);
            }
            break;
        default:
            {
                loginParams.callback->OnOperationResult(operation_result::InternalError);
            }
            break;
        }
        
        self->m_loginParams.reset();
    }
}

int MessengerImpl::MessageHandler(xmpp_conn_t * const conn,
                                  xmpp_stanza_t * const stz,
                                  void * const userdata)
{
    xmpp::XmppConnectionPtr connection = std::make_shared<xmpp::XmppConnection>(conn);
    xmpp::XmppStanzaPtr stanza = std::make_shared<xmpp::XmppStanza>(stz);
    
    xmpp::XmppStanzaParser parser(connection->GetContext());
    
    if (parser.IsMessageStanza(stanza))
    {
        Message msg;
        UserId senderId;
        if (parser.ParseMessageStanza(stanza, msg, senderId))
        {
            MessengerImpl* self = reinterpret_cast<MessengerImpl*>(userdata);
            
            self->SendMessageStatus(senderId, msg.identifier, message_status::Delivered);
            self->NotifyNewMessage(senderId, msg);
        }
    }
    else if (parser.IsMessageStatusStanza(stanza))
    {
        MessageId msgId;
        message_status::Type status;
        if (parser.ParseMessageStatusStanza(stanza, msgId, status))
        {
            MessengerImpl* self = reinterpret_cast<MessengerImpl*>(userdata);
            self->NotifyMessageStatus(msgId, status);
        }
    }
    else if (parser.IsMessageErrorStanza(stanza))
    {
        MessageId msgId;
        if (parser.ParseMessageErrorStanza(stanza, msgId))
        {
            MessengerImpl* self = reinterpret_cast<MessengerImpl*>(userdata);
            self->NotifyMessageStatus(msgId, message_status::FailedToSend);
        }
    }
    else if (parser.IsActiveUsersResponseStanza(stanza))
    {
        UserList userList;
        if (parser.ParseActiveUsersResponseStanza(stanza, userList))
        {
            IRequestUsersCallback* cb = reinterpret_cast<IRequestUsersCallback*>(userdata);
            cb->OnOperationResult(operation_result::Ok, userList);
        }

        connection->DeleteIdHandler(&MessageHandler, xmpp::stanza_ids::GetActiveUsers.c_str());
    }
 
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<IMessenger> GetMessengerInstance(const MessengerSettings& settings)
{
	return std::make_shared<MessengerImpl>(settings);
}
	
}
