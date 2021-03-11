#include <csp/share/flash.hpp>
#include <hal.hpp>

namespace
{
constexpr std::uint32_t start_address = 0x08000000;
constexpr std::uint32_t end_address = 0x08040000;

constexpr std::uint32_t addr_flash_sector_0 = 0x08000000; /* Base @ of Sector 0, 16 Kbytes */
constexpr std::uint32_t addr_flash_sector_1 = 0x08004000; /* Base @ of Sector 1, 16 Kbytes */
constexpr std::uint32_t addr_flash_sector_2 = 0x08008000; /* Base @ of Sector 2, 16 Kbytes */
constexpr std::uint32_t addr_flash_sector_3 = 0x0800C000; /* Base @ of Sector 3, 16 Kbytes */
constexpr std::uint32_t addr_flash_sector_4 = 0x08010000; /* Base @ of Sector 4, 64 Kbytes */
constexpr std::uint32_t addr_flash_sector_5 = 0x08020000; /* Base @ of Sector 5, 128 Kbytes */

}

namespace csp::flash
{
std::uint32_t page_address(std::uint32_t address)
{
    if (address >= end_address)
        return 0;

    if (address >= addr_flash_sector_0 and address < addr_flash_sector_1)
        return addr_flash_sector_0;
    else if (address >= addr_flash_sector_1 and address < addr_flash_sector_2)
        return addr_flash_sector_1;
    else if (address >= addr_flash_sector_2 and address < addr_flash_sector_3)
        return addr_flash_sector_2;
    else if (address >= addr_flash_sector_3 and address < addr_flash_sector_4)
        return addr_flash_sector_3;
    else if (address >= addr_flash_sector_4 and address < addr_flash_sector_5)
        return addr_flash_sector_4;
    else if (address >= addr_flash_sector_5)
        return addr_flash_sector_5;

    return 0;
}

std::size_t page_size(std::uint32_t page_address)
{
     if (address >= end_address)
        return 0;

    if (address >= addr_flash_sector_0 and address < addr_flash_sector_1)
        return 0;
    else if (address >= addr_flash_sector_1 and address < addr_flash_sector_2)
        return 1;
    else if (address >= addr_flash_sector_2 and address < addr_flash_sector_3)
        return 2;
    else if (address >= addr_flash_sector_3 and address < addr_flash_sector_4)
        return 3;
    else if (address >= addr_flash_sector_4 and address < addr_flash_sector_5)
        return 4;
    else if (address >= addr_flash_sector_5)
        return 5;

    return 0;
}

bool erase(std::uint32_t start, std::uint32_t end)
{
    FLASH_EraseInitTypeDef erase_inits;
	std::uint32_t errors;

    std::uint32_t start_address = csp::flash::page_address(start);
    std::uint32_t end_address = start_address + (end - start);
	std::uint32_t count = page_size(end_address);

    erase_inits.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	erase_inits.TypeErase = FLASH_TYPEERASE_PAGES;
	erase_inits.PageAddress = start_address;
	erase_inits.NbPages = count;

	return (HAL_FLASHEx_Erase(&erase_inits, &errors) == HAL_OK);
}
}
