#ifndef SRC_LIB_BUFFERS_LINER_BUFFER
#define SRC_LIB_BUFFERS_LINER_BUFFER

#include <memory>
#include <array>

namespace lib::buffers
{
template<class T, std::size_t max_size>
class liner_buffer
{
public:
    using data_t = std::array<T, max_size>;

public:
	explicit liner_buffer()
	    : _buffer()
	{
	}

    const T* data() const noexcept { return _buffer.data(); }

    bool empty() const noexcept
    {
        return _size == 0;
    }

    bool full() const noexcept
    {
        return _size == max_size;
    }

    size_t capacity() const noexcept
    {
        return max_size;
    }

    size_t size() const noexcept { return _size; }

    void put(T item) noexcept
    {
        if (full()) return;

        _buffer[_size] = item;
        _size++;
    }

    T pop() noexcept
    {
        if (empty()) return T();

        T result = _buffer[_size];
        _size--;

        return result;
    }

    void reset() noexcept
    {
        _size = 0;
    }

private:
    data_t _buffer;
    std::size_t _size = 0;
};
}

#endif /* SRC_LIB_BUFFERS_LINER_BUFFER */
