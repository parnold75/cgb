#include "serial_io.hpp"

namespace cgb
{

serial_io::serial_io( const char* device_name) :
    reader_callback( [](const cgb_byte*, cgb_size){} )
{
}

serial_io::~serial_io()
{
}

void serial_io::write(cgb_byte * buf, cgb_size size)
{
}

void serial_io::read(cgb_byte * buf, cgb_size size)
{
}

void serial_io::set_reader_callback( reader_callback_event callback )
{
    reader_callback = callback == nullptr ? [](const cgb_byte*, cgb_size){} : callback;
}


} // namespace cgb
