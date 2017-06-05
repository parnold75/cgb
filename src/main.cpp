#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <random>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "cgb.h"
#include "serial_io.hpp"
#include "fifo.hpp"
#include "args.hpp"
#include "msg_queue.hpp"

using namespace std::chrono;
using namespace std;

/**
* @brief Helper. Gibt eine CGB Nachricht aus.
*
* @param buffer
* @param n
*/
void print( cgb_byte* buffer, cgb_size n)
{
    for(int i=0; i< n; i++)
    {
        printf("0x%X ", buffer[i]);
    }
}

/**
* @brief 
*/
void unit_test()
{
    const int max_num_of_bytes = 112;
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);

    cgb_byte src[max_num_of_bytes *2];
    cgb_byte dest[max_num_of_bytes *2];
    cgb_byte src_7Bit_buffer[max_num_of_bytes * 2];

    for(int num_of_bytes = 0; num_of_bytes < max_num_of_bytes; num_of_bytes++)
    {
        for(int index_byte = 0; index_byte < num_of_bytes; index_byte++)
        {
            src[index_byte] = (cgb_byte) 0xFF * dist(mt);
        }

        if(cgb::args::debug())
        {
            print(src, num_of_bytes);
        }

        cgb_size len = 0;
        cgb_encoding(src, src_7Bit_buffer, num_of_bytes, &len);

        if(cgb::args::debug())
        {
            print(src_7Bit_buffer, len);
            cout << endl;
        }

        cgb_decoding(src_7Bit_buffer, dest, len, &len);
        for(int index_byte = 0; index_byte < num_of_bytes; index_byte++)
        {
            if(src[index_byte] != dest[index_byte])
            {
                printf("invalid data. num_of_bytes: %d index: %d  0x%X/0x%X\n", num_of_bytes, index_byte, src[index_byte], dest[index_byte]);
                return;
            }
        }
    }
    printf("test ok.\n");
}

/**
* @brief 
*/
void unit_test1()
{
    cgb_byte * src = (cgb_byte*)"Hallo\0";

    cgb_byte encoding_buffer[128];
    cgb_byte dest[128];
    cgb_size len = 0;
    cgb_encoding(src, encoding_buffer, 6, &len);
    cgb_decoding(encoding_buffer, dest, len, &len);

    printf("len: %d '%s'\n", len, (char*)dest);
}


/**
* @brief 
*/
void all_unit_tests()
{
    struct timespec res;
    struct timespec start;
    struct timespec end;

    clock_getres( CLOCK_REALTIME, &res);
    clock_gettime( CLOCK_REALTIME, &start);

    unit_test();
    unit_test1();

    clock_gettime( CLOCK_REALTIME, &end);
    printf("nsec: %d\n", (int)(end.tv_nsec-start.tv_nsec));

    //unit_test1();

    std::cout << "press enter" << endl;
}

/**
* @brief 
*
* @param fifoAddresses
*/
void fifo_op()
{
    vector< int > address;
    if(cgb::args::address(address) == false)
    {
        std::cout << "Address missing." << std::endl;
        return;
    }

        // ==========================================
        // MSG - Queue's
        // ==========================================
        cgb::msg_queue to([](const cgb_byte * src, cgb_size size)
        {
            cgb_byte buf[256];
            cgb_size len;
            cgb_encoding(src, buf, size, &len);
            print(buf, len);

            cgb_byte dest[256];
            cgb_decoding(buf, dest, len, &len);
            print(dest, len);
        }, std::string("TO"));    

        cgb::msg_queue from([](const cgb_byte*, cgb_size size)
        {
            std::cout << "from buffer exe" << std::endl;
        }, std::string("FROM"));    
    
        // ==========================================
        // create FIFO's
        // ==========================================
        // unique_ptr deleter
        auto fifo_vector_del = [](std::vector< cgb::unique_fifo > * v)
        {
            for(size_t i = 0; i < v->size(); i++)
            {
                (*v)[i].reset();
            }
            v->clear();
        };

        // ==========================================
        // type def (using) soll die Lesbarkeit erhöhen.
        using unique_fifo_vector = std::unique_ptr< std::vector< cgb::unique_fifo >, decltype(fifo_vector_del) >;
        unique_fifo_vector fifos = unique_fifo_vector( new vector< cgb::unique_fifo >, fifo_vector_del );
    
        for( auto fa : address )
        {
            fifos->push_back(std::move(
                std::unique_ptr<cgb::fifo>(
                    cgb::fifo::reader(fa, to))));

            fifos->push_back(std::move(
                std::unique_ptr<cgb::fifo>(
                    cgb::fifo::writer(fa, from))));
        }

        std::getchar();
}

/**
* @brief 
*/
void send_op(std::string & s)
{
       
        vector< int > address;
        if(cgb::args::address(address) == false)
        {
            std::cout << "Address missing." << std::endl;
            return;
        }

        std::cout << "send: '" << s << "'" << std::endl;
        cgb_byte* src = (cgb_byte*) s.c_str();
        cgb_byte buffer[256];
        cgb_byte dest[256];

        cgb_size len = 0;
        cgb_encoding(src, buffer, s.length()+1, &len);
        
        if(cgb::args::debug())
        {
            std::cout << "encoding buffer: [";
            print(buffer, len);
            std::cout << "]" << std::endl;
        }
        cgb_decoding(buffer, dest, len, &len);
        std::cout << "decoding buffer: [" << (char*)dest << "]" << std::endl;
}

/**
* @brief 
*
* @param argc
* @param argv
*
* @return 
*/
int main(int argc, char** argv)
{
    cgb::args::init(argc, argv);

    if(cgb::args::show_help())
    {
        return 0;
    }

    // ==========================================
    // serial io
    string device_name;
    if(cgb::args::device(device_name) == false)
    {
        cout << "Missing device name." << endl;
        return 0;
    }

    cout << "Current device: " << device_name << endl;
    cgb::unique_serial_io serial(new cgb::serial_io(device_name.c_str()));



    //auto ms = milliseconds(3000);
    //std::this_thread::sleep_for(ms);

    // schreibt alle empfangenden daten auf std::out.
    if(cgb::args::listen())
    {
        serial->set_reader_callback( [&](const cgb_byte * buf, cgb_size size)
        {
        });
        
        return 0;
    }

    // ==========================================
    // Werden fifo's benötigt.
    if(cgb::args::fifo())
    {
        fifo_op();
    }
   
    string s;
    if(cgb::args::send(s))
    {
        send_op(s);
    }

    if(cgb::args::unit_test())
    {
        all_unit_tests();
    }
    return 0;
}
