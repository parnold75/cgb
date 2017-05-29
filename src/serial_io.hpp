#ifndef __SERIAL_HPP__
#define __SERIAL_HPP__

#include <functional>
#include <memory>

#include "cgb.h"

namespace cgb
{

using reader_callback_event = std::function< void(const cgb_byte *, cgb_size)>;


class serial_io
{
    private:
    reader_callback_event reader_callback;

    public:
    serial_io(const char* device_name);
    ~serial_io();

    void write(cgb_byte * buf, cgb_size size);
    void read(cgb_byte * buf, cgb_size size);

    void set_reader_callback( reader_callback_event callback);
};

using unique_serial_io = std::unique_ptr< serial_io >;

} // namespace cgb

#endif // __SERIAL_HPP__
