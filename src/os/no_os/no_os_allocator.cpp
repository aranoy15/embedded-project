//
// Created by Антон Федосеев on 08.08.2021.
//

#include <os.hpp>
#include <cstdlib>

void* os::alloc(std::size_t size)
{
    return std::malloc(size);
}

void os::free(void* ptr)
{
    std::free(ptr);
}

bool os::block_init(std::uint8_t* ptr, std::size_t size)
{
    (void)ptr;
    (void)size;
    return true;
}

