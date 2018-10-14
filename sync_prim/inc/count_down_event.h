
#ifndef SYNC_PRIM_COUNT_DOWN_EVENT_H
#define SYNC_PRIM_COUNT_DOWN_EVENT_H

#include <condition_variable>
#include <atomic>

namespace sync_prim
{
    class count_down_event
    {
    public:
        count_down_event(std::size_t);

        void add();
        void add(std::size_t count);

        bool try_add();
        bool try_add(std::size_t count);

        void wait();

        template<typename rep, typename period>
        bool wait(std::chrono::duration<rep, period> limit)
        {
            std::mutex m;
            std::unique_lock<std::mutex> local_lock(m);

            return cond_var_.wait_for(local_lock, limit, [this] {
                return count_.load() == 0;
            });
        }

        // Subtracts one to the countdown and notify
        // the threads waiting if the count reaches zero
        bool signal();

    private:
        std::condition_variable cond_var_;
        std::atomic<std::size_t> count_;
    };
}

#endif //SYNC_PRIM_BASE_RESET_EVENT_H
