#ifndef CSP_SHARE_CSP
#define CSP_SHARE_CSP

#include <cinttypes>

namespace csp
{
void init();
void error_callback(const char* file, int line);
void jump_to(std::uint32_t address);

namespace critical
{
    void enter();
    void exit();

    struct Section final
    {
        Section() { enter(); }
        ~Section() { exit(); }
    };
}
}

#endif /* CSP_SHARE_CSP */
