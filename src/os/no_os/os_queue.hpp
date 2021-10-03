//
// Created by Антон Федосеев on 28.08.2021.
//

#ifndef SRC_OS_NO_OS_OS_QUEUE_HPP
#define SRC_OS_NO_OS_OS_QUEUE_HPP

//#include <os.hpp>
//#include <os_template.hpp>
#include <csp.hpp>
#include <lib/data/queue.hpp>

namespace os::queue
{

template<typename T, std::size_t length>
class Queue
{
public:
    using critical_section_t = csp::critical::Section;

public:
    Queue() noexcept = default;
    ~Queue() = default;

    void init() {}

    [[nodiscard]] bool empty() const { return _queue.empty(); }
    [[nodiscard]] bool full() const { return _queue.full(); }
    [[nodiscard]] std::size_t capacity() const { return _queue.capacity(); }
    [[nodiscard]] std::size_t size() const { return _queue.size(); }

    void push(const T& item)
    {
        critical_section_t section;
        _queue.push(item);
    }

    T pop()
    {
        critical_section_t section;
        return _queue.pop();
    }

    bool peek(T& item) const
    {
        critical_section_t section;
        return _queue.peek(item);
    }

    void clear();

private:
    lib::data::queue<T, length> _queue;
};

template<typename T, std::size_t length>
using queue_t = Queue<T, length>;

}

#endif  // SRC_OS_NO_OS_OS_QUEUE_HPP
