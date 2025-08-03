cmake_minimum_required(VERSION 3.28.2)

include(../sdk/config.cmake)
project(sieelf_xtask4 C)

include_directories("../libsie/src/include")
link_libraries(crt crt_helper gcc c ft sie)
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(XTask4_ELKA ${SOURCES})
target_sdk_setup(XTask4_ELKA ELKA)
target_link_directories(XTask4_ELKA PUBLIC "../libsie/build/lib/ELKA")
target_compile_options(XTask4_ELKA BEFORE PUBLIC -std=c11 -Wall -Werror -DSWILIB_MODERN)
