//
// Created by Антон Федосеев on 28.08.2021.
//

#include <os.hpp>

auto os::mutex::create() -> id_t
{
    return static_cast<id_t>(1);
}

bool os::mutex::lock(id_t)
{
    return true;
}

void os::mutex::unlock(id_t)
{
}

void os::mutex::remove(id_t)
{
}

auto os::recursive_mutex::create() -> id_t
{
    return static_cast<id_t>(1);
}

bool os::recursive_mutex::lock(id_t)
{
    return true;
}

void os::recursive_mutex::unlock(id_t)
{
}

void os::recursive_mutex::remove(id_t)
{
}
