//
// Created by afedoseev on 19.08.2021.
//

#ifndef SRC_LIB_STREAM_WRITER
#define SRC_LIB_STREAM_WRITER

#include <cinttypes>

namespace lib::stream
{
class IOutput
{

public:
    using function_ptr = void(*)(IOutput&);

public:
    enum class IntegerBase : std::uint8_t
    {
        Dec = 10,
        Hex = 16,
        Oct = 8,
        Bin = 2
    };

    enum class Result : std::uint8_t
    {
        None = 0,
        Ok,
        Error,
        Timeout
    };

public:
    virtual ~IOutput() = default;

    virtual void write(std::uint8_t data) = 0;
    virtual void flush() = 0;

    IOutput& operator<<(std::uint8_t data);
    IOutput& operator<<(std::int8_t data);
    IOutput& operator<<(std::uint16_t data);
    IOutput& operator<<(std::int16_t data);
    IOutput& operator<<(std::uint32_t data);
    IOutput& operator<<(std::int32_t data);
    IOutput& operator<<(std::uint64_t data);
    IOutput& operator<<(std::int64_t data);

    IOutput& operator<<(const char* data);

    IOutput& operator<<(float data);

    void set_base(IntegerBase base) { _base = base; }
    void set_result(Result result) { _last_result = result; }

    [[nodiscard]] Result get_result() const { return _last_result; }

    IOutput& operator<<(function_ptr func);

    void set_base(IntegerBase base) { _base = base; }

private:
    IntegerBase _base = IntegerBase::Dec;
    std::uint8_t _digits = 2;
    Result _last_result = Result::None;

private:
    void print_number(std::int64_t number);
    void print_number(std::uint64_t number);
    void print_float(float number);
};

inline void endl(IOutput& stream)
{
    stream.operator<<(static_cast<std::uint8_t>('\n'));
}
}  // namespace lib::stream

#endif /* SRC_LIB_STREAM_WRITER */
