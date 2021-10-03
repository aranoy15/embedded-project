//
// Created by afedoseev on 17.06.2021.
//

#ifndef EMBEDDED_PROJECT_STRING_HPP
#define EMBEDDED_PROJECT_STRING_HPP

#include <cinttypes>
#include <cstring>
#include <cmath>
#include <lib/stream/reader.hpp>
#include <lib/stream/writer.hpp>

namespace lib::data
{
template <std::size_t length>
class string
{
public:
    static const char TERMINATOR = '\0';

public:
    using iterator = char*;
    using const_iterator = const char*;

public:
    string() noexcept;
    explicit string(const char* str) noexcept;
    template <std::size_t copy_length>
    explicit string(const string<copy_length>& other) noexcept;
    ~string() = default;

    [[nodiscard]] std::size_t capacity() const noexcept { return length; }
    [[nodiscard]] std::size_t size() const noexcept { return _size; }
    [[nodiscard]] std::size_t free() const noexcept { return length - _size; }

    [[nodiscard]] bool full() const noexcept { return _size == length; }
    [[nodiscard]] bool empty() const noexcept { return _size == 0; }
    void clear() noexcept { _size = 0; }

    [[nodiscard]] const char* as_str() const noexcept { return _data; }

    [[maybe_unused]] [[nodiscard]] const uint8_t* as_bytes() const noexcept
    {
        return const_cast<const uint8_t*>(_data);
    }

    iterator begin() { return &_data[0]; }
    [[nodiscard]] const_iterator begin() const { return &_data[0]; }
    iterator end() { return &_data[_size]; }
    [[nodiscard]] const_iterator end() const { return &_data[_size]; }

    void append(char ch) noexcept;
    void append(const char* str) noexcept;
    template <std::size_t append_length>
    void append(string<append_length>& str) noexcept;

    bool starts_with(char ch) noexcept;
    bool starts_with(const char* str) noexcept;
    template <std::size_t starts_length>
    bool starts_with(const string<starts_length>& str) noexcept;

    bool contains(char ch) noexcept;
    bool contains(const char* str) noexcept;
    template <std::size_t contains_length>
    bool contains(const string<contains_length>& str) noexcept;

    bool equal(char ch) noexcept;
    bool equal(const char* str) noexcept;
    template<std::size_t equal_length>
    bool equal(const string<equal_length>& str) noexcept;

    void erase(std::size_t start, std::size_t size) noexcept;

    void replace(const char* find, const char* replace);

    int last_index_of(const char* str);
    int last_index_of(const char* str, std::size_t from_index);

    template <std::size_t copy_length>
    string& operator=(const string<copy_length>& other) noexcept;
    string& operator=(const char* str) noexcept;

