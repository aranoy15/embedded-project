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
    class string final
    {
    public:
        static const char TERMINATE = '\0';

    public:
        string() noexcept;
        string(const char* str) noexcept;
        template <std::size_t copy_length>
        explicit string(const string<copy_length>& other) noexcept;
        ~string() = default;

        std::size_t capacity() const noexcept { return length; }
        std::size_t size() const noexcept { return _size; }
        std::size_t free() const noexcept { return length - _size; }

        bool is_full() const noexcept { return _size == length; }
        [[maybe_unused]] bool is_empty() const noexcept { return _size == 0; }
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

        template <std::size_t copy_length>
        string& operator=(const string<copy_length>& other) noexcept;
        string& operator=(const char* str) noexcept;
        char& operator[](int index) noexcept;

    private:
        char _data[length];
        std::size_t _size;

    private:
        void terminate() noexcept;
        void copy(const char* data, std::size_t size) noexcept;
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

        _data[_size + 1] = TERMINATE;
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
    char& string<length>::operator[](int index) noexcept
    {
        if (index < 0 or index > _size) return _data[0];

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
    auto operator+(string<left_length>& lhs, string<right_length> rhs)
        -> string<left_length>&
    {
        lhs.append(rhs);
        return lhs;
    }

}  // namespace data
}  // namespace lib

#endif  // EMBEDDED_PROJECT_STRING_HPP
