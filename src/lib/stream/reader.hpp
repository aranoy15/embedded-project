#ifndef EMBEDDED_PROJECT_STREAM_READER_HPP
#define EMBEDDED_PROJECT_STREAM_READER_HPP

#include <os.hpp>

namespace lib::stream
{

class IInput
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

    enum class Result : std::uint8_t
    {
        None = 0,
        Ok,
        Error,
        Timeout
    };

public:
    virtual ~IInput() = default;

    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;

    IInput& operator>>(std::uint8_t& data);
    IInput& operator>>(std::int8_t& data);
    IInput& operator>>(std::uint16_t& data);
    IInput& operator>>(std::int16_t& data);
    IInput& operator>>(std::uint32_t& data);
    IInput& operator>>(std::int32_t& data);

    void set_timeout(tick_t timeout) { _timeout = timeout; }
    [[nodiscard]] tick_t timeout() const { return _timeout; }

    void set_lookahead_mode(LookaheadMode mode) { _lookahead_mode = mode; }
    [[nodiscard]] LookaheadMode lookahead_mode() const { return _lookahead_mode; }

    std::size_t read_bytes(std::uint8_t buffer[], std::size_t size);
    std::size_t read_bytes_until(std::uint8_t terminator, std::uint8_t buffer[],
                                 std::size_t size);
    std::int64_t parse_int(LookaheadMode lookahead, char ignore);
    float parse_float(LookaheadMode, char ignore);

    void set_result(Result result) { _last_result = result; }
    [[nodiscard]] Result get_result() const { return _last_result; }

protected:
    tick_t _timeout = 1000;
    LookaheadMode _lookahead_mode = LookaheadMode::SkipAll;
    Result _last_result = Result::None;

protected:
    int timed_read();
    int timed_peek();
    int peek_next_digit(LookaheadMode lookahead, bool detect_decimal);

};
}

#endif  // EMBEDDED_PROJECT_STREAM_READER_HPP
