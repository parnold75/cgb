#ifndef __FILE_OPEN_HPP__
#define __FILE_OPEN_HPP__

#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace cgb
{

class file_handle
{
    private:
    int handle = -1;

    public:
    file_handle(const char * name, int oflag);
    ~file_handle();
    int get_handle(){ return handle; }
};

}

#endif // __FILE_OPEN_HPP__
