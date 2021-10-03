//
// Created by Антон Федосеев on 30.08.2021.
//

#include <bsp.hpp>
#include <csp.hpp>

void bsp::init()
{
    csp::init();
    csp::rcc::init();
}
