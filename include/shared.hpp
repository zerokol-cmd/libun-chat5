#pragma once
#include <vector>
using bytes = std::vector<uint8_t>;
#if defined(_WIN32)
#define __export __declspec(dllexport)
#elif defined(__GNUC__) &&                                                     \
    ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define __export __attribute__((visibility("default")))
#else
#define __export
#endif
