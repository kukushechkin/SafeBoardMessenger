#ifndef MESSENGER_H
#define MESSENGER_H

#include <memory>

#include "types.h"
#include "callbacks.h"
#include "observers.h"
#include "settings.h"

namespace messenger
{
	
class IMessenger
{
public:
	virtual ~IMessenger() {}
	
	virtual void Login(const UserId& userId,
                       const std::string& password,
                       const SecurityPolicy& securityPolicy,
                       ILoginCallback* callback) = 0;
	virtual void Disconnect() throw() = 0;
	
	virtual void RequestActiveUsers(IRequestUsersCallback *callback) = 0;
	
	virtual Message SendMessage(const UserId& recepientId, const MessageContent& msgData) = 0;
	virtual void SendMessageSeen(const UserId& userId, const MessageId& msgId) = 0;

	virtual void RegisterObserver(IMessagesObserver* observer) = 0;
	virtual void UnregisterObserver(IMessagesObserver* observer) = 0;
};

extern std::shared_ptr<IMessenger> GetMessengerInstance(const MessengerSettings& settings);
	
}

#endif /* MESSENGER_H */
