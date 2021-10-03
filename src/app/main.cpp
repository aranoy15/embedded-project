//
// Created by Антон Федосеев on 27.06.2021.
//

#include <bsp.hpp>
#include <os.hpp>

using scheduler_t = os::Scheduler<15 * 1024, 5>;

class BlinkTask : public os::task::Task
    {
    public:
        BlinkTask()
        : os::task::Task(os::task::Priority::Idle, 1024, 100)
        {}

        void setup() noexcept override
        {
            bsp::gpio::status::init();
        }

        void loop() noexcept override
        {
            bsp::gpio::status::toggle();
        }
    };


int main()
{
    bsp::init();

    scheduler_t::reg<BlinkTask>();

    scheduler_t::init();
    scheduler_t::start();
}

/*
#include <csp.hpp>
#include <os.hpp>

#include <lib/data/queue.hpp>
#include <lib/data/string.hpp>
#include <lib/data/vector.hpp>
#include <lib/stream/writer.hpp>
#include <lib/stream/actions.hpp>
#include <lib/stream/reader.hpp>

using scheduler_t = os::Scheduler<15 * 1024, 5>;

using status_led = csp::gpio::Gpio<
    csp::gpio::Port::_C,
    csp::gpio::Pin::_13,
    csp::gpio::Mode::PushPull>;

const csp::usb::Number usb_number = csp::usb::Number::_1;

const csp::i2c::Number i2c_number = csp::i2c::Number::_1;
const csp::i2c::Mode i2c_mode = csp::i2c::Mode::Standard;
const csp::i2c::TransferMode i2c_transfer_mode = csp::i2c::TransferMode::Simple;

const csp::uart::Number mhz_number = csp::uart::Number::_1;
const csp::uart::TransferMode mhz_transfer_mode = csp::uart::TransferMode::Interrupt;
const csp::uart::Settings mhz_settings = {
    .speed = csp::uart::Speed::_9600,
    .word_length = csp::uart::WordLength::_8,
    .stop_bit = csp::uart::StopBit::_1,
    .parity = csp::uart::Parity::None
};

const std::uint16_t bme280_address = 0x76;

static std::uint8_t receive_data[16] = { 0x00 };
static std::size_t receive_size = 0;

using queue_t = lib::data::queue<std::uint8_t, 1024>;
static queue_t log_queue;

void csp::uart::transmit_callback(Number number)
{
    if (number != mhz_number) return;
}

void csp::uart::receive_callback(Number number, std::size_t size)
{
    if (number != mhz_number and size == 0) return;
    receive_size = size;

    csp::uart::abort_receive(mhz_number, mhz_transfer_mode);
    csp::uart::receive(mhz_number, mhz_transfer_mode, receive_data, sizeof(receive_data));
}

void csp::usb::receive_callback(Number number, uint8_t data[], std::size_t size)
{
    if (number != usb_number) return;

    for (std::size_t i = 0; i < size; ++i)
        log_queue.push(data[i]);
}

class LogStream : public lib::stream::Writer,
                  public lib::stream::Reader,
                  public lib::lock::Lockable
{
public:
    LogStream() : Writer(), Reader(), _mutex() { set_timeout(0); }
    ~LogStream() override = default;

    void lock() override
    {
        _mutex.lock();
    }

    void unlock() override
    {
        _mutex.unlock();
    }

    int available() override
    {
        return static_cast<int>(log_queue.size());
    }

    int read() override
    {
        if (log_queue.empty()) return -1;
        return log_queue.pop();
    }

    int peek() override
    {
        if (log_queue.empty()) return -1;

        std::uint8_t result = 0;
        if (not log_queue.peek(result))
            return -1;

        return result;
    }

    void write(std::uint8_t data) override
    {
        csp::usb::transmit(usb_number, &data, sizeof(data));
    }

    void flush() override {}

private:
    os::mutex_t _mutex;
};

class I2cStream : public lib::stream::Writer,
                  public lib::stream::Reader,
                  public lib::lock::Lockable
{
public:
    I2cStream() : _address(0), _mutex() {}
    ~I2cStream() override = default;

    void lock() override { _mutex.lock(); }
    void unlock() override { _mutex.unlock(); }

    int available() override { return 1; }

    int read() override
    {
        std::uint8_t data = 0;

        if (csp::i2c::receive(i2c_number, i2c_transfer_mode, _address, &data, sizeof(data)))
            return static_cast<int>(data);

        return -1;
    }

    int peek() override
    {
        return -1;
    }

    void write(std::uint8_t data) override
    {
        _data.push_back(data);
    }

    void flush() override
    {
        csp::i2c::transmit(i2c_number, i2c_transfer_mode, _address,
                           _data.data(), _data.size());
        _data.clear();
    }

    void set_address(std::uint16_t address) { _address = address; }
    [[nodiscard]] std::uint16_t get_address() const { return _address; }

private:
    std::uint16_t _address;
    os::mutex_t _mutex;
    lib::data::vector<std::uint8_t, 32> _data;
};

namespace
{
LogStream& log_stream()
{
    static LogStream instance;
    return instance;
}
}

class BlinkTask : public os::task::Task
{
public:
    BlinkTask()
        : os::task::Task(os::task::Priority::Idle, 1024, 100)
    {}

    void setup() noexcept override
    {
        status_led::init();
    }

    void loop() noexcept override
    {
        status_led::toggle();
        //LogStream stream;
        static lib::data::string<128> input_str;

        {
            lib::lock::Lock lock(&log_stream());

            log_stream() >> input_str;
            if (not input_str.empty()) {
                log_stream() << "Receive: " << input_str
                             << lib::stream::actions::endl();
                input_str.clear();
            }
        }
    }
};

namespace
{
I2cStream& i2c_stream()
{
    static I2cStream instance;
    return instance;
}
}

class SensorTask : public os::task::Task
{
public:
    SensorTask()
        : os::task::Task(os::task::Priority::High, 3 * 1024, 1000)
    {}

    void setup() noexcept override
    {
        csp::i2c::init(i2c_number, i2c_mode);

        csp::uart::init(mhz_number, mhz_settings);
        csp::uart::receive(mhz_number, mhz_transfer_mode, receive_data, sizeof(receive_data));
        csp::uart::enable_interrupt(mhz_number);
    }

    void loop() noexcept override
    {
        static std::uint8_t data[] = { 0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };

        std::uint16_t co2_value = (receive_data[2] << 8) | receive_data[3];

        using endl = lib::stream::actions::endl;

        float f_data = 13.27f;

        {
            lib::lock::Lock lock(&log_stream());

            log_stream() << "CO2 concentration: " << co2_value << endl();
            log_stream() << "Float test: " << f_data << endl();
        }

        csp::uart::transmit(mhz_number, mhz_transfer_mode, data, sizeof(data));

        //i2c_scanner();
        {
            lib::lock::Lock lock(&i2c_stream());

            i2c_stream().set_address(0x76);

            i2c_stream() << lib::stream::actions::byte_write(0xD0);
            i2c_stream().flush();

            std::uint8_t result = 0;
            i2c_stream() >> lib::stream::actions::byte_read(result);

            {
                lib::lock::Lock log_lock(&log_stream());
                log_stream() << "I2C read result: " << result << lib::stream::actions::endl();
            }
        }
    }

private:
    static void i2c_scanner()
    {
        using namespace lib::stream::actions;

        std::size_t count = 0;
        for (std::uint16_t i = 0; i < 128; ++i) {
            //LogStream() << int_base(int_base::integer_base_t::Hex) << "Check i2c address: 0x" << i << endl();
            if (csp::i2c::is_device_ready(i2c_number, i)) {
                count++;
                LogStream() << int_base(int_base::integer_base_t::Hex) << "Ok I2C address: 0x" << i << endl();
            }
        }

        if (count == 0)
            LogStream() << "Not found any devices" << endl();
    }
};

int main()
{
    csp::init();
    csp::rcc::init();
    csp::usb::device::cdc::init(usb_number);

    scheduler_t::reg<BlinkTask>();
    scheduler_t::reg<SensorTask>();

    scheduler_t::init();
    scheduler_t::start();
}
*/
