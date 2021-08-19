//
// Created by afedoseev on 19.08.2021.
//

#ifndef EMBEDDED_PROJECT_STREAM_WRITTER_HPP
#define EMBEDDED_PROJECT_STREAM_WRITTER_HPP

#include <cinttypes>

namespace lib::stream
{

namespace actions
{
struct base_writer;
}

class Writer
{
public:
    enum class IntegerBase : std::uint8_t
    {
        Dec = 10,
        Hex = 16,
        Oct = 8,
        Bin = 2
    };
public:
     Writer() = default;
     virtual ~Writer() = default;

     virtual void write(std::uint8_t data) = 0;
     virtual void flush() = 0;

     Writer& operator<<(std::uint8_t data);
     Writer& operator<<(std::int8_t data);
     Writer& operator<<(std::uint16_t data);
     Writer& operator<<(std::int16_t data);
     Writer& operator<<(std::uint32_t data);
     Writer& operator<<(std::int32_t data);
     Writer& operator<<(std::uint64_t data);
     Writer& operator<<(std::int64_t data);

     Writer& operator<<(const char* data);

     Writer& operator<<(float data);

     Writer& operator<<(actions::base_writer& action);
     Writer& operator<<(actions::base_writer&& action);

     void set_base(IntegerBase base) { _base = base; }

private:
    IntegerBase _base = IntegerBase::Dec;
    std::uint8_t _digits = 2;

private:
    void print_number(std::int64_t number);
    void print_number(std::uint64_t number);
    void print_float(float number);
};
}

#endif  // EMBEDDED_PROJECT_STREAM_WRITTER_HPP
