//
// Created by afedoseev on 17.06.2021.
//

#ifndef EMBEDDED_PROJECT_STRING_HPP
#define EMBEDDED_PROJECT_STRING_HPP

#include <cinttypes>
#include <cstring>

namespace lib
{
namespace data
{

    template <std::size_t length>
    class string
    {
    public:
        static const char TERMINATOR = '\0';

    public:
        string() noexcept;
        explicit string(const char* str) noexcept;
        template <std::size_t copy_length>
        explicit string(const string<copy_length>& other) noexcept;
        ~string() = default;

        std::size_t capacity() const noexcept { return length; }
        std::size_t size() const noexcept { return _size; }
        std::size_t free() const noexcept { return length - _size; }

        bool is_full() const noexcept { return _size == length; }
        bool is_empty() const noexcept { return _size == 0; }
        void clear() noexcept { _size = 0; }

        const char* as_str() const noexcept { return _data; }

        [[maybe_unused]] const uint8_t* as_bytes() const noexcept
        {
            return const_cast<const uint8_t*>(_data);
        }

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

        template <std::size_t copy_length>
        string& operator=(const string<copy_length>& other) noexcept;
        string& operator=(const char* str) noexcept;
        char operator[](int index) const noexcept;
        char operator[](std::size_t index) const noexcept;
        char& operator[](int index) noexcept;
        char& operator[](std::size_t index) noexcept;

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
        if (is_full()) return;

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
        if (is_full()) return;

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
    char string<length>::operator[](int index) const noexcept
    {
        if (index < 0 or index >= _size) return _data[0];
        return _data[index];
    }

    template <std::size_t length>
    char string<length>::operator[](std::size_t index) const noexcept
    {
        if (index >= _size) return _data[0];
        return _data[index];
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
        if (is_empty()) return false;

        return _data[0] == ch;
    }
    template <std::size_t length>
    bool string<length>::starts_with(const char* str) noexcept
    {
        if (is_empty()) return false;

        std::size_t len = strlen(str);

        if (size() < len) return false;

        return strncmp(as_str(), str, len) == 0;
    }
    template <std::size_t length>
    template <std::size_t starts_length>
    bool string<length>::starts_with(const string<starts_length>& str) noexcept
    {
        if (is_empty() or str.is_empty()) return false;

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
        if (size == 0 or is_empty()) return false;

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

        if (len == 0 or is_empty()) return false;

        return contains(str, len);
    }
    template <std::size_t length>
    template <std::size_t contains_length>
    bool string<length>::contains(const string<contains_length>& str) noexcept
    {
        if (str.is_empty() or is_empty()) return false;

        return contains(str.as_str(), str.size());
    }
    template <std::size_t length>
    void string<length>::erase(std::size_t start, std::size_t size) noexcept
    {
        if (is_empty() or start >= this->size()) return;

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
        if (is_empty() or size() > 1) return false;

        return _data[0] == ch;
    }

    template<std::size_t length>
    bool string<length>::equal(const char* str) noexcept
    {
        std::size_t len = strlen(str);

        if (len == 0 and is_empty()) return true;

        return strcmp(as_str(), str) == 0;
    }

    template<std::size_t length>
    template<std::size_t equal_length>
    bool string<length>::equal(const string<equal_length>& str) noexcept
    {
        if (str.is_empty() and is_empty()) return true;

        return strcmp(as_str(), str.as_str()) == 0;
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

}  // namespace data
}  // namespace lib

#endif  // EMBEDDED_PROJECT_STRING_HPP
