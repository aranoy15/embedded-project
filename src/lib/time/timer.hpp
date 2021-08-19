#ifndef SRC_LIB_TIME_TIMER
#define SRC_LIB_TIME_TIMER

#include <lib/time/time.hpp>

namespace lib::time
{
class Timer
{
public:
    Timer() : _start_time(), _is_started(false) {}

    [[nodiscard]] bool is_started() const { return _is_started; }

    void start();
    void restart();
    void stop();
    bool has_expired(const Time& timeout);
    Time elapsed();

private:
    Time::time_t _start_time;
    bool _is_started;
};
}

#endif /* SRC_LIB_TIME_TIMER */
