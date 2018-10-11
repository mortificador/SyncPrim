#include "base_reset_event.h"

namespace sync_prim
{
    base_reset_event::base_reset_event(bool signaled) :
            signaled_(signaled)
    {}

    void base_reset_event::wait_one()
    {
        std::unique_lock<std::mutex> l(m_);
        if (signaled_ == false)
        {
            // This mutex is used by the condition variable to call the lambda. There is no shared state to protect,
            // so it's ok to use a local mutex.
            // Ok, I lied, there is shared state to protect, the variable signaled_. It could happen that ::set() is called,
            // inside ::set() we set signaled_ to true, then, before notify_all() is called, a spurious wake up occurs,
            // the condition variable sees that signaled_ is true, and unblocks (before notify_all() is called in ::set()).
            // This scenario is, however, unlikely to happen, and the consequences are practically none.
            // On UB:
            // Even though the standard says that calling condition variables to wait on different mutexes on different
            // threads is undefined behaviour, implementations only lead to undefined behaviour if you actually modify
            // shared variables inside the cond var lambda
            std::mutex mutex;
            std::unique_lock<std::mutex> local_lock(mutex);
            // The lambda is called immediately after we call wait, and then every time the condition variable
            // is notified or when there is a spurious wake up
            cond_var_.wait(local_lock, [this, &l]() {
                // We need to unlock the variable to avoid deadlocks (otherwise we will keep the mutex locked
                // and calling any other method that locks the mutex will deadlock)
                if (l.owns_lock())
                {
                    l.unlock();
                }
                return signaled_;
            });
        }
    }


    void base_reset_event::set_all()
    {
        std::lock_guard<std::mutex> l(m_);
        signaled_ = true;
        cond_var_.notify_all();
    }

    void base_reset_event::set_one()
    {
        std::lock_guard<std::mutex> l(m_);
        signaled_ = true;
        cond_var_.notify_one();
    }

    void base_reset_event::reset()
    {
        std::lock_guard<std::mutex> l(m_);
        signaled_ = false;
    }
}
