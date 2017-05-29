#include "fifo.hpp"
#include "file_handle.hpp"

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
    cgb_byte bytes[110];
    msg.enqueue(bytes, (cgb_size) 0);

    // open the fifo.
    std::unique_ptr<file_handle> handle(new file_handle(name.c_str(), O_RDONLY|O_NONBLOCK));

    if(handle->get_handle() < 0)
    {
        std::cout << "Can't open fifo (reader) '" << name << "' errno: " << errno << std::endl;
        return;
    }

    int readed = 0;  
    while( (readed = read(handle->get_handle(), bytes, 110 ))>=0 && not_exit )
    {
        if(readed == 0)
        {
            continue;
        }
        msg.enqueue(bytes, (cgb_size) readed);
    }
    
    std::cout << "fifo thread stopped. '" << name << "'" << std::endl;
/*
    char buf[1024];

    while(1)
    {
        int readed = 0;
        memset(buf, 0, 1024);
        if((readed = read(fifo_handle, buf, 100)) != 0)
        {
            buf[--readed] = 0;
            std::cout << "readed: " << readed << " '" << buf << "'" << std::endl;
            
            cgb_byte buffer[256];
            cgb_byte dest[256];
            cgb_size len = 0;
            
            cgb_encoding((cgb_byte*)buf, buffer, readed, &len);
        
            if(cgb::args::debug())
            {
                std::cout << "Encoding buffer: " << (int)len << " [";
                print(buffer, len);
                std::cout << "]" << std::endl;
            }

            memset(dest, 0, 256);
            cgb_decoding(buffer, dest, len, &len);
            std::cout << "Decoding buffer: " << (int)len << " [" << (char*)dest << "]" << std::endl;
        }
    }
*/
}

/*
    worker
*/
void fifo::writer_thread()
{
    using namespace std::chrono;
    auto ms = milliseconds(100);

    while( not_exit )
    {
        std::unique_ptr<file_handle> handle(new file_handle(name.c_str(), O_WRONLY|O_NONBLOCK));
        if(handle->get_handle() < 0)
        {
            if(errno != ENXIO)
            {
                std::cout << "Can't open fifo (writer) '" << name << "' errno: " << errno << std::endl;
            }
            std::this_thread::sleep_for(ms);
            continue;
        }
        // fd ändern.
        //fcntl(file_handle->get_handle(), O_
    }
    std::cout << "fifo thread stopped. '" << name << "'" << std::endl;
}

} // namespace cgb
