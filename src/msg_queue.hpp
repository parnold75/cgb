#ifndef __MSG_QUEUE_HPP__
#define __MSG_QUEUE_HPP__

#include <mutex>
#include <functional>
#include <queue>
#include <thread>

#include "cgb.h"
#include <string>

using namespace std;

using msg_queue_event = std::function< void(const cgb_byte *, cgb_size)>;

namespace cgb
{

class msg_queue
{
    private:
    msg_queue_event _f_callback;
    string _name;    
    mutex _mutex;
    thread _thread;
    queue<cgb_byte*> _queue;

 
    public:
    msg_queue(msg_queue_event callback, string name);
    ~msg_queue();

    void enqueue(const cgb_byte* buffer, cgb_size size);

    private:
    void worker();
};

} // namespace cgb

#endif // __MSG_QUEUE_HPP__
