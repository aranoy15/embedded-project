#ifndef CSP_SHARE_UART
#define CSP_SHARE_UART

namespace csp::uart
{
enum class Number
{
    None = 0,
    _1 = 1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7
};

enum class Speed
{
    _9600 = 9600,
    _19200 = 19200,
    _38400 = 38400,
    _57600 = 57600,
    _115200 = 115200,
    _230400 = 230400
};

enum class WordLength
{
    _7,
    _8,
    _9
};

enum class StopBit
{
    _0_5,
    _1,
    _1_5,
    _2
};

enum class Parity
{
    None,
    Even,
    Odd
};

enum class FlowControl
{
    None,
    Rts,
    Cts,
    RtsAndCts
};

enum class TransferMode
{
    Simple = 0,
    Interrupt,
    //Dma
};

struct Settings
{
    Speed speed = Speed::_9600;
    WordLength word_length = WordLength::_8;
    StopBit stop_bit = StopBit::_1;
    Parity parity = Parity::None;
    FlowControl flow_control = FlowControl::None;
};

bool init(Number number, Settings settings);
bool deinit(Number number);
bool enable_interrupt(Number number);
bool disable_interrupt(Number number);
bool transmit(Number number, TransferMode mode, const uint8_t data[], std::size_t size);
bool abort_transmit(Number number, TransferMode mode);
bool receive(Number number, TransferMode mode, uint8_t data[], std::size_t size);
bool abort_receive(Number number, TransferMode mode);
void transmit_callback(Number number);
void receive_callback(Number number, std::size_t size);
void receive_error_callback(Number number);
}

#endif /* CSP_SHARE_UART */
