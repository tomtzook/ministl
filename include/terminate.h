#pragma once

#include "debug.h"

namespace framework {

[[noreturn]] void terminate();

}

#define _abort(_msg) \
    do {                             \
        trace_error("abort: " _msg); \
        framework::terminate();      \
    } while (false);

#define catastrophic_error(_msg) _abort(_msg)
