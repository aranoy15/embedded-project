#ifndef SRC_LIB_TIME_TIME
#define SRC_LIB_TIME_TIME

#include <bsp.hpp>

namespace lib::time
{
class Time final
{
public:
    using time_t = bsp::os::time::time_t;
    static constexpr time_t milliseconds_in_second = 1000; 
    static constexpr time_t seconds_in_minute = 60;
    static constexpr time_t minutes_in_hour = 60;
    static constexpr time_t hours_in_day = 24;

public:
    Time() : _ticks() {}
    explicit Time(time_t ticks) : _ticks(ticks) {}
    explicit Time(const Time& t) : _ticks(t._ticks) {}

    Time operator=(const Time& t) { return Time(t._ticks); }

    static time_t current() { return bsp::os::time::current(); }

    static Time milliseconds(time_t msecs) { return Time(msecs); }
    static Time seconds(time_t s) { return Time(s * milliseconds_in_second); }
    static Time minutes(time_t m) { return Time(m * seconds(seconds_in_minute).as_time()); }
    static Time hours(time_t h) { return Time(h * minutes(minutes_in_hour).as_time()); }
    static Time days(time_t d) { return Time(d * hours(hours_in_day).as_time()); }

    time_t as_time() const { return _ticks; }

private:
    time_t _ticks;
};

bool operator==(const Time& t1, const Time& t2); 
bool operator>(const Time& t1, const Time& t2); 
bool operator<(const Time& t1, const Time& t2); 
bool operator>=(const Time& t1, const Time& t2); 
bool operator<=(const Time& t1, const Time& t2);
Time operator+(const Time& t1, const Time& t2);
Time operator+(const Time& t1, Time::time_t t2);
Time operator-(const Time& t1, const Time& t2);
Time operator-(const Time& t1, Time::time_t t2);
Time operator*(const Time& t1, const Time& t2);
Time operator*(const Time& t1, Time::time_t t2);
Time operator/(const Time& t1, const Time& t2);
Time operator/(const Time& t1, Time::time_t t2);
}

#endif /* SRC_LIB_TIME_TIME */
