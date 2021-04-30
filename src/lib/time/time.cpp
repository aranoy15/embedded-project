#include <lib/time/time.hpp>
#include <cassert>

namespace lib::time
{

bool operator==(const Time& t1, const Time& t2)
{
    return t1.as_time() == t2.as_time();
}

bool operator>(const Time& t1, const Time& t2)
{
    return t1.as_time() > t2.as_time();
}

bool operator<(const Time& t1, const Time& t2)
{
    return t1.as_time() < t2.as_time();
}

bool operator>=(const Time& t1, const Time& t2)
{
    return t1 > t2 or t1 == t2;
}

bool operator<=(const Time& t1, const Time& t2)
{
    return t1 < t2 or t1 == t2;
}

Time operator+(const Time& t1, const Time& t2)
{
    return Time(t1.as_time() + t2.as_time());
}

Time operator+(const Time& t1, Time::time_t t2)
{
    return Time(t1.as_time() + t2);
}

Time operator-(const Time& t1, const Time& t2)
{
    return Time(t1.as_time() - t2.as_time());
}

Time operator-(const Time& t1, Time::time_t t2)
{
    return Time(t1.as_time() - t2);
}

Time operator*(const Time& t1, const Time& t2)
{
    return Time(t1.as_time() * t2.as_time());
}

Time operator*(const Time& t1, Time::time_t t2)
{
    return Time(t1.as_time() * t2);
}

Time operator/(const Time& t1, const Time& t2)
{
    assert(t2.as_time() != 0);

    return Time(t1.as_time() / t2.as_time());
}

Time operator/(const Time& t1, Time::time_t t2)
{
    return Time(t1.as_time() + t2);
}
}

