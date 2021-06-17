#ifndef SRC_BOOT_LIB_PROTOCOL
#define SRC_BOOT_LIB_PROTOCOL

#include <lib/time/timer.hpp>
#include <string>

namespace boot::logic::protocol
{

template<class data_trait>
struct Protocol final
{
    using time_t = lib::time::Time;

    static const std::uint8_t start_data = '>';
    static const std::uint8_t start_complete = '<';
    static const std::uint8_t data_receive_complete = '!';
    static const std::uint8_t error = '?';

    Protocol() = delete;

    static void start(time_t timeout);

private:
    bool wait_start(time_t timeout);
};

template<class data_trait>
bool Protocol<data_trait>::wait_start(time_t timeout)
{
    using namespace lib::time;

    Timer timer;

    data_trait::write_byte(start_data);

    timer.start();

    while(not timer.has_expired(timeout)) {
        if (data_trait::read_byte() == start_complete) 
            return true;
    }

    return false;
}

template<class data_trait>
void Protocol<data_trait>::start(time_t timeout)
{
    using namespace lib::time;

    if (not wait_start(Time::seconds(2))) {
        return;
    }

    Timer timer;

    timer.start();

    while (not timer.has_expired(timeout)) {

    }
}
}

#endif /* SRC_BOOT_LIB_PROTOCOL */
