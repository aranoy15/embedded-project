#ifndef SRC_LIB_WAKE_WAKE
#define SRC_LIB_WAKE_WAKE

#include <cinttypes>
#include <limits>
#include <liner_buffer.hpp>
#include <crc.hpp>

namespace lib::wake
{
struct Packet final
{
    using byte_t = std::uint8_t;
    using address_t = byte_t;
    using command_t = byte_t;
    using length_t = std::size_t;

    Packet();
    Packet(address_t address, command_t command);
    ~Packet();

    Packet(const Packet& packet);
    Packet operator=(const Packet& packet);

    address_t address() const { return _address; }
    command_t command() const { return _command; }
    length_t length() const { return _length; }
    const byte_t* data() const { return _data; }

    void set_address(address_t addr) { _address = (1 << 7) | addr; }
    void set_command(command_t cmd) { _command = (cmd & ~(1 << 7)); }
    void set_length(std::size_t len) { _length = len; }
    void set_data(uint8_t* data) { _data = data; }

private:
    address_t _address;
    command_t _command;
    length_t _length;
    byte_t* _data;
};

template <typename address_type, typename command_type, typename length_type,
          std::size_t frame_size, length_type crc_base, length_type crc_polynom>
class Protocol final
{

public:
    using byte_t = std::uint8_t;
    using data_t = lib::buffers::liner_buffer<uint8_t, frame_size>;

    static constexpr length_type address_index = 0;
    static constexpr length_type command_index = address_index + sizeof(address_type);
    static constexpr length_type length_index = command_index + sizeof(command_type);
    static constexpr length_type data_index = length_index + sizeof(length_type);

    enum
    {
        FEND = 0xC0,
        FESC = 0xDB,
        TFEND = 0xDC,
        TFESC = 0xDD
    };

public:
    Protocol() = delete;

    static bool process(byte_t b) noexcept;
    static Packet unpack() noexcept;
    static void pack(const Packet& packet, byte_t* data) noexcept;

    static void clear() noexcept;

private:
    static length_type packet_size() noexcept;
    static std::size_t header_length() noexcept;
    static length_type checksum(const byte_t data[], std::size_t size);

private:
    inline static data_t _data;

    inline static bool _packet_started = false;
    inline static bool _byte_stuffing = false;
    inline static bool _data_started = false;
    inline static bool _packet_receive = false;
    inline static length_type _length = 0;
};

template <typename address_type, typename command_type, typename length_type,
          std::size_t frame_size, length_type crc_base, length_type crc_polynom>
std::size_t Protocol<address_type, command_type, length_type, frame_size,
                     crc_base, crc_polynom>::header_length() noexcept
{
    std::size_t result = 0;

    result += sizeof(address_type);
    result += sizeof(command_type);
    result += sizeof(length_type);

    return result;
}

template <typename address_type, typename command_type, typename length_type,
          std::size_t frame_size, length_type crc_base, length_type crc_polynom>
length_type Protocol<address_type, command_type, length_type, frame_size,
                     crc_base, crc_polynom>::packet_size() noexcept
{
    length_type result = 0;

    result += sizeof(address_type);
    result += sizeof(command_type);
    result += sizeof(length_type);
    result += frame_size;

    return result;
}

template <typename address_type, typename command_type, typename length_type,
          std::size_t frame_size, length_type crc_base, length_type crc_polynom>
bool Protocol<address_type, command_type, length_type, frame_size,
                     crc_base, crc_polynom>::process(uint8_t b) noexcept
{
    if (_packet_receive) _packet_receive = false;

    if (b == FEND and _packet_started) {
        _data.reset();
    } else if (_packet_started) {
        if (b == FEND) _byte_stuffing = true;

        if (b == TFEND and _byte_stuffing) {
            _data.pop();
            _data.put(FEND);
            _byte_stuffing = false;
        } else if (b == TFESC and _byte_stuffing) {
            _data.pop();
            _data.put(FESC);
            _byte_stuffing = false;
        } else {
            _data.put(b);
        }

        if (_data.size() == header_length()) {
            _length = *reinterpret_cast<const length_type*>(&_data.data()[length_index]);
            _data_started = true;
        }

        if (_data_started and _data.size() == packet_size()) {
            length_type crc_calc = checksum(_data.data(), _data.size() - sizeof(length_type));
            length_type crc_actual = _data.data()[_data.size() - sizeof(length_type)];

            if (crc_calc == crc_actual) {
                _packet_receive = true;
                return true;
            } else {
                clear();
            }
        }
    } else if (b == FEND) {
        _data.put(b);
        _packet_started = true;
    }

    return false;
}

template <typename address_type, typename command_type, typename length_type,
          std::size_t frame_size, length_type crc_base, length_type crc_polynom>
void Protocol<address_type, command_type, length_type, frame_size,
                     crc_base, crc_polynom>::clear() noexcept
{
    _data.reset();
    _packet_started = false;
    _byte_stuffing = false;
    _data_started = false;
    _packet_receive = false;
    _length = 0;
}

template <typename address_type, typename command_type, typename length_type,
          std::size_t frame_size, length_type crc_base, length_type crc_polynom>
Packet Protocol<address_type, command_type, length_type, frame_size,
                     crc_base, crc_polynom>::unpack() noexcept
{
    Packet result;

    if (_packet_receive) {
        //result.set_address(_data.data()[address_index]);
        //result.set_command(_data.data()[command_index]);
        //result.set_length(_data.data()[length_index]);
        //result.set_data(&_data.data()[data_index]);

        result.set_address(*reinterpret_cast<address_type*>(_data.data()[address_index]));
        result.set_address(*reinterpret_cast<command_type*>(_data.data()[command_index]));
        result.set_address(*reinterpret_cast<length_type*>(_data.data()[length_index]));
        result.set_address(*reinterpret_cast<uint8_t*>(_data.data()[data_index]));
    }

    return result;
}

template <typename address_type, typename command_type, typename length_type,
          std::size_t frame_size, length_type crc_base, length_type crc_polynom>
length_type Protocol<address_type, command_type, length_type, frame_size,
                     crc_base, crc_polynom>::checksum(const byte_t data[], std::size_t size)
{
    return lib::crc::calc<length_type, crc_base, crc_polynom>(data, size);
}

}



#endif /* SRC_LIB_WAKE_WAKE */
