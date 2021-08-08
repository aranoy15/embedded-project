//
// Created by Антон Федосеев on 08.08.2021.
//

#include <no_os_allocator.hpp>
#include <os.hpp>
#include <cstring>

namespace
{
os::Allocator& allocator()
{
    static os::Allocator statement;
    return statement;
}
}  // namespace

void* os::alloc(std::size_t size)
{
    return allocator().alloc(size);
}

void os::free(void* ptr)
{
    allocator().free(ptr);
}

bool os::block_init(std::uint8_t* ptr, std::size_t size)
{
    allocator().init(ptr, size);
    return true;
}

os::Allocator::Allocator() noexcept : _data(nullptr),
                                      _start(nullptr),
                                      _used(nullptr),
                                      _end(nullptr)
{
}

void os::Allocator::init(byte_t* block, std::size_t size) noexcept
{
    assert((size % sizeof(std::size_t)) != 0);

    _data = block;
    _start = _data;
    _used = _start;
    _end = _start + size;
}

void* os::Allocator::alloc(std::size_t size) noexcept
{
    if (_used == _end) return nullptr;

    // align size
    while ((size % sizeof(word_t)) != 0)
        size++;

    void* result = _used;
    _used += size;

    return result;
}

void os::Allocator::free(void* ptr) noexcept
{
    assert(ptr >= _start and ptr < _end);

    auto data = reinterpret_cast<byte_t*>(ptr);
    _used = data;
}
