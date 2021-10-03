//
// Created by afedoseev on 19.08.2021.
//

#ifndef EMBEDDED_PROJECT_QUEUE_HPP
#define EMBEDDED_PROJECT_QUEUE_HPP

namespace lib::data
{

template<typename T, std::size_t length>
class queue
{
public:
    queue() noexcept;
    ~queue() = default;

    [[nodiscard]] bool empty() const;
    [[nodiscard]] bool full() const { return _full; }
    [[nodiscard]] std::size_t capacity() const { return length; }
    [[nodiscard]] std::size_t size() const;

    void push(const T& item);
    T pop();
    bool peek(T& item) const;
    void clear();

private:
    T _data[length];
    std::size_t _head;
    std::size_t _tail;
    bool _full;
};

template <typename T, std::size_t length>
queue<T, length>::queue() noexcept
    : _data(),
      _head(0),
      _tail(0),
      _full(false)
{
}

template <typename T, std::size_t length>
bool queue<T, length>::empty() const
{
    return (not full() and (_head == _tail));
}

template <typename T, std::size_t length>
std::size_t queue<T, length>::size() const
{
    std::size_t size = length;

    if (not full()) {
        if (_head >= _tail) size = _head - _tail;
        else size = capacity() + _head - _tail;
    }

    return size;
}

template <typename T, std::size_t length>
void queue<T, length>::push(const T& item)
{
    _data[_head] = item;

    if (full())
        _tail = (_tail + 1) % capacity();

    _head = (_head + 1) % capacity();
    _full = _head == _tail;
}

template <typename T, std::size_t length>
T queue<T, length>::pop()
{
    if (empty()) return T();

    auto result = _data[_tail];
    _full = false;
    _tail = (_tail + 1) % capacity();

    return result;
}

template <typename T, std::size_t length>
bool queue<T, length>::peek(T& item) const
{
    if (empty()) return false;
    item = _data[_tail];
    return true;
}

template <typename T, std::size_t length>
void queue<T, length>::clear()
{
    _head = _tail;
    _full = false;
}

}
#endif  // EMBEDDED_PROJECT_QUEUE_HPP
