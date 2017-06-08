#include "fifo.hpp"
#include "file_handle.hpp"

#include <boost/pool/object_pool.hpp>
#include <chrono>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

using namespace std;
using namespace std::chrono;

namespace cgb
{

/**
* @brief static
*
* @param address
* @param msg
*
* @return 
*/
fifo * fifo::reader(int address, msg_queue & msg)
{
    stringstream ss;
    ss << "cgbTO." << address;

    fifo * f = new fifo(address, ss.str(), msg);

    if(f->create())
    {
        f->worker = std::thread(&fifo::reader_thread, f);
    }
    return f;
}

/**
* @brief static
*
* @param address
* @param msg
*
* @return 
*/
fifo * fifo::writer(int address, msg_queue & msg)
{
    std::stringstream ss;
    ss << "cgbFROM." << address;

    fifo * f = new fifo(address, ss.str(), msg);

    if(f->create())
    {
        f->worker = std::thread(&fifo::writer_thread, f);
    }
    return f;
}

/*
    create and open (readonly) the fifo.
*/
fifo::fifo(int address, std::string name, msg_queue & msg) :
    name(name),
    msg(msg)
{
}

/*
*/
fifo::~fifo()
{
    not_exit = 0;
    worker.join();

    unlink(name.c_str());
    std::cout << "~fifo " << name << std::endl;
}

/*
    create new fifo
*/
bool fifo::create()
{
    unlink(name.c_str());
    auto result = mkfifo(name.c_str(), 0666);
    if(result < 0)
    {
        std::cout << "Can't create fifo '" << name << "' errno: " << errno << std::endl;
    }
    return result >= 0;
}

/*
    worker
*/
void fifo::reader_thread()
{
    auto ms = milliseconds(100);
    auto handle = std::make_unique<file_handle>( name.c_str(), O_RDONLY|O_NONBLOCK );
    if(handle->get_handle() < 0)
    {
        std::cout << "Can't open fifo (reader) '" << name << "' errno: " << errno << std::endl;
        return;
    }

    auto buffer = get_buffer();
    while( not_exit )
    {
        buffer->size = read( handle->get_handle(), buffer->buffer, buffer->MAX_SIZE );   
        
        if(buffer->size == 0)
        {
            std::this_thread::sleep_for(ms);
            continue;
        }

        msg.enqueue(std::move(buffer));
        buffer = get_buffer();
    }
}

/*
    worker
    Schreibt die Daten von den Clients in den Output-FIFO zurück.
*/
void fifo::writer_thread()
{
    auto ms = milliseconds(100);

    while( not_exit )
    {
        auto handle = make_unique< file_handle >( name.c_str(), O_WRONLY|O_NONBLOCK );
        if(handle->get_handle() < 0)
        {
            if(errno != ENXIO)
            {
                std::cout << "Can't open fifo (writer) '" << name << "' errno: " << errno << std::endl;
                std::this_thread::sleep_for(ms);
            }
            continue;
        }
    }
    std::cout << "fifo thread stopped. '" << name << "'" << std::endl;
}

} // namespace cgb
