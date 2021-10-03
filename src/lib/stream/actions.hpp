//
// Created by afedoseev on 19.08.2021.
//

#ifndef EMBEDDED_PROJECT_STREAM_ACTIONS_HPP
#define EMBEDDED_PROJECT_STREAM_ACTIONS_HPP

#include <lib/stream/writer.hpp>
#include <lib/stream/reader.hpp>

namespace lib::stream::actions
{

struct base_writer
{
    virtual void action(Writer& stream) = 0;
};

struct base_reader
{
    virtual void action(Reader& stream) = 0;
};

struct endl : base_writer
{
    void action(Writer& stream) override;
};

struct int_base : base_writer
{
    using integer_base_t = lib::stream::Writer::IntegerBase;
    explicit int_base(integer_base_t base) : base(base) {}
    void action(Writer& stream) override;
    integer_base_t base;
};

struct buffer_write : base_writer
{
public:
    buffer_write(std::uint8_t* buffer, std::size_t size);
    void action(Writer& stream) override;

private:
    std::uint8_t* _buffer;
    std::size_t _size;
};

struct byte_write : base_writer
{
public:
    explicit byte_write(std::uint8_t byte);
    void action(Writer& stream);

private:
    std::uint8_t _byte;
};

struct buffer_read : base_reader
{
public:
    buffer_read(std::uint8_t* buffer, std::size_t size);

    void action(Reader& stream) override;

private:
    std::uint8_t* _buffer;
    std::size_t  _size;
};

struct byte_read : base_reader
{
    using tick_t = os::task::tick_t;

    explicit byte_read(std::uint8_t& byte, tick_t timeout = 100);
    void action(Reader& stream) override;


private:
    std::uint8_t& _byte;
    tick_t _timeout;
};

struct lookahead : base_reader
{
    using lookahead_mode_t = Reader::LookaheadMode;

    explicit lookahead(lookahead_mode_t lookahead);
    void action(Reader& stream) override;

private:
    lookahead_mode_t _lookahead;
};
}

#endif  // EMBEDDED_PROJECT_STREAM_ACTIONS_HPP
