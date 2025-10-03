#ifndef PLATFORM_H
#define PLATFORM_H

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#if defined(__GNUC__) || defined(__clang__)
    #define nodefault __builtin_unreachable();
#elif defined (_MSC_VER)
    #define nodefault __assume(0);
#else
    #define nodefault do { } while (0);
#endif

#endif