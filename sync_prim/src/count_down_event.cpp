#include "count_down_event.h"

namespace sync_prim
{
    count_down_event::count_down_event(std::size_t initial_count) :
    count_{initial_count}
    {
    }

    void count_down_event::add(std::size_t count)
    {
        count_ += count;
    }

    void count_down_event::add()
    {
        count_++;
    }

    bool count_down_event::try_add()
    {
        return try_add(1);
    }

    bool count_down_event::try_add(std::size_t to_add)
    {
        auto curr_val = count_.load();
        if (curr_val != 0)
        {
            auto wanted = curr_val + to_add;
            while (count_.compare_exchange_weak(curr_val, wanted) == false)
            {
                wanted = curr_val + to_add;
            }

            return true;
        }

        return false;
    }

    void count_down_event::wait()
    {
        std::mutex m;
        std::unique_lock<std::mutex> local_lock(m);

        cond_var_.wait(local_lock, [this] {
           return count_.load() == 0;
        });
    }


    bool count_down_event::signal()
    {
        auto val = count_.load();
        if (val == 0)
        {
            return false;
        }

        auto wanted = val - 1;
        while (count_.compare_exchange_weak(val, wanted) == false)
        {
            // If val is zero here, someone else
            // set it to zero while we were trying
            // to set it to zero. There's nothing else
            // to do
            if (val == 0)
            {
                return false;
            }

            wanted = val - 1;
        }

        // If between this point and the end of the function a
        // spurious wake up happens, the threads will be notified
        // twice (the spurious wake up will be processed as a real one),
        // as the condition for the threads to wake up
        // is to check that count_ is zero

        // If wanted is zero here, we were the ones setting count_
        // to zero, we notify the threads waiting
        if ( wanted == 0)
        {
            cond_var_.notify_all();
            return true;
        }

        return false;
    }
}
