//
// Created by Антон Федосеев on 28.08.2021.
//

#include <lib/lock/lock.hpp>

lib::lock::Lock::Lock(Lockable *lockable)
    : _lockable(lockable)
{
    if (_lockable)
        _lockable->lock();
}

lib::lock::Lock::~Lock()
{
    if (_lockable)
        _lockable->unlock();
}
