#ifndef SRC_LIB_UTILS_CRC
#define SRC_LIB_UTILS_CRC

#include <cinttypes>

namespace lib::crc
{
template<typename base, std::uint8_t polynom, std::uint8_t init>
base calc(std::uint8_t data[], std::size_t size)
{
    base result = init;

    for (std::size_t i = 0; i < size; ++i) {
        result ^= data[i];

        for (std::uint8_t j = 0; j < 8; ++j) {
            result = (result & (1 << ((sizeof(base) * 8) - 1))) ? 
                        (result << 1) & polynom :
                        result << 1;
        }
    }
}
}

#endif /* SRC_LIB_UTILS_CRC */
