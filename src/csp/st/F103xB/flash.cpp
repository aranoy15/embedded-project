#include <csp/share/flash.hpp>
#include <hal.hpp>

namespace
{
constexpr std::uint32_t start_address = 0x08000000;
constexpr std::uint32_t end_address = 0x08010000;
constexpr std::size_t page_size = 0x400;
constexpr std::uint32_t page_count = (end_address - start_address) / ::page_size;
}

namespace csp::flash
{
std::uint32_t page_address(std::uint32_t address)
{
    if (address >= end_address)
        return 0;

    for (std::size_t i = 0; i < ::page_count; ++i) {
        std::uint32_t current_page_address = start_address + (::page_size * i);
        std::uint32_t current_end_address = current_page_address + ::page_size;

        if (address >= current_page_address and address < current_end_address)
            return current_page_address;
    }

    return 0;
}

std::size_t page_size(std::uint32_t page_address)
{
    if (page_address < start_address or page_address > end_address)
        return 0;
    
    return ::page_size;
}

bool erase(std::uint32_t start, std::uint32_t end)
{
    FLASH_EraseInitTypeDef erase_inits;
	std::uint32_t errors;

    std::uint32_t start_address = csp::flash::page_address(start);
    std::uint32_t end_address = start_address + (end - start);
	std::uint32_t count = page_address(end_address) / page_size(end_address);

	erase_inits.TypeErase = FLASH_TYPEERASE_PAGES;
	erase_inits.PageAddress = start_address;
	erase_inits.NbPages = count;

	return (HAL_FLASHEx_Erase(&erase_inits, &errors) == HAL_OK);
}
}
