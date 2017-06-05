#ifndef __CGB_BUFFER_HPP__
#define __CGB_BUFFER_HPP__

#include <memory>
#include <functional>

#include "cgb.h"

namespace cgb
{

/*
    Sende bzw. Empfangs buffer.
*/    
class cgb_buffer 
{
    public:
    const static cgb_size MAX_SIZE = 127;
    cgb_size size;
    cgb_byte buffer[MAX_SIZE];
};

// "typedef" - für unique_ptr und deleter
// Objekt wird vom Daten-Pool erstellt und durch den deleter wieder zugeführt.
using unique_cgb_buffer = std::unique_ptr< cgb_buffer, std::function<void(cgb_buffer*)> >;

} // namespace cgb

#endif // __CGB_BUFFER_HPP__
