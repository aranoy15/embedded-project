//
// Created by afedoseev on 19.08.2021.
//

#include <lib/stream/actions.hpp>

void lib::stream::actions::endl::action(lib::stream::Writer& stream)
{
     stream << static_cast<const char*>("\r\n");
     stream.flush();
}
void lib::stream::actions::int_base::action(lib::stream::Writer& stream)
{
    stream.set_base(base);
}

lib::stream::actions::buffer_read::buffer_read(std::uint8_t* buffer,
                                               std::size_t size)
    : _buffer(buffer), _size(size)
{
}

void lib::stream::actions::buffer_read::action(Reader& stream)
{
    std::size_t size = stream.read_bytes(_buffer, _size);

    if (size > 0) stream.set_result(Reader::Result::Ok);
    else stream.set_result(Reader::Result::Error);
}

lib::stream::actions::lookahead::lookahead(lookahead_mode_t lookahead)
    : _lookahead(lookahead)
{
}

void lib::stream::actions::lookahead::action(lib::stream::Reader& stream)
{
    stream.set_lookahead_mode(_lookahead);
}

lib::stream::actions::byte_read::byte_read(uint8_t& byte, tick_t timeout)
    : _byte(byte), _timeout(timeout)
{
}

void lib::stream::actions::byte_read::action(Reader& stream)
{
    tick_t start = os::task::current_tick();

    do {
        int data = stream.read();

        if (data >= 0) {
            _byte = static_cast<std::uint8_t>(data);
            stream.set_result(Reader::Result::Ok);
            break;
        }

        os::task::delay(10);
    } while (os::task::current_tick() - start < _timeout);

    stream.set_result(Reader::Result::Timeout);
}

lib::stream::actions::buffer_write::buffer_write(std::uint8_t* buffer,
                                                 std::size_t size)
    : _buffer(buffer), _size(size)
{
}

void lib::stream::actions::buffer_write::action(Writer& stream)
{
    for (std::size_t i = 0; i < _size; ++i)
        stream.write(_buffer[i]);

    stream.set_result(Writer::Result::Ok);
}
lib::stream::actions::byte_write::byte_write(std::uint8_t byte)
    : _byte(byte)
{
}
void lib::stream::actions::byte_write::action(Writer& stream)
{
    stream.write(_byte);
    stream.set_result(Writer::Result::Ok);
}
