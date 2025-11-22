#pragma once

#include <verify.h>

#define EXPECT_VERIFY(...) \
    ({                                  \
        auto __result = (__VA_ARGS__);  \
        if (!__result) {                \
            trace_result("error result", __result); \
            EXPECT_FALSE(true); \
        }                               \
        __result.release_value();       \
    })
