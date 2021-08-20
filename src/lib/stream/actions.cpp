//
// Created by afedoseev on 19.08.2021.
//

#include <lib/stream/actions.hpp>

void lib::stream::actions::endl::action(lib::stream::Writer& stream)
{
     stream << static_cast<const char*>("\r\n");
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
    stream.read_bytes(_buffer, _size);
}

lib::stream::actions::lookahead::lookahead(lookahead_mode_t lookahead)
    : _lookahead(lookahead)
{
}

void lib::stream::actions::lookahead::action(lib::stream::Reader& stream)
{
    stream.set_lookahead_mode(_lookahead);
}
