cmake_minimum_required (VERSION 3.0)
project (cgb)

add_definitions(-std=c++11)
add_definitions( -J2 )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -DBOOST_LOG_DYN_LINK -DBOOST_COROUTINES_NO_DEPRECATION_WARNING ")

#set(CMAKE_VERBOSE_MAKEFILE ON)

#add_subdirectory (cgb)
find_package(Boost)

file (GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.c")

add_executable (cgb ${SOURCE_FILES})

target_link_libraries(cgb pthread)
target_link_libraries(cgb 
    rt boost_system boost_coroutine boost_filesystem
    boost_regex boost_thread boost_context boost_chrono boost_log  
    boost_atomic boost_date_time boost_log_setup boost_program_options
) 

 
