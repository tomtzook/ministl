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
        include/span.h
        include/array.h
        include/intrinsin.h

        src/empty.cpp
        src/crt.cpp
        src/heap.cpp
        src/memory.cpp
        src/intrinsin.cpp
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

set(COMPILE_DEFS )

if (DEFINED MINISTL_DEFINE_TYPES)
    set(COMPILE_DEPS ${COMPILE_DEPS} _ministl_define_types_=1)
elseif (DEFINED MINISTL_TYPES_FROM_STD)
    set(COMPILE_DEPS ${COMPILE_DEPS} _ministl_define_types_=2)
endif ()

if (DEFINED MINISTL_SIZET_SIZE)
    set(COMPILE_DEPS ${COMPILE_DEPS} _ministl_size_t_type_=${MINISTL_SIZET_SIZE})
endif ()

if (DEFINED MINISTL_DEFINE_CRT)
    set(COMPILE_DEPS ${COMPILE_DEPS} _ministl_define_crt_=1)
endif ()

if (DEFINED MINISTL_DEFINE_INTRINSIC)
    set(COMPILE_DEPS ${COMPILE_DEPS} _ministl_define_intrinsic_=1)
endif ()

target_compile_definitions(ministl PUBLIC ${COMPILE_DEPS})

if (DEFINED MINISTL_UNIT_TESTS)
    find_package(GTest REQUIRED)
    enable_testing()

    add_executable(tests
            tests/mock_heap.cpp
            tests/mock_debug.cpp
            tests/mock_terminate.cpp
            tests/test_optional.cpp
            tests/test_result.cpp
            tests/test_buffer.cpp
            tests/expect.h)
    target_link_libraries(tests PRIVATE GTest::gtest_main ministl)

    #gtest_discover_tests(tests)
endif ()
