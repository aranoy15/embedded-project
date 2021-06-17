#include <lib/time/timer.hpp>
//#include <cassert>

namespace lib::time
{
void Timer::start()
{
    _start_time = Time::current();
    _is_started = true;
}

void Timer::restart()
{
    start();
}

void Timer::stop()
{
    _is_started = false;
}

bool Timer::has_expired(const Time& timeout)
{
    //assert(is_started());

    if (not is_started()) return false;

    if ((Time::current() - _start_time) >= timeout.as_time())
        return true;
    
    return false;
}

Time Timer::elapsed()
{
    return Time(Time::current() - _start_time);
}

}
