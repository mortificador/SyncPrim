//
// Created by fernando on 21/09/18.
//

#ifndef SYNC_PRIM_BASE_RESET_EVENT_H
#define SYNC_PRIM_BASE_RESET_EVENT_H

#include <condition_variable>
#include <mutex>

namespace sync_prim
{
    class base_reset_event
    {
    public:
        base_reset_event(bool signaled);

        void wait_one();

        template<typename rep, typename period>
        void wait_one(std::chrono::duration<rep, period> limit)
        {
            std::unique_lock<std::mutex> l(m_);
            if (signaled_ == false)
            {
                std::mutex mutex;
                std::unique_lock<std::mutex> local_lock(mutex);
                cond_var_.wait(local_lock, [this, &l]() {
                    if (l.owns_lock())
                    {
                        l.unlock();
                    }
                    return signaled_;
                });
            }
        };

        // In C# Set() returns false only if the kernel call failed
        // That can't happen here.
        void set_all();
        void set_one();

        void reset();

    private:
        std::condition_variable cond_var_;
        bool signaled_;
        std::mutex m_;
    };
}

#endif //SYNC_PRIM_BASE_RESET_EVENT_H
