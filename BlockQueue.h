#pragma once
#include <cstdio>
#include <cstdint>
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class BlockQueue {
public:
    BlockQueue(size_t capacity=SIZE_MAX);
    void push(const T& e);
    T pop();
    size_t size();
    bool empty();

private:
    const size_t capacity;
    std::queue<T> Q;
    std::mutex mtx;
    std::condition_variable Q_not_empty;
    std::condition_variable Q_not_full;
};

template<typename T>
BlockQueue<T>::BlockQueue(size_t capacity):
    capacity(capacity)
{
}

template<typename T>
void
BlockQueue<T>::push(const T& e)
{
    std::unique_lock<std::mutex> lck(mtx);
    Q_not_full.wait(lck, [this]{ return Q.size() != capacity; });
    Q.push(e);
    Q_not_empty.notify_one();
}

template<typename T>
T
BlockQueue<T>::pop()
{
    std::unique_lock<std::mutex> lck(mtx);
    Q_not_empty.wait(lck, [this]{ return !Q.empty(); });
    T e(Q.front());
    Q.pop();
    Q_not_full.notify_one();
    return e;
}

template<typename T>
size_t
BlockQueue<T>::size()
{
    std::lock_guard<std::mutex> lck(mtx);
    return Q.size();
}

template<typename T>
bool
BlockQueue<T>::empty()
{
    std::lock_guard<std::mutex> lck(mtx);
    return Q.empty();
}
