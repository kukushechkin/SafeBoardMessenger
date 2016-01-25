#ifndef MESSENGER_OBSERVERS_H
#define MESSENGER_OBSERVERS_H

#include "types.h"

namespace messenger
{

class IMessagesObserver
{
public:
    virtual ~IMessagesObserver() {}
    
    virtual void OnMessageStatusChanged(const MessageId& msgId, message_status::Type status) = 0;
    virtual void OnMessageReceived(const UserId& senderId, const Message& msg) = 0;
};

}

#endif /* MESSENGER_OBSERVERS_H */
