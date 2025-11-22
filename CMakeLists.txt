cmake_minimum_required(VERSION 3.28)
project(ministl
        LANGUAGES C CXX
        VERSION "1.0.0")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
enable_testing()

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
        include/span.h

        src/empty.cpp
        src/crt.cpp
        src/heap.cpp
        src/memory.cpp
)

add_library(ministl STATIC ${SOURCES})
target_include_directories(ministl
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/ministl>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(ministl PRIVATE -nostdinc++ -fno-exceptions)
target_link_options(ministl PRIVATE -nostdlib -nolibc -nodefaultlibs)

if (DEFINED NOSTD)
    target_compile_definitions(ministl PUBLIC __define_types__=1 __sizet_type__=2 __define_crt__=1 __define_intrinsic__=1)
endif ()

add_executable(tests
        tests/mock_heap.cpp
        tests/mock_debug.cpp
        tests/mock_terminate.cpp
        tests/test_optional.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main ministl)

#gtest_discover_tests(tests)
