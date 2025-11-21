cmake_minimum_required(VERSION 3.28)
project(minstl
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

        src/heap.h

        src/empty.cpp
        src/crt.cpp
        src/heap.cpp
        src/memory.cpp

)

add_library(minstl STATIC ${SOURCES})
target_include_directories(minstl
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/minstl>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(minstl PRIVATE -nostdinc++ -fno-exceptions)
target_link_options(minstl PRIVATE -nostdlib)
