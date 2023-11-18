#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <assert.h>

#define STATIC_ASSERT static_assert

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef _MSC_VER
#define PRETTY_FUNCTION __FUNCSIG__
#else
#define PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte(s).");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 byte(s).");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 byte(s).");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 byte(s).");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte(s).");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 byte(s).");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 byte(s).");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 byte(s).");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 byte(s).");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 byte(s).");

#if defined(__APPLE__)
    #define CORTEX_PLATFORM_APPLE 1
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define CORTEX_PLATFORM_MACOS 1
        // MacOS specific stuff
    #else
        #error "Cortex only supports MacOS Desktops at this time."
    #endif
#elif defined(__EMSCRIPTEN__)
    #define CORTEX_PLATFORM_WEB 1
#else
    #error "Cortex only supports Apple platforms at this time."
#endif

#ifndef CORTEX_PLATFORM_APPLE
    #define CORTEX_PLATFORM_APPLE 0
#endif

#ifndef CORTEX_PLATFORM_MACOS
    #define CORTEX_PLATFORM_MACOS 0
#endif

#ifndef CORTEX_PLATFORM_WEB
    #define CORTEX_PLATFORM_WEB 0
#endif

typedef enum LogLevel {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
} LogLevel;

typedef enum LogLevelBit {
    LOG_LEVEL_FATAL_BIT = (1 << LOG_LEVEL_FATAL),
    LOG_LEVEL_ERROR_BIT = (1 << LOG_LEVEL_ERROR),
    LOG_LEVEL_WARN_BIT = (1 << LOG_LEVEL_WARN),
    LOG_LEVEL_INFO_BIT = (1 << LOG_LEVEL_INFO),
    LOG_LEVEL_DEBUG_BIT = (1 << LOG_LEVEL_DEBUG),
    LOG_LEVEL_TRACE_BIT = (1 << LOG_LEVEL_TRACE),
} LogLevelBit;

inline void CoreLog(LogLevel verbosity, const char *msg, const char *file, i32 line, ...) {

    static const char *logLevelLabels[6] = {
        "[FATAL]",
        "[ERROR]",
        "[WARN]",
        "[INFO]",
        "[DEBUG]",
        "[TRACE]",
    };

#if CORTEX_PLATFORM_WEB
    static const char *logLevelColors[6] = {
        "",
        "",
        "",
        "",
        "",
        "",
    };
#else
    static const char *logLevelColors[6] = {
        "\e[0;31m",
        "\e[0;31m",
        "\e[0;33m",
        "\e[0;32m",
        "\e[0;36m",
        "\e[0;37m",
    };
#endif

    int buffer_size = 16000;
    char output[buffer_size];
    memset(output, 0, sizeof(output));

    __builtin_va_list p_args;
    va_start(p_args, line);
    vsnprintf(output, buffer_size, msg, p_args);
    va_end(p_args);
#if CORTEX_PLATFORM_WEB
    printf("%s%-7s: %s Line: %i\n%-9s%s%s\n", logLevelColors[verbosity], logLevelLabels[verbosity], file, line, "", output, "");
#else
    printf("%s%-7s: %s Line: %i\n%-9s%s%s\n", logLevelColors[verbosity], logLevelLabels[verbosity], file, line, "", output, "\e[0m");
#endif
}

#ifdef CORTEX_NO_LOGGING
    #define CX_FATAL(msg, ...)
    #define CX_ERROR(msg, ...)
    #define CX_WARN(msg, ...)
    #define CX_INFO(msg, ...)
    #define CX_DEBUG(msg, ...)
    #define CX_TRACE(msg, ...)
#else
    #define CX_FATAL(msg, ...) CoreLog(LOG_LEVEL_FATAL, msg, __FILE__, __LINE__, ##__VA_ARGS__)
    #define CX_ERROR(msg, ...) CoreLog(LOG_LEVEL_ERROR, msg, __FILE__, __LINE__, ##__VA_ARGS__)
    #define CX_WARN(msg, ...)  CoreLog(LOG_LEVEL_WARN, msg, __FILE__, __LINE__, ##__VA_ARGS__)
    #define CX_INFO(msg, ...)  CoreLog(LOG_LEVEL_INFO, msg, __FILE__, __LINE__, ##__VA_ARGS__)
    #define CX_DEBUG(msg, ...) CoreLog(LOG_LEVEL_DEBUG, msg, __FILE__, __LINE__, ##__VA_ARGS__)
    #define CX_TRACE(msg, ...) CoreLog(LOG_LEVEL_TRACE, msg, __FILE__, __LINE__, ##__VA_ARGS__)
#endif

inline void LogAssertionFailure(const char *expr, const char *file, i32 line, const char* msg) {
    CoreLog(LOG_LEVEL_FATAL, "Assertion failed for expression %s. %s", file, line, expr, msg);
}

#define DebugBreak() __builtin_trap()

#ifdef CORTEX_NO_ASSERTS
    #define CX_ASSERT(expr, msg)
    #define CX_DEBUGASSERT(expr, msg)
#else
    #define CX_ASSERT(expr, msg)                                            \
        {                                                                \
            if (expr)                                                    \
            {                                                            \
            }                                                            \
            else                                                         \
            {                                                            \
                LogAssertionFailure(#expr, __FILE__, __LINE__, #msg);    \
                DebugBreak();                                            \
            }                                                            \
        }

        #ifdef CORTEX_DEBUG
            #define CX_DEBUGASSERT(expr, msg)                                       \
                {                                                                \
                    if (expr)                                                    \
                    {                                                            \
                    }                                                            \
                    else                                                         \
                    {                                                            \
                        LogAssertionFailure(#expr, __FILE__, __LINE__, #msg);    \
                        DebugBreak();                                            \
                    }                                                            \
                }
        #else
            #define CX_DEBUGASSERT(expr, msg)
        #endif
#endif