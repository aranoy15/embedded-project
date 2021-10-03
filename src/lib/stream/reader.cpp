//
// Created by afedoseev on 20.08.2021.
//


#include <lib/stream/reader.hpp>
#include <lib/stream/actions.hpp>

auto lib::stream::Reader::operator>>(actions::base_reader& action) -> Reader&
{
    action.action(*this);
    return *this;
}

auto lib::stream::Reader::operator>>(actions::base_reader&& action) -> Reader&
{
    action.action(*this);
    return *this;
}

int lib::stream::Reader::timed_read()
{
    int result = -1;

    tick_t start = os::task::current_tick();

    do {
        result = read();
        if (result >= 0) return result;
    } while(os::task::current_tick() - start < timeout());

    return -1;
}

int lib::stream::Reader::timed_peek()
{
    int result = -1;

    tick_t start = os::task::current_tick();

    do {
        result = peek();
        if (result >= 0) return result;
    } while (os::task::current_tick() - start < timeout());

    return -1;
}

int lib::stream::Reader::peek_next_digit(LookaheadMode lookahead,
                                         bool detect_decimal)
{
    int result;

    while (true) {
        result = timed_peek();

        if (result < 0 or
            result == '-' or
            (result >= '0' and result <= '9') or
            (detect_decimal and result == '.')) {
            return result;
        }

        switch (lookahead) {
            case LookaheadMode::SkipNone: return -1;
            case LookaheadMode::SkipWhitespace:
                switch (result) {
                    case ' ':
                    case '\t':
                    case '\r':
                    case '\n': break;
                    default: return -1;
                };
            case LookaheadMode::SkipAll:
                break;
        }

        read();
    }
}

std::size_t lib::stream::Reader::read_bytes(std::uint8_t* buffer,
                                            std::size_t size)
{
    std::size_t count = 0;

    while (count < size) {
        int item = timed_read();
        if (item < 0) break;
        *buffer++ = static_cast<std::uint8_t>(item);
        count++;
    }

    return count;
}

std::size_t lib::stream::Reader::read_bytes_until(std::uint8_t terminator,
                                                  std::uint8_t* buffer,
                                                  std::size_t size)
{
    std::size_t count = 0;

    while (count < size) {
        int item = timed_read();
        if (item < 0 or item == terminator) break;
        *buffer++ = static_cast<std::uint8_t>(item);
        count++;
    }

    return count;
}

std::int64_t lib::stream::Reader::parse_int(
    lib::stream::Reader::LookaheadMode lookahead, char ignore)
{
    bool is_negative = false;
    std::int64_t result = 0;
    int item = -1;

    item = peek_next_digit(lookahead, false);

    if (item < 0) return 0;

    do {
        if (item == ignore) {
            //ignore this character
        }  else if (item == '-') {
            is_negative = true;
        } else if (item >= '0' and item <= '9') {
            result = result * 10 + item - '0';
        }

        read();
        item = timed_peek();
    } while ((item >= '0' and item <= '9') or item == ignore);

    if (is_negative)
        result = -result;

    return result;
}

float lib::stream::Reader::parse_float(LookaheadMode lookahead, char ignore)
{
    bool is_negative = false;
    bool is_fraction = false;
    std::int32_t result = 0;
    int item = -1;
    float fraction = 1.0;

    do {
        if (item == ignore) {
            //ignore
        } else if (item == '-') {
            is_negative = true;
        } else if (item == '.') {
            is_fraction = true;
        } else if (item >= '0' and item <= '9') {
            result = result * 10 + item - '0';

            if (is_fraction)
                fraction *= 0.1;
        }

        read();
        item = timed_peek();
    } while ((item >= '0' and item <= '9') or
             (item == '.' and not is_fraction) or
             item == ignore);

    if (is_negative)
        result = -result;

    if (is_fraction)
        return static_cast<float>(result) * fraction;
    else
        return static_cast<float>(result);
}

auto lib::stream::Reader::operator>>(int32_t& data) -> Reader&
{
    data = (int32_t)parse_int(_lookahead_mode, ' ');
    return *this;
}

auto lib::stream::Reader::operator>>(uint32_t& data) -> Reader&
{
    data = (uint32_t)parse_int(_lookahead_mode, ' ');
    return *this;
}
