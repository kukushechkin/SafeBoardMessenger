#ifndef MESSENGER_CALLBACKS_H
#define MESSENGER_CALLBACKS_H

#include "types.h"

namespace messenger
{

class IAsyncOperationCallback
{
public:
    virtual ~IAsyncOperationCallback() {}
};

class ILoginCallback : public IAsyncOperationCallback
{
public:
    virtual void OnOperationResult(operation_result::Type result) = 0;
};

class IRequestUsersCallback : public IAsyncOperationCallback
{
public:
    virtual void OnOperationResult(operation_result::Type result, const UserList& users) = 0;
};

}

#endif /* MESSENGER_CALLBACKS_H */
