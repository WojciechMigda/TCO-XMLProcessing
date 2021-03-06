set( CMAKE_CXX_STANDARD 17 )

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(APP main)
add_executable( ${APP} "src/dummy.cpp" )
target_sources( ${APP} PRIVATE
  "src/main.cpp"
  "src/work.cpp"
  "src/error_printer.cpp"
  "src/filesystem.cpp"
  "src/parser.cpp"
  "src/jsonize.cpp"
  "src/file_io.cpp"
)

include_directories( "src" )
include_directories(
  "$<TARGET_PROPERTY:clipp,MY_INCLUDE_DIRS>" )
include_directories(
  "$<TARGET_PROPERTY:json,MY_INCLUDE_DIRS>" )
include_directories(
  "$<TARGET_PROPERTY:fmt,MY_INCLUDE_DIRS>" )
include_directories(
  "$<TARGET_PROPERTY:spdlog,MY_INCLUDE_DIRS>" )

find_package( Boost 1.41.0 REQUIRED )
target_include_directories( ${APP} PUBLIC ${Boost_INCLUDE_DIRS})

find_package( Filesystem REQUIRED COMPONENTS Experimental )
target_link_libraries( ${APP} ${Tsetlini_LIBRARY} std::filesystem )

target_compile_definitions( ${APP} PRIVATE "$<TARGET_PROPERTY:fmt,INTERFACE_COMPILE_DEFINITIONS>" )
