#include <iostream>
#include <vector>
#include <string>
#include "args.hpp"

using namespace std;

namespace cgb
{

/* static */
var_map args::vmap;
opt_desc args::general("General options");
opt_desc args::all("cgb - Chain-Gang-Bus - master");

/* static */
void args::init(int argc, char** argv)
{
    using namespace boost::program_options;
    using namespace std;

    string deviceDefault = "/dev/ttyAMA0";

    general.add_options()
        ("help", "Produce this help message.")
        ("send,s", value< string >(), "Send the message.")
        ("acknowledge", "Paket muss vom client bestätigt werden.")
        ("listen,l", "Horcht auf Nachrichten. [-a <address>]")
        ("ping,p", "Ping mode.")
        ("address,a", value< vector< int > >(), "Adressen für spezifizierte funktion.")
        ("device,d", value< string >()->default_value(deviceDefault), "Device")
        ("debug", "debug mode")
        ("fifo", "FIFO Address mapper.")
        ("unit-test", "run unit tests");

    all.add(general);

    store(parse_command_line(argc, argv, all), vmap);
}

/* static */
bool args::show_help()
{
    if(vmap.count("help"))
    {
        std::cout << all;
        return true;
    }
    return false;
}

/* static */
bool args::send(string & msg)
{
    if(vmap.count("send"))
    {
        msg = vmap["send"].as<string>();
        return true;
    }
    return false;
}

/* static */
bool args::acknowledge()
{
    return false;
}

/* static */
bool args::listen()
{
    return vmap.count("listen") > 0;
}    

/* static */
bool args::ping(string & address)
{
    return false;
}

/* static */
bool args::address(vector< int > & address)
{
    if(vmap.count("address") > 0)
    {
        auto addr = vmap["address"].as<vector<int>>();
        std::copy (addr.begin(), addr.end(), std::back_inserter(address));
        return true;
    }

    return false;
}

/* static */
bool args::device(string & device)
{
    if(vmap.count("device") > 0)
    {
        device = vmap["device"].as< string >();
        return true;
    }
    return false;
}

/* static */
bool args::debug()
{
    return vmap.count("debug") > 0;
}

/* static */
bool args::fifo()
{
    return vmap.count("fifo") > 0;
}

/* static */
bool args::unit_test()
{
    return vmap.count("unit-test") > 0;
}

} // namespace cgb


