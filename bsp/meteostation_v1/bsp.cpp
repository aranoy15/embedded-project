#include <bsp.hpp>
#include <csp.hpp>

namespace bsp
{
void init()
{
    csp::init();
    csp::rcc::init();
}
}




