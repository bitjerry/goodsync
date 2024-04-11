cmake_minimum_required(VERSION 3.20)
project(goodsync C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_VERBOSE_MAKEFILE ON)

#set(OPENSSL_USE_STATIC_LIBS TRUE)

find_package(OpenSSL COMPONENTS Crypto)

set(Python3_USE_STATIC_LIBS FALSE)

find_package(Python3 COMPONENTS Interpreter Development)

add_library(
        goodsync SHARED
        goodsync/goodsync_warp.c
        goodsync/goodsync.c
        goodsync/crc.c
        goodsync/base.c)

target_link_libraries(goodsync PUBLIC Python3::Module OpenSSL::Crypto)
set_target_properties(goodsync PROPERTIES OUTPUT_NAME "_GoodSync" PREFIX "")

# If other tools report errors, just comment it, feel free
if ($CMAKE_STRIP)
    set(CMAKE_STRIP "strip")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-all "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Strip debug symbols done on final binary.")
endif ()
