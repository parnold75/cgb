#include "file_handle.hpp"

namespace cgb
{
    /**
    * @brief 
    *
    * @param name
    * @param oflag
    */
    file_handle::file_handle(const char * name, int oflag)
    {
        handle = open(name, oflag);
    }

    /**
    * @brief 
    */
    file_handle::~file_handle()
    {
        if(handle >= 0)
        {
            close(handle);
            handle = -1;
            std::cout << "~file_handle" << std::endl;
        }
    }

} // namespace cgb
