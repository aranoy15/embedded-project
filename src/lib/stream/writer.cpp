//
// Created by afedoseev on 19.08.2021.
//

#include <cstring>

#include <lib/stream/writer.hpp>

void lib::stream::IOutput::print_number(std::uint64_t number)
{
    char buffer[8 * sizeof(std::uint64_t) + 1]; // Assumes 8-bit chars plus zero byte.
    char* str = &buffer[sizeof(buffer) - 1];

    *str = '\0';
    auto base = static_cast<std::uint8_t>(_base);

    do {
        char symbol = number % base;
        number /= base;

        *--str = symbol < 10 ? symbol + '0' : symbol + 'A' - 10;
    } while (number > 0);

    operator<<(static_cast<const char*>(str));
}

void lib::stream::IOutput::print_number(std::int64_t number)
{
    if (_base == IntegerBase::Dec) {
        if (number < 0) {
            write(static_cast<std::uint8_t>('-'));
            number = -number;
        }

        print_number(static_cast<std::uint64_t>(number));
    } else {
        print_number(static_cast<std::uint64_t>(number));
    }
}

auto lib::stream::IOutput::operator<<(const char* data) -> IOutput&
{
    std::size_t data_size = std::strlen(data);

    for (std::size_t i = 0; i < data_size; ++i)
        write(static_cast<std::uint8_t>(data[i]));

    return *this;
}

auto lib::stream::IOutput::operator<<(std::uint8_t data) -> IOutput&
{
    operator<<(static_cast<std::uint64_t>(data));
    return *this;
}

auto lib::stream::IOutput::operator<<(std::int8_t data) -> IOutput&
{
    operator<<(static_cast<std::int64_t>(data));
    return *this;
}

auto lib::stream::IOutput::operator<<(std::uint16_t data) -> IOutput&
{
    operator<<(static_cast<std::uint64_t>(data));
    return *this;
}

auto lib::stream::IOutput::operator<<(std::int16_t data) -> IOutput&
{
    operator<<(static_cast<std::int64_t>(data));
    return *this;
}

auto lib::stream::IOutput::operator<<(std::uint32_t data) -> IOutput&
{
    operator<<(static_cast<std::uint64_t>(data));
    return *this;
}

auto lib::stream::IOutput::operator<<(std::int32_t data) -> IOutput&
{
    operator<<(static_cast<std::int64_t>(data));
    return *this;
}

auto lib::stream::IOutput::operator<<(std::uint64_t data) -> IOutput&
{
    print_number(data);
    return *this;
}

auto lib::stream::IOutput::operator<<(std::int64_t data) -> IOutput&
{
    print_number(data);
    return *this;
}

auto lib::stream::IOutput::operator<<(function_ptr func) -> IOutput&
{
    func(*this);
    return *this;
}

void lib::stream::IOutput::print_float(float number)
{
    if (number < 0.0f) {
        write(static_cast<std::uint8_t>('-'));
        number = -number;
    }

    std::uint8_t digits = _digits;

    double rounding = 0.5;
    for (std::uint8_t i = 0; i < digits; ++i)
        rounding /= 10.0;

    number += static_cast<float>(rounding);

    auto int_part = static_cast<std::uint64_t>(number);
    double remainder = number - static_cast<double>(int_part);

    print_number(int_part);

    if (digits > 0)
        write(static_cast<std::uint8_t>('.'));

    while (digits-- > 0) {
        remainder *= 10.0;
        auto to_print = static_cast<std::uint8_t>(remainder);
        operator<<(to_print);
        remainder -= to_print;
    }
}
auto lib::stream::IOutput::operator<<(float data) -> IOutput&
{
    print_float(data);
    return *this;
}
