#ifndef MYQUEUE_H
#define MYQUEUE_H
#include <queue>

template < typename T >
class MyQueue
{
public:
    MyQueue(){}
    void push(T obj)
    {
        m_queue.push(obj);
    }
    T& pop()
    {
        T& obj = m_queue.front();
        m_queue.pop();
        return obj;
    }
    bool empty()
    {
        return m_queue.empty();
    }

private:
    std::queue<T> m_queue;
};

#endif // MYQUEUE_H
