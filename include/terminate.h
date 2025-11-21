#pragma once

#include "debug.h"

namespace framework {

[[noreturn]] void terminate();

}

#define abort(_msg) \
    do {                             \
        trace_error("abort: " _msg); \
        framework::terminate();      \
    } while (false);
