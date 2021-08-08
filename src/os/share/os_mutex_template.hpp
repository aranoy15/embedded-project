//
// Created by Антон Федосеев on 07.08.2021.
//

#ifndef SRC_OS_SHARE_OS_MUTEX_TEMPLATE_HPP
#define SRC_OS_SHARE_OS_MUTEX_TEMPLATE_HPP

#include <os_hal.hpp>
//#include <concepts>
#include <lock.hpp>
#include <cassert>

namespace os::mutex
{
id_t create();
bool lock(id_t id);
void unlock(id_t id);
void remove(id_t id);

struct Implementation
{
    using id_t = os::recursive_mutex::id_t;
    static constexpr id_t empty_id = os::recursive_mutex::empty_id;

    static id_t create() { return os::mutex::create(); }
    static bool lock(id_t id) { return os::mutex::lock(id); }
    static void unlock(id_t id) { os::mutex::unlock(id); }
    static void remove(id_t id) { os::mutex::remove(id); }
};
}

namespace os::recursive_mutex
{
id_t create();
bool lock(id_t id);
void unlock(id_t id);
void remove(id_t id);

struct Implementation
{
    using id_t = os::recursive_mutex::id_t;
    static constexpr id_t empty_id = os::recursive_mutex::empty_id;

    static id_t create() { return os::recursive_mutex::create(); }
    static bool lock(id_t id) { return os::recursive_mutex::lock(id); }
    static void unlock(id_t id) { os::recursive_mutex::unlock(id); }
    static void remove(id_t id) { os::recursive_mutex::remove(id); }
};
}

namespace os
{

//template<typename T>
//concept MutexConcept = requires {
//    typename T::id_t;
//    T::empty_id;
//};


template<typename MutexImpl>
class MutexBase final : public lib::lock::Lockable
{
public:
    using id_t = typename MutexImpl::id_t;

public:
    MutexBase() noexcept : Lockable(), _id()
    {
        if (os::is_started()) create();
    }

    ~MutexBase() override = default;

    void lock() override
    {
        if (not os::is_started()) return;
        if (_id == MutexImpl::empty_id) create();

        MutexImpl::lock(_id);
    }

    void unlock() override
    {
        if (_id == MutexImpl::empty_id) return;
        MutexImpl::unlock(_id);
    }

private:
    id_t _id;

private:
    void create()
    {
        _id = MutexImpl::create();
        assert(_id != MutexImpl::empty_id);
    }
};

using mutex_t = MutexBase<os::mutex::Implementation>;
using recursive_mutex_t = MutexBase<os::recursive_mutex::Implementation>;
}

#endif  // SRC_OS_SHARE_OS_MUTEX_TEMPLATE_HPP
