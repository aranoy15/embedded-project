#ifndef CSP_SHARE_FLASH
#define CSP_SHARE_FLASH

#include <cinttypes>

namespace csp::flash
{
void lock();
void unlock();
bool erase(std::uint32_t address);
bool full_erase();
bool write(std::uint32_t address, const std::uint8_t data[], std::size_t size);
bool read(std::uint32_t address, const std::uint8_t data[], std::size_t size);
void jump(const std::uint32_t address);
std::uint32_t page_address(std::uint32_t address);
std::size_t page_size(std::uint32_t page_address);
}

#endif /* CSP_SHARE_FLASH */
