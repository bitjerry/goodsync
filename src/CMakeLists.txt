cmake_minimum_required(VERSION 3.24)
project(goodsync C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(OPENSSL_USE_STATIC_LIBS TRUE)

find_package(OpenSSL COMPONENTS Crypto)

add_executable(goodsync example.c goodsync.c crc.c base.c)

target_link_libraries(goodsync PRIVATE OpenSSL::Crypto)
