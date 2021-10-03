//
// Created by afedoseev on 25.06.2021.
//

#ifndef EMBEDDED_PROJECT_OS_HAL_HPP
#define EMBEDDED_PROJECT_OS_HAL_HPP

#include <cinttypes>
#include <os_queue.hpp>

namespace os
{
constexpr bool have_heap = false;

namespace task
{
using id_t = int32_t;
}

namespace mutex
{
using id_t = int32_t;
constexpr id_t empty_id = -1;
}

namespace recursive_mutex
{
using id_t = int32_t;
constexpr id_t empty_id = -1;
}

namespace queue
{
using id_t = int32_t;
constexpr id_t empty_id = -1;
}
}

#endif //EMBEDDED_PROJECT_OS_HAL_HPP
