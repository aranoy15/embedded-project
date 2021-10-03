//
// Created by Антон Федосеев on 07.08.2021.
//

#ifndef SRC_OS_SHARE_OS_QUEUE_TEMPLATE_HPP
#define SRC_OS_SHARE_OS_QUEUE_TEMPLATE_HPP

#include <os_hal.hpp>
#include <cassert>

namespace os::queue
{
id_t create(std::size_t count, std::size_t size);
bool put(id_t id, const void* data, std::size_t size);
bool get(id_t id, void* data, std::size_t size);
std::size_t size(id_t id);
void remove(id_t id);
void clear(id_t id);

template<typename T, std::size_t length>
class Queue
{
public:
    Queue() noexcept : _id() {}
    virtual ~Queue() = default;

    void init()
    {
        _id = os::queue::create(length, sizeof(T));
        assert(_id != os::queue::empty_id);
    }

    void deinit()
    {
        os::queue::remove(_id);
    }

    bool push(const T& data)
    {
        return os::queue::put(_id, reinterpret_cast<const void*>(&data), sizeof(T));
    }

    [[nodiscard]] std::size_t size() const { return os::queue::size(_id); }
    [[nodiscard]] bool full() const { return size() == length; }
    [[nodiscard]] bool empty() const { return size() == 0; }

    T pop()
    {
        T result;
        os::queue::get(_id, reinterpret_cast<void*>(&result), sizeof(T));

        return result;
    }

    void clear()
    {
        os::queue::clear(_id);
    }

private:
    id_t _id;
};

template<typename T, std::size_t length>
using queue_t = Queue<T, length>;
}

#endif  // SRC_OS_SHARE_OS_QUEUE_TEMPLATE_HPP
