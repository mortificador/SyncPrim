#include "manual_reset_event.h"

namespace sync_prim
{
	manual_reset_event::manual_reset_event(bool signaled) : 
		reset_event_(signaled)
	{}

    void manual_reset_event::wait_one()
    {
        reset_event_.wait_one();
    }


    void manual_reset_event::set()
    {
        reset_event_.set_all();
    }

    void manual_reset_event::reset()
    {
        reset_event_.reset();
    }
}
