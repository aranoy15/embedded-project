//
// Created by Антон Федосеев on 28.08.2021.
//

#ifndef SRC_LIB_LOCK_HPP
#define SRC_LIB_LOCK_HPP

namespace lib::lock
{
class Lockable
{
public:
    Lockable() = default;
    virtual ~Lockable() = default;

    virtual void lock() = 0;
    virtual void unlock() = 0;
};

class Lock final
{
public:
    Lock(Lockable* lockable);
    ~Lock();

private:
    Lockable* _lockable;
};
}

#endif  // SRC_LIB_LOCK_HPP
