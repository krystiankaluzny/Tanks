#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <queue>

template<typename T>
class MyQueue
{
public:
    MyQueue(){}
    void push(T obj)
    {
        internal_queue.push(obj);
    }

    bool pop(T* obj)
    {
        if(internal_queue.empty()) return false;

        (*obj) = internal_queue.front();
        internal_queue.pop();

        return true;
    }
private:
    std::queue<T> internal_queue;
};

#endif // MYQUEUE_H
