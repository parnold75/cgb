#ifndef __FIFO_HPP__
#define __FIFO_HPP__

#include <string>
#include <thread>
#include "msg_queue.hpp"

using namespace std;

namespace cgb
{

class fifo
{
    private:
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
    bool create( );
    void reader_thread();
    void writer_thread();
};

using unique_fifo = std::unique_ptr< fifo >;

}

#endif // --FIFO_HPP_
