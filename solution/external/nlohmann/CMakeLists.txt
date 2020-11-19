# ./external/nlohmann/CMakeLists.txt -- Traditional CMake

cmake_minimum_required( VERSION 2.8.10 )

project( External.json )
set( VERSION 1.0 )
set( DESCRIPTION "Json header-only lib." )

# Provide target for "json" library.
add_library( json UNKNOWN
    IMPORTED GLOBAL )
# Store include search-path containing headers
# of "json".
set_target_properties( json PROPERTIES
    MY_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} )
# Require at least compiling with C++14.
#set_target_properties( clipp PROPERTIES
#    MY_COMPILE_FEATURES "-std=c++1z" )
