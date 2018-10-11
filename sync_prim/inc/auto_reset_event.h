//
// Created by fernando on 21/09/18.
//

#ifndef SYNC_PRIM_AUTO_RESET_EVENT_H
#define SYNC_PRIM_AUTO_RESET_EVENT_H

#include "base_reset_event.h"
#include <condition_variable>
#include <mutex>

namespace sync_prim
{
    class auto_reset_event
    {
    public:
        auto_reset_event(bool signaled = false);

        void wait_one();

        template<typename rep, typename period>
        void wait_one(std::chrono::duration<rep, period> limit)
        {
            reset_event_.wait_one(limit);
        };

        void set();
        void reset();

    private:
        base_reset_event reset_event_;
    };
}

#endif //SYNC_PRIM_AUTO_RESET_EVENT_H
