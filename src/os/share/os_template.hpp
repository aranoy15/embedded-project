//
// Created by afedoseev on 24.06.2021.
//

#ifndef EMBEDDED_PROJECT_OS_TEMPLATE_HPP
#define EMBEDDED_PROJECT_OS_TEMPLATE_HPP

#include <cinttypes>
#include <os_task_template.hpp>
#include <type_traits>

namespace os
{
bool init();
bool start();
bool stop();

void* alloc(std::size_t size);
void free(void* ptr);

bool is_started();

bool block_init(std::uint8_t* ptr, std::size_t size);

template<std::size_t heap_size, std::size_t task_count>
class Scheduler final
{
    static_assert((heap_size % sizeof(std::size_t)) == 0);

public:
    Scheduler() = delete;

    static bool init()
    {
        if (not os::block_init(_heap, heap_size)) return false;
        if (not os::init()) return false;

        return true;
    }

    template<typename T, typename... Args>
    static void reg(Args&&... args) noexcept
    {
        static_assert(std::is_base_of<os::task::Task, T>::value);

        if (_tasks_count >= task_count) return;

        _tasks[_tasks_count++] = new T(args...);
    }

    static bool start()
    {
        for (std::size_t i = 0; i < _tasks_count; ++i) {
            _tasks[i]->start();
        }

        return os::start();
    }

    static bool stop()
    {
        return os::stop();
    }

private:
    inline static std::uint8_t _heap[heap_size];
    inline static os::task::Task* _tasks[task_count];
    inline static std::size_t _tasks_count = 0;
};
}

#endif //EMBEDDED_PROJECT_OS_TEMPLATE_HPP
