//
// Created by Антон Федосеев on 07.08.2021.
//

#ifndef SRC_CSP_SHARE_TIMER_TEMPLATE_HPP
#define SRC_CSP_SHARE_TIMER_TEMPLATE_HPP

namespace csp::timer
{
enum class Number
{
    None = 0,
    _1,
    _2,
    _3,
    _4,
    _5
};

enum class Mode
{
    CountUp,
    CountDown
};

using frequency_t = std::uint32_t;
using value_t = std::uint32_t;

bool init(Number number, Mode mode, frequency_t frequency);
value_t value(Number number);
bool deinit(Number number);
bool enable_interrupt(Number number);
bool disable_interrupt(Number number);

void timer_callback(Number number);
}

#endif  // SRC_CSP_SHARE_TIMER_TEMPLATE_HPP
