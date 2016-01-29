#ifndef MESSENGER_OPERATION_QUEUE_H
#define MESSENGER_OPERATION_QUEUE_H

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <queue>

namespace messenger
{
namespace detail
{
        
typedef std::function<void ()> OperationFunc;
    
class OperationQueue
{
public:
    OperationQueue();
    virtual ~OperationQueue();

    void AddOperation(OperationFunc func);
    
private:
    void StartProcessing();
    void StopProcessing();
    
    void ProcessingThread();
    
private:
    OperationQueue(const OperationQueue&) = delete;
    OperationQueue& operator=(const OperationQueue&) = delete;
    
private:
    typedef std::queue<OperationFunc> OperationFuncQueue;
    
    std::thread             m_processingThread;
    std::atomic_bool        m_isRunning;

    OperationFuncQueue      m_operations;
    std::mutex              m_operationsMutex;
    
    std::mutex              m_newOperationMutex;
    std::condition_variable m_newOperationCondition;
};

}
}

#endif /* MESSENGER_OPERATION_QUEUE_H */
