#pragma once

#include <memory>
#include <array>

namespace lib::buffers
{
template<class T, std::size_t max_size>
class circular_buffer
{
public:
	explicit circular_buffer()
	    : _buffer()
	{
	}

    bool empty() const
    {
        return (not _full and (_head == _tail));
    }

    bool full() const
    {
        return _full;
    }

    size_t capacity() const
    {
        return max_size;
    }

    size_t size() const
    {
        size_t size = max_size;

        if (not full()) {
            if (_head >= _tail) size = _head - _tail;
            else size = max_size + _head - _tail;
        }

        return size;
    }

    void put(T item)
    {
        _buffer[_head] = item;

        if (_full) {
            _tail = (_tail + 1) % max_size;
        }

        _head = (_head + 1) % max_size;

        _full = _head == _tail;
    }

    T get() 
    {
        if (empty()) return T();

        auto result = _buffer[_tail];
        _full = false;
        _tail = (_tail + 1) % max_size;

        return result;
    }

    void reset()
    {
        _head = _tail;
        _full = false;
    }

private:
    std::array<T, max_size> _buffer;
    std::size_t _head = 0;
    std::size_t _tail = 0;
    bool _full = false;
};
}
