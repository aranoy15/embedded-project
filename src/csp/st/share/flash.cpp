#include <flash_template.hpp>
#include <hal.hpp>

namespace csp::flash
{
bool write(std::uint32_t address, const std::uint8_t data[], std::size_t size)
{
    for (std::size_t i = 0; i < size; i += sizeof(std::uint32_t)) {
        std::uint32_t current_address = address + i;
        std::uint32_t word_to_write = *(std::uint32_t*)(data + i);

        auto status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, current_address, word_to_write);

        if (status != HAL_OK) return false;
    }

    return true;
}

bool read(std::uint32_t address, std::uint8_t data[], std::size_t size)
{
    volatile std::uint8_t *pointer = (volatile std::uint8_t*)(address);

    for (std::size_t i = 0; i < size; ++i) {
        data[i] = pointer[i];
    }

    return true;
}

void lock()
{
    HAL_FLASH_Lock();
}

void unlock()
{
    HAL_FLASH_Unlock();
}

void jump(const std::uint32_t address)
{
    //__disable_irq();

    SCB->VTOR = address;
    volatile std::uint32_t* vector_table = (std::uint32_t*)address;

    using jump_func_t = void(*)();
    jump_func_t reset_vector = (jump_func_t)vector_table[1];

    reset_vector();
    while(true);
}
}
