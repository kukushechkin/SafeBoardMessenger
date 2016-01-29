#include "operation_queue.h"

namespace messenger
{
namespace detail
{

OperationQueue::OperationQueue() :
m_isRunning(false)
{
}

OperationQueue::~OperationQueue()
{
    StopProcessing();
}

void OperationQueue::AddOperation(OperationFunc func)
{
    StartProcessing();
    
    m_operationsMutex.lock();
    m_operations.push(func);
    m_operationsMutex.unlock();
    
    std::unique_lock<std::mutex> lock(m_newOperationMutex);
    m_newOperationCondition.notify_all();
}

void OperationQueue::StartProcessing()
{
    if (!m_isRunning)
    {
        m_isRunning = true;

        std::thread thread(std::bind(&OperationQueue::ProcessingThread, this));
        m_processingThread.swap(thread);
    }
}

void OperationQueue::StopProcessing()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        m_processingThread.join();
    }
}

void OperationQueue::ProcessingThread()
{
    while (m_isRunning)
    {
        OperationFunc func;
        m_operationsMutex.lock();
        if (m_operations.size())
        {
            func = m_operations.front();
            m_operations.pop();
        }
        m_operationsMutex.unlock();
        
        if (!!func)
        {
            func();
        }
        else
        {
            std::unique_lock<std::mutex> lock(m_newOperationMutex);
            m_newOperationCondition.wait(lock);
        }
    }
}

}
}
