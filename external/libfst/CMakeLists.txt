cmake_minimum_required(VERSION 3.0)
project(fstlib)

find_package(ZLIB REQUIRED)

add_library(fstapi STATIC
    fstapi.c
    fastlz.c
    lz4.c
)
target_include_directories(fstapi PRIVATE external/libfst)
target_link_libraries(fstapi PRIVATE ZLIB::ZLIB)
target_compile_definitions(fstapi PRIVATE FST_CONFIG_INCLUDE="fstapi.h")
