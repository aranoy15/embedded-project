//
// Created by Антон Федосеев on 29.08.2021.
//

#ifndef SRC_BSP_SHARE_BSP_USB_TEMPLATE_HPP
#define SRC_BSP_SHARE_BSP_USB_TEMPLATE_HPP

#include <csp.hpp>
#include <lib/data/vector.hpp>
#include <lib/data/queue.hpp>
#include <os.hpp>

namespace bsp::usb
{
template<std::size_t packet_size, std::size_t packet_count>
class Usb final
{
public:
    using word_t = std::uint8_t;
    using packet_t = lib::data::vector<word_t, packet_size>;
    using queue_t = lib::data::queue<packet_t, packet_count>;
    using mutex_t = os::mutex_t;

public:
    Usb() = delete;

    static bool init()
    {
        _queue.init();

        return true;
    }

private:
    inline static queue_t _queue;
    inline static mutex_t _rx_mutex;
    inline static mutex_t _tx_mutex;
};
}

#endif  // SRC_BSP_SHARE_BSP_USB_TEMPLATE_HPP
