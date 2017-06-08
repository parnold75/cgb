#ifndef __ARGS_HPP__
#define __ARGS_HPP__

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <vector>
#include <string>

using namespace std;

using var_map = boost::program_options::variables_map;
using opt_desc = boost::program_options::options_description;

namespace cgb
{

class args
{
    private:
    static var_map vmap;
    static opt_desc general;
    static opt_desc all;

    public:
    static void init(int argc, char** argv);

    static bool show_help();
    static bool send(string & msg);
    static bool acknowledge();
    static bool listen();
    static bool ping(string & address);
    static bool address(vector< int > & address);
    static bool device(string & device);
    static bool debug();
    static bool fifo();
    static bool unit_test();
}; // class args

} // namespace cgb

#endif // __ARGS_HPP__
