//
// Created by afedoseev on 19.08.2021.
//

#ifndef EMBEDDED_PROJECT_STREAM_ACTIONS_HPP
#define EMBEDDED_PROJECT_STREAM_ACTIONS_HPP

#include <lib/stream/writer.hpp>

namespace lib::stream::actions
{

struct base_writer
{
    virtual void action(Writer& stream) = 0;
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
}

#endif  // EMBEDDED_PROJECT_STREAM_ACTIONS_HPP
