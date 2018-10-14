//
// Created by fernando on 21/09/18.
//

#ifndef SYNC_PRIM_MANUAL_RESET_EVENT_H
#define SYNC_PRIM_MANUAL_RESET_EVENT_H

#include "base_reset_event.h"
#include <condition_variable>
#include <mutex>

namespace sync_prim
{

    class manual_reset_event
    {
    public:
		manual_reset_event(bool signaled = false);

        void wait_one();

        template<typename rep, typename period>
        bool wait_one(std::chrono::duration<rep, period> limit)
        {
            return reset_event_.wait_one(limit);
        };

        // In C# Set() returns false only if the kernel call failed
        // That can't happen here.
        void set();
        void reset();

    private:
        base_reset_event reset_event_;
    };
}

#endif //SYNC_PRIM_MANUAL_RESET_EVENT_H
