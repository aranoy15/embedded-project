#ifndef EMBEDDED_PROJECT_VEIW_HPP
#define EMBEDDED_PROJECT_VEIW_HPP

namespace lib::data
{
template<typename T>
class view
{
public:
    using type_t = T;
    using type_ptr = T*;

public:
    view(type_ptr data, std::size_t size);
    ~view() = default;

    view(const view& other);
    view& operator=(const view& other);

    type_ptr data() { return _data; }
    const type_ptr data() const { return _data; }
    std::size_t size() const { return _size; }

private:
    type_ptr _data;
    std::size_t _size;
};

template<typename T>
view<T>::view(type_ptr data, std::size_t size) 
    : _data(data), _size(size)
{
}

template<typename T>
view<T>::view(const view& other)
    : _data(other._data), _size(other._size)
{
}

template<typename T>
auto view<T>::operator=(const view& other) -> view&
{
    this->_data = other._data;
    this->_size = other._size;
    return *this;
}
}

#endif // EMBEDDED_PROJECT_VEIW_HPP