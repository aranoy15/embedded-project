//
// Created by afedoseev on 30.06.2021.
//

#include <cinttypes>
#include <os_template.hpp>

void* operator new(std::size_t size) noexcept
{
    return os::alloc(size);
}

void* operator new[](std::size_t size) noexcept
{
    return os::alloc(size);
}

void operator delete(void* block) noexcept
{
    os::free(block);
}

void operator delete[](void* block) noexcept
{
    os::free(block);
}
