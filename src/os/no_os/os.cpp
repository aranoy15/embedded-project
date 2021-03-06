#include <os.hpp>
#include <list>
#include <csp.hpp>

namespace
{
    bool is_started = false;
    std::list<os::task::Task*> tasks_list;

    void do_loop()
    {
        for (auto item : tasks_list) {
            item->setup();
        }

        while (::is_started) {
            for (auto item : tasks_list) {
                if (item->timeout() > 0) {
                    if (os::task::current_tick() - item->start_tick() >= item->timeout()) {
                        item->loop();
                        item->start_tick() = os::task::current_tick();
                    }
                } else item->loop();
            }
        }
    }
}

bool os::init()
{
    return true;
}

bool os::start()
{
    ::is_started = true;

    using task_t = os::task::Task;

    tasks_list.sort([](const task_t* left, const task_t* right) -> bool {
        return left->priority() > right->priority();
    });

    do_loop();
    return true;
}

bool os::stop()
{
    ::is_started = false;
    return true;
}

bool os::is_started()
{
    return ::is_started;
}

bool os::task::Task::start() noexcept
{
    tasks_list.push_back(this);
    return true;
}

void os::task::Task::static_func(argument_t argument)
{
    (void)argument;
}

void os::critical::enter()
{
    csp::critical::enter();
}

void os::critical::exit()
{
    csp::critical::exit();
}
