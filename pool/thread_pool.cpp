//
// Created by root on 23-3-11.
//

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>
#include <cassert>

class thread_pool
{
public:
    explicit thread_pool(size_t thread_count = 8) : _pool(std::shared_ptr<pool>())
    {
        assert(thread_count > 0);
        for(size_t i = 0; i < thread_count; i++)
        {
            std::thread(
                [pool = _pool] () {
                    std::unique_lock<std::mutex> locker(pool->mtx);
                    while(true)
                    {
                        if(pool->tasks.empty())
                        {
                            auto task = std::move(pool->tasks.front());
                            pool->tasks.pop();
                            locker.unlock();
                            task();
                            locker.lock();
                        }
                        else if(pool->is_closed)
                            break;
                        else
                            pool->cond.wait(locker);
                    }
                }
            ).detach();
        }
    }

    thread_pool() = default;

    thread_pool(thread_pool&&) = default;

    ~thread_pool()
    {
        if(static_cast<bool>(_pool))
        {
            std::lock_guard<std::mutex> locker(_pool->mtx);
            _pool->is_closed = true;
        }
        _pool->cond.notify_all();
    }

private:
    struct pool
    {
        std::mutex mtx;
        std::condition_variable cond;
        bool is_closed;
        std::queue<std::function<void()>> tasks;
    };

    std::shared_ptr<pool> _pool;
};