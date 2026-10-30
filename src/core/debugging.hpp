#pragma once

#include <raylib.h>

// platform detection for debug breaking
#if defined(_WIN64)
    #include <intrin.h>
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__linux__)
    #include <csignal>
    #define DEBUG_BREAK() raise(SIGTRAP)
#else
    #error "Platform doesn't support debugbreak!"
#endif

// logging
#ifdef ENABLE_DEBUGGING
    #define LOG_INFO(...)  TraceLog(LOG_INFO, __VA_ARGS__)
    #define LOG_WARN(...)  TraceLog(LOG_WARNING, __VA_ARGS__)
    #define LOG_ERROR(...) TraceLog(LOG_ERROR, __VA_ARGS__)
    #define LOG_FATAL(...) TraceLog(LOG_FATAL, __VA_ARGS__)
#else
	#define LOG_INFO(...)
	#define LOG_WARN(...)
	#define LOG_ERROR(...)
	#define LOG_FATAL(...)
#endif

// asserting
#ifdef ENABLE_DEBUGGING
    #define ASSERT(x, ...)                                  \
        do {                                                \
            if (!(x)) {                                     \
                LOG_ERROR("Assertion failed: %s. Message: " \
                               __VA_ARGS__                  \
                               "\nAt: %s, Line: %d",        \
                               #x, __FILE__, __LINE__);     \
                DEBUG_BREAK();                              \
            }                                               \
        } while (0)
#else
    #define ASSERT(...) 
#endif