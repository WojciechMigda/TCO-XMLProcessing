# ./external/dbg/CMakeLists.txt -- Traditional CMake

cmake_minimum_required( VERSION 2.8.10 )

project( External.dbg )
set( VERSION 1.0 )
set( DESCRIPTION "dbg header-only lib." )

# Provide target for "dbg" library.
add_library( dbg UNKNOWN
    IMPORTED GLOBAL )
# Store include search-path containing headers
# of "dbg".
set_target_properties( dbg PROPERTIES
    MY_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} )
# Require at least compiling with C++14.
#set_target_properties( clipp PROPERTIES
#    MY_COMPILE_FEATURES "-std=c++1z" )
