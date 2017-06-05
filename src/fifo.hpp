#ifndef __FIFO_HPP__
#define __FIFO_HPP__

#include <boost/pool/object_pool.hpp>

#include <string>
#include <thread>
#include <memory>
#include <functional>

#include "cgb_buffer.hpp"
#include "msg_queue.hpp"

using namespace std;

namespace cgb
{

/*
    Erstellt ein FIFO-Device und öffnet es per READONLY bzw. WRITEONLY
*/
class fifo
{
    private:
    boost::object_pool< cgb_buffer > pool;
    int not_exit = 1;
    string name;
    msg_queue &msg;
    thread worker;

    public:
    static fifo * reader(int address, msg_queue& msg);
    static fifo * writer(int address, msg_queue& msg);

    private:
    fifo(int address, string name, msg_queue &msg);
    
    public:
    ~fifo();


    private:
    unique_cgb_buffer get_buffer()
    {
        auto deleter =  [&](cgb_buffer * o)
        { 
            pool.destroy(o); 
        };

        return std::move( unique_cgb_buffer( 
            pool.malloc(), deleter ) );
    }

    bool create( );
    void reader_thread();
    void writer_thread();
};

using unique_fifo = std::unique_ptr< fifo >;

}

#endif // --FIFO_HPP_
