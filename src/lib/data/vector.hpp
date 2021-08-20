//
// Created by afedoseev on 19.08.2021.
//

#ifndef EMBEDDED_PROJECT_VECTOR_HPP
#define EMBEDDED_PROJECT_VECTOR_HPP

namespace lib::data
{

template<typename T, std::size_t length>
class vector
{
    static_assert(length != 0);

public:
    using iterator = T*;
    using const_iterator = const T*;

public:
    vector() noexcept;
    ~vector() = default;

    [[nodiscard]] std::size_t size() const { return _size; }
    [[nodiscard]] std::size_t capacity() const { return length; }
    [[nodiscard]] bool full() const { return _size == length; }
    [[nodiscard]] bool empty() const { return _size == 0; }

    void push_back(const T& data);
    T pop_back();

    // iterator implementation
    iterator begin() { return &_data[0]; }
    const_iterator begin() const { return &_data[0]; }
    iterator end() { return &_data[_size]; }
    const_iterator end() const { return &_data[_size]; }

private:
    T _data[length];
    std::size_t _size;
};

template <typename T, std::size_t length>
vector<T, length>::vector() noexcept : _data(), _size(0)
{
}

template <typename T, std::size_t length>
void vector<T, length>::push_back(const T& data)
{
    if (full()) return;
    _data[_size++] = data;
}

template <typename T, std::size_t length>
T vector<T, length>::pop_back()
{
    if (empty()) return T();
    return _data[_size--];
}

}

#endif  // EMBEDDED_PROJECT_VECTOR_HPP
