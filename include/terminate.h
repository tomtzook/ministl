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

// todo: add stack unwinder
#define catastrophic_error(_msg) abort(_msg)
