#include <flash.hpp>

namespace csp::flash
{

constexpr std::uint32_t start_address = 0x08000000;
constexpr std::uint32_t end_address = 0x0801FC00;
constexpr std::size_t current_page_size = 0x400;

std::uint32_t page_address(std::uint32_t address)
{
    std::uint32_t result = 0;

    return result;
}
}
