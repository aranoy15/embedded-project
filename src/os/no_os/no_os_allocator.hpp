//
// Created by Антон Федосеев on 08.08.2021.
//

#ifndef SRC_OS_NO_OS_NO_OS_ALLOCATOR_HPP
#define SRC_OS_NO_OS_NO_OS_ALLOCATOR_HPP

#include <cinttypes>

namespace os
{
class Allocator
{
public:
    using word_t = std::uint32_t;
    using byte_t = std::uint8_t;

public:
    Allocator() noexcept;

    void init(byte_t* block, std::size_t size) noexcept;

    void* alloc(std::size_t size) noexcept;
    void free(void* ptr) noexcept;

private:
    byte_t* _data;
    byte_t* _start;
    byte_t* _used;
    byte_t* _end;
};
}

#endif  // SRC_OS_NO_OS_NO_OS_ALLOCATOR_HPP
