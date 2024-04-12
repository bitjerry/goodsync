cmake_minimum_required(VERSION 3.24)
project(goodsync C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories($ENV{OPENSSL_INCLUDE_DIR})
link_directories($ENV{OPENSSL_LIBRARY_DIR})

add_executable(goodsync example.c goodsync.c crc.c base.c)

target_link_libraries(goodsync PRIVATE $ENV{OPENSSL_LIBRARIES})