    char& operator[](int index) noexcept;
    char& operator[](std::size_t index) noexcept;
    const char& operator[](int index) const noexcept;
    const char& operator[](std::size_t index) const noexcept;

private:
    char _data[length + sizeof(TERMINATOR)];
    std::size_t _size;

private:
    void terminate() noexcept;
    void copy(const char* data, std::size_t size) noexcept;
    bool contains(const char* data, std::size_t size) noexcept;
};

template <std::size_t length>
string<length>::string() noexcept : _data(), _size(0)
{
}

template <std::size_t length>
string<length>::string(const char* str) noexcept : _data(), _size(0)
{
    std::size_t len = strlen(str);

    copy(str, len);
}

template <std::size_t length>
template <std::size_t copy_length>
string<length>::string(const string<copy_length>& other) noexcept : _data(),
                                                                    _size(0)
{
    copy(other.as_str(), other.size());
}

template <std::size_t length>
void string<length>::terminate() noexcept
{
    if (full()) return;

    _data[_size] = TERMINATOR;
}

template <std::size_t length>
void string<length>::copy(const char* data, std::size_t size) noexcept
{
    std::size_t copy_size = length < size ? length : size;

    memcpy(_data, data, copy_size);
    _size = copy_size;

    terminate();
}

template <std::size_t length>
void string<length>::append(char ch) noexcept
{
    if (full()) return;

    _data[_size++] = ch;

    terminate();
}

template <std::size_t length>
void string<length>::append(const char* str) noexcept
{
    std::size_t len = strlen(str);

    std::size_t copy_size = free() < len ? free() : len;

    memcpy(&_data[_size], str, copy_size);
    _size += copy_size;

    terminate();
}

template <std::size_t length>
template <std::size_t append_length>
void string<length>::append(string<append_length>& str) noexcept
{
    std::size_t copy_size = free() < str.size() ? free() : str.size();

    memcpy(&_data[_size], str.as_str(), copy_size);
    _size += copy_size;

    terminate();
}

template <std::size_t length>
char& string<length>::operator[](int index) noexcept
{
    if (index < 0 or index >= _size) return _data[0];
    return _data[index];
}

template <std::size_t length>
char& string<length>::operator[](std::size_t index) noexcept
{
    if (index >= _size) return _data[0];
    return _data[index];
}

template <std::size_t length>
const char& string<length>::operator[](int index) const noexcept
{
    if (index < 0 or index >= _size) return _data[0];
    return _data[index];
}

template <std::size_t length>
const char& string<length>::operator[](std::size_t index) const noexcept
{
    if (index >= _size) return _data[0];
    return _data[index];
}

template <std::size_t length>
template <std::size_t copy_length>
auto string<length>::operator=(const string<copy_length>& other) noexcept
    -> string<length>&
{
    copy(other.as_str(), other.size());
    return *this;
}

template <std::size_t length>
auto string<length>::operator=(const char* str) noexcept -> string<length>&
{
    std::size_t len = strlen(str);
    copy(str, len);
    return *this;
}
template <std::size_t length>
bool string<length>::starts_with(char ch) noexcept
{
    if (empty()) return false;

    return _data[0] == ch;
}
template <std::size_t length>
bool string<length>::starts_with(const char* str) noexcept
{
    if (empty()) return false;

    std::size_t len = strlen(str);

    if (size() < len) return false;

    return strncmp(as_str(), str, len) == 0;
}
template <std::size_t length>
template <std::size_t starts_length>
bool string<length>::starts_with(const string<starts_length>& str) noexcept
{
    if (empty() or str.empty()) return false;

    return strncmp(as_str(), str.as_str(), str.size()) == 0;
}
template <std::size_t length>
bool string<length>::contains(char ch) noexcept
{
    return contains(&ch, sizeof(ch));
}
template <std::size_t length>
bool string<length>::contains(const char* data, std::size_t size) noexcept
{
    if (size == 0 or empty()) return false;

    for (std::size_t i = 0; i < this->size(); ++i) {
        if (_data[i] == data[0]) {
            if (this->size() >= (i + size))
                if (strncmp(&_data[i], data, size) == 0)
                    return true;
        }
    }

    return false;
}
template <std::size_t length>
bool string<length>::contains(const char* str) noexcept
{
    std::size_t len = strlen(str);

    if (len == 0 or empty()) return false;

    return contains(str, len);
}
template <std::size_t length>
template <std::size_t contains_length>
bool string<length>::contains(const string<contains_length>& str) noexcept
{
    if (str.empty() or empty()) return false;

    return contains(str.as_str(), str.size());
}
template <std::size_t length>
void string<length>::erase(std::size_t start, std::size_t size) noexcept
{
    if (empty() or start >= this->size()) return;

    size_t end_index = start + size;

    if (end_index >= this->size()) {
        _size = start;
    } else {
        size_t count_move = this->size() - end_index + 1;
        memmove(&_data[start], &_data[end_index], count_move);
        _size -= size;
    }

    terminate();
}

template <std::size_t length>
bool string<length>::equal(char ch) noexcept
{
    if (empty() or size() > 1) return false;

    return _data[0] == ch;
}

template<std::size_t length>
bool string<length>::equal(const char* str) noexcept
{
    std::size_t len = strlen(str);

    if (len == 0 and empty()) return true;

    return strcmp(as_str(), str) == 0;
}

template<std::size_t length>
template<std::size_t equal_length>
bool string<length>::equal(const string<equal_length>& str) noexcept
{
    if (str.empty() and empty()) return true;

    return strcmp(as_str(), str.as_str()) == 0;
}
template <std::size_t length>
void string<length>::replace(const char* find, const char* replace)
{
        if (find == nullptr or replace == nullptr) return;

        std::size_t find_length = strlen(find);
        std::size_t replace_length = strlen(replace);

        int diff = replace_length - find_length;

        char *read_from = _data;
        char *found_at;
        if (diff == 0) {
            while ((found_at = strstr(read_from, find)) != nullptr) {
                std::memcpy(found_at, replace, replace_length);
                read_from = found_at + replace_length;
            }
        } else if (diff < 0) {
            std::size_t needed_size = size(); // compute size needed for result
            diff = 0 - diff;

            while ((found_at = std::strstr(read_from, find)) != nullptr) {
                read_from = found_at + find_length;
                needed_size -= diff;
            }

            if (needed_size == size()) return;
            int index = size() - 1;

            while (index >= 0 && (index = last_index_of(find, index)) >= 0) {
                read_from = _data + index + find_length;
                std::memmove(read_from - diff, read_from, size() - (read_from - _data));

                _size -= diff;
                _data[size()] = 0;

                std::memcpy(_data + index, replace, replace_length);
                index--;
            }
        } else {
            size_t needed_size = size(); // compute size needed for result

            while ((found_at = strstr(read_from, find)) != nullptr) {
                read_from = found_at + find_length;
                needed_size += diff;
            }

            if (needed_size == size()) return;
            if (needed_size > capacity()) return; // XXX: tell user!

            int index = size() - 1;
            while (index >= 0 && (index = last_index_of(find, index)) >= 0) {
                read_from = _data + index + find_length;
                std::memmove(read_from + diff, read_from, size() - (read_from - _data));

                _size += diff;
                _data[_size] = 0;

                std::memcpy(_data + index, replace, replace_length);
                index--;
            }
        }
}
template <std::size_t length>
int string<length>::last_index_of(const char* str, std::size_t from_index)
{
    if (str == nullptr) return -1;
    std::size_t str_length = strlen(str);

    if (str_length == 0 or empty() or str_length > size()) return -1;

    int found = -1;

    for (char* ptr = _data; ptr <= _data + from_index; ++ptr) {
        ptr = std::strstr(ptr, str);
        if (not ptr) break;
        if ((std::size_t)(ptr - _data) <= from_index) found = ptr - _data;
    }

    return found;
}
template <std::size_t length>
int string<length>::last_index_of(const char* str)
{
    std::size_t str_length = strlen(str);
    return last_index_of(str, size() - str_length);
}

template <std::size_t length>
auto operator+(string<length>& lhs, char rhs) noexcept -> string<length>&
{
    lhs.append(rhs);
    return lhs;
}

template <std::size_t length>
auto operator+(char lhs, string<length>& rhs) noexcept -> string<length>&
{
    rhs.append(lhs);
    return rhs;
}

template <std::size_t length>
auto operator+(string<length>& lhs, const char* rhs) noexcept
    -> string<length>&
{
    lhs.append(rhs);
    return lhs;
}

template <std::size_t length>
auto operator+(const char* lhs, string<length>& rhs) noexcept
    -> string<length>&
{
    rhs.append(lhs);
    return rhs;
}

template <std::size_t left_length, std::size_t right_length>
auto operator+(string<left_length>& lhs, string<right_length> rhs) noexcept
    -> string<left_length>&
{
    lhs.append(rhs);
    return lhs;
}

template<std::size_t length>
bool operator==(string<length>& lhs, char rhs) noexcept
{
    return lhs.equal(rhs);
}

template <std::size_t length>
bool operator==(char lhs, string<length>& rhs) noexcept
{
    return rhs.equal(lhs);
}

template <std::size_t length>
bool operator==(string<length>& lhs, const char* rhs) noexcept
{
    return lhs.equal(rhs);
}

template <std::size_t length>
bool operator==(const char* lhs, string<length>& rhs) noexcept
{
    return rhs.equal(lhs);
}

template <std::size_t left_length, std::size_t right_length>
bool operator==(string<left_length>& lhs,
                string<right_length>& rhs) noexcept
{
    return lhs.equal(rhs);
}


}  // namespace lib::data

template<std::size_t length>
inline lib::stream::Reader& operator>>(lib::stream::Reader& stream, lib::data::string<length>& data)
{
    if (stream.available() <= 0 or length == 0)
        return stream;

    size_t read_size = std::min((std::size_t)stream.available(), data.capacity());

    do {
        auto symbol = static_cast<char>(stream.read());
        data.append(symbol);
        read_size--;
    } while (read_size > 0);

    return stream;
}

template<std::size_t length>
inline lib::stream::Writer& operator<<(lib::stream::Writer& stream, lib::data::string<length>& data)
{
    for (char symbol : data)
        stream.write(static_cast<std::uint8_t>(symbol));

    return stream;
}

#endif  // EMBEDDED_PROJECT_STRING_HPP
