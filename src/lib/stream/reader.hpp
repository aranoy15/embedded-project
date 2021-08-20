//
// Created by afedoseev on 20.08.2021.
//

#ifndef EMBEDDED_PROJECT_STREAM_READER_HPP
#define EMBEDDED_PROJECT_STREAM_READER_HPP

#include <os.hpp>

namespace lib::stream
{

namespace actions
{
struct base_reader;
}

class Reader
{
public:
    using tick_t = os::task::tick_t;

public:
    enum class LookaheadMode : std::uint8_t
    {
        SkipAll,
        SkipNone,
        SkipWhitespace
    };

public:
    Reader();
    virtual ~Reader() = default;

    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;

    Reader& operator>>(actions::base_reader& action);
    Reader& operator>>(actions::base_reader&& action);

    Reader& operator>>(std::uint8_t& data);
    Reader& operator>>(std::int8_t& data);
    Reader& operator>>(std::uint16_t& data);
    Reader& operator>>(std::int16_t& data);
    Reader& operator>>(std::uint32_t& data);
    Reader& operator>>(std::int32_t& data);

    void set_timeout(tick_t timeout) { _timeout = timeout; }
    [[nodiscard]] tick_t timeout() const { return _timeout; }

    void set_lookahead_mode(LookaheadMode mode) { _lookahead_mode = mode; }
    [[nodiscard]] LookaheadMode lookahead_mode() const { return _lookahead_mode; }

    std::size_t read_bytes(std::uint8_t buffer[], std::size_t size);
    std::size_t read_bytes_until(std::uint8_t terminator, std::uint8_t buffer[],
                                 std::size_t size);
    std::int64_t parse_int(LookaheadMode lookahead, char ignore);
    float parse_float(LookaheadMode, char ignore);

protected:
    tick_t _timeout;
    LookaheadMode _lookahead_mode;

protected:
    int timed_read();
    int timed_peek();
    int peek_next_digit(LookaheadMode lookahead, bool detect_decimal);

//protected:
//    std::uint32_t parse_int(char ignore);
//    float parse_float(char ignore);
};
}

#endif  // EMBEDDED_PROJECT_STREAM_READER_HPP
