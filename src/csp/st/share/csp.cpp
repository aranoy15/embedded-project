//
// Created by afedoseev on 16.08.2021.
//

#include <csp.hpp>
#include <hal.hpp>

void csp::jump_to(std::uint32_t address)
{
    __disable_irq();  // disable interrupts

    SCB->VTOR = address;
    auto int_vector_table = (uint32_t *)address;

    typedef void (*ResetVectorFunc)();
    auto reset_vector = (ResetVectorFunc)int_vector_table[1];

    reset_vector();
}

void csp::critical::enter()
{
    __disable_irq();
}

void csp::critical::exit()
{
    __enable_irq();
}

