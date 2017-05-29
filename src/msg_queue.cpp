#include "msg_queue.hpp"
#include <iostream>
#include <sstream>

using namespace std;

namespace cgb
{

msg_queue::msg_queue(msg_queue_event callback, std::string name ) :
    _f_callback(callback),
    _name(name)

{
    _thread = thread(&msg_queue::worker, this);
}

msg_queue::~msg_queue()
{
    _thread.join();
    
    stringstream ss;
    ss << "~msg_thread '" << _name << "' stoppped." << endl;
    cout << ss.str();
}

void msg_queue::worker()
{
    cout << "msg_queue thread '" << _name << "' stopped" << endl;
}

void msg_queue::enqueue( const cgb_byte* buffer, cgb_size size )
{
    unique_lock<mutex> lock(_mutex);
    _f_callback(buffer, size);
}

} // namespace cgb
