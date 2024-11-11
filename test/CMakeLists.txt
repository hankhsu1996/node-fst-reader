# Set minimum CMake version
cmake_minimum_required(VERSION 3.10)
project(fst_reader_test)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find ZLIB
find_package(ZLIB REQUIRED)

# Add directories for includes
include_directories(${CMAKE_SOURCE_DIR}/../src)
include_directories(${CMAKE_SOURCE_DIR}/../external/libfst)

# Link the directory where libfstapi.a is built
link_directories(${CMAKE_SOURCE_DIR}/../external/libfst/build)

# Add the test executable and specify the sources
add_executable(fst_reader_test FstFileTest.cpp)

# Link the test executable with libfstapi and zlib
target_link_libraries(fst_reader_test fstapi ZLIB::ZLIB)
