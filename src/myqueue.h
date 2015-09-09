#ifndef MYQUEUE_H
#define MYQUEUE_H
#include <queue>

/**
 * Kolejka zdarzeń
 */
template < typename T >
class MyQueue
{
public:
    MyQueue(){}
    /**
     * Dodanie do kolejki
     * @param obj
     */
    void push(T obj)
    {
        m_queue.push(obj);
    }
    /**
     * Zdjęcie z kolejki
     * @return
     */
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
    /**
     * Kolejka standardowa
     */
    std::queue<T> m_queue;
};

#endif // MYQUEUE_H
