cmake_minimum_required(VERSION 3.28)
project(ministl
        LANGUAGES C CXX
        VERSION "1.0.0")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

file(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)

set(SOURCES
        include/memory.h
        include/terminate.h
        include/status.h
        include/debug.h
        include/result.h
        include/optional.h
        include/unique_ptr.h
        include/buffer.h
        include/verify.h
        include/lock.h
        include/atomic.h

        src/empty.cpp
        src/crt.cpp
        src/heap.cpp
        src/memory.cpp
        include/span.h

)

add_library(ministl STATIC ${SOURCES})
target_include_directories(ministl
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/ministl>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(ministl PRIVATE -nostdinc++ -fno-exceptions)
target_compile_definitions(ministl PRIVATE __define_types__=1 __sizet_type__=1)
target_link_options(ministl PRIVATE -nostdlib -nolibc -nodefaultlibs)
