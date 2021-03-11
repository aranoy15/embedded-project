#ifndef SRC_LIB_WAKE_WAKE
#define SRC_LIB_WAKE_WAKE

#include <cinttypes>

namespace lib::wake
{
struct Packet final
{
    Packet();
    Packet(uint8_t address, uint8_t command);
    ~Packet();

    Packet(const Packet& packet);
    Packet operator=(const Packet& packet);

    uint8_t address() const { return _address; }
    uint8_t command() const { return _command; }
    std::size_t length() const { return _length; }
    const uint8_t* data() const { return _data; }

    void set_address(uint8_t addr) { _address = (1 << 7) | addr; }
    void set_command(uint8_t cmd) { _command = (cmd & ~(1 << 7)); }
    //void set_length(std::size_t len) { _command = (len & ~(1 << 7)); }
    void set_data(uint8_t* data) { _data = data; }

private:
    uint8_t _address;
    uint8_t _command;
    std::size_t _length;
    uint8_t* _data;
};

class Protocol final
{
public:
    static constexpr uint8_t max_frame_size = 256;
    static constexpr uint8_t header_length = 4;
    static constexpr uint8_t crc_length = 1;

    static constexpr uint8_t address_index = 1;
    static constexpr uint8_t command_index = 2;
    static constexpr uint8_t length_index = 3;
    static constexpr uint8_t data_index = 4;

    enum
    {
        FEND = 0xC0,
        FESC = 0xDB,
        TFEND = 0xDC,
        TFESC = 0xDD
    };

public:
    Protocol() = delete;

    static bool process(uint8_t b) noexcept;
    static Packet unpack() noexcept;
    static void pack(const Packet& packet, uint8_t* data) noexcept;

    static void clear() noexcept;

private:
    static uint8_t crc8(uint8_t data[], std::size_t size) noexcept;

    static void push(uint8_t b) noexcept;
    static uint8_t pop() noexcept;
    static uint8_t packet_size() noexcept;

private:
    inline static uint8_t _data[max_frame_size];
    inline static uint8_t _size = 0;

    static uint8_t _crc_table[256];

    inline static bool _packet_started = false;
    inline static bool _byte_stuffing = false;
    inline static bool _data_started = false;
    inline static bool _packet_receive = false;
    inline static uint8_t _length = 0;
};

Packet::Packet() : _address(0), _command(0), _length(0), _data(nullptr)
{
}

Packet::Packet(uint8_t address, uint8_t command)
    : _address(address), _command(command), _length(0), _data(nullptr)
{
	_address = (1 << 7) | _address;
	_command = (command & ~(1 << 7));
}

Packet::~Packet() {}

Packet::Packet(const Packet& packet)
    : _address(packet._address),
      _command(packet._command),
      _length(packet._length),
      _data(packet._data)
{
}

bool Protocol::process(uint8_t b) noexcept
{
    if (_packet_receive) _packet_receive = false;

    if (b == FEND and _packet_started) {
        clear();
    } else if (_packet_started) {
        if (b == FEND) _byte_stuffing = true;

        if (b == TFEND and _byte_stuffing) {
            pop();
            push(FEND);
            _byte_stuffing = false;
        } else if (b == TFESC and _byte_stuffing) {
            pop();
            push(FESC);
            _byte_stuffing = false;
        } else {
            push(b);
        }

        if (_size == header_length) {
            _length = _data[length_index];
            _data_started = true;
        }

        if (_data_started and _size == packet_size()) {
            uint8_t crc_calc = crc8(_data, _size - 1);
            uint8_t crc_actual = _data[_size - 2];

            if (crc_calc == crc_actual) {
                _packet_receive = true;
                return true;
            } else {
                clear();
            }
        }
    } else if (b == FEND) {
        push(b);
        _packet_started = true;
    }

    return false;
}

void Protocol::push(uint8_t b) noexcept
{
    _data[_size++] = b;
}

uint8_t Protocol::pop() noexcept
{
    if (_size <= 0) return 0;
    uint8_t result = _data[_size--];

    return  result;
}

void Protocol::clear() noexcept
{
    _packet_started = false;
    _byte_stuffing = false;
    _data_started = false;
    _packet_receive = false;
    _size = 0;
    _length = 0;
}

uint8_t Protocol::packet_size() noexcept
{
    return header_length + _length + crc_length + (_byte_stuffing ? 1 : 0);
}

Packet Protocol::unpack() noexcept
{
    Packet result;

    if (_packet_receive) {
        result.set_address(_data[address_index]);
        result.set_command(_data[command_index]);
        result.set_length(_data[length_index]);
        result.set_data(&_data[data_index]);
    }

    return result;
}

uint8_t Protocol::crc8(uint8_t data[], std::size_t size)
{
    uint8_t result = 0xFF;

    for (std::size_t i = 0; i < size; ++i) {
        result = _crc_table[result ^ data[i]];
    }

    return  result;
}

}



#endif /* SRC_LIB_WAKE_WAKE */
