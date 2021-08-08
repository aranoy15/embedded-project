//
// Created by Антон Федосеев on 07.08.2021.
//

#ifndef SRC_OS_SHARE_OS_TASK_TEMPLATE_HPP
#define SRC_OS_SHARE_OS_TASK_TEMPLATE_HPP

#include <os_hal.hpp>
//#include <os_event_template.hpp>
#include <os_queue_template.hpp>

namespace os::task
{

enum class Priority
{
    Idle = 0,
    Low,
    Normal,
    High,
    RealTime
};

using tick_t = std::uint32_t;

tick_t tick();
void delay(tick_t ms);
void yield();

class Task
{
public:
    using handler_t = os::task::id_t;
    using stack_t = std::size_t;
    using priority_t = os::task::Priority;
    using argument_t = void*;
    //using event_queue_t = os::queue::Queue<os::event::EventData, 10>;

public:
    Task(priority_t priority, stack_t stack)
        : _id(), _stack(stack), _priority(priority)/*, _event_queue()*/
    {}

    virtual ~Task() = default;

    bool start() noexcept;

    [[nodiscard]] virtual const char *name() const { return "none"; }
    [[nodiscard]] stack_t stack() const { return _stack; }
    [[nodiscard]] priority_t priority() const { return _priority; }

protected:
    virtual void setup() noexcept = 0;
    virtual void loop() noexcept = 0;

private:
    handler_t _id;
    stack_t _stack;
    priority_t _priority;
    //event_queue_t _event_queue;

private:
    static void static_func(argument_t argument);
};
}

#endif  // SRC_OS_SHARE_OS_TASK_TEMPLATE_HPP
