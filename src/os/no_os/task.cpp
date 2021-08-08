//
// Created by Антон Федосеев on 08.08.2021.
//

#include <os.hpp>
#include <csp.hpp>

auto os::task::tick() -> tick_t
{
    return csp::tick::current();
}

void os::task::delay(tick_t ms)
{
    csp::tick::delay(ms);
}

void os::task::yield()
{
}
