#include "auto_reset_event.h"

namespace sync_prim
{
    auto_reset_event::auto_reset_event(bool signaled) :
            reset_event_(signaled)
    {}

    void auto_reset_event::wait_one()
    {
        reset_event_.wait_one();
    }

    void auto_reset_event::set()
    {
        reset_event_.set_one();
    }

    void auto_reset_event::reset()
    {
        reset_event_.reset();
    }
}
