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
