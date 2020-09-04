// This file is part of Tempest-core project
// Author: Karol Kontny

#pragma once

// Detect platform
#ifdef _WIN32
#define TST_PLATFORM PLATFORM_WINDOWS
#elif defined(__linux)
#define TST_PLATFORM PLATFORM_LINUX
#elif defined(__unix)
#define TST_PLATFORM PLATFORM_UNIX
#else
#define TST_PLATFORM PLATFORM_UNKNOWN
#endif //

// Compiler

#define TST_COMPILER_VC 0x00010000
#define TST_COMPILER_VC15 0x00010001
#define TST_COMPILER_VC15_3 0x00010002
#define TST_COMPILER_VC15_5 0x00010003
#define TST_COMPILER_VC15_6 0x00010004
#define TST_COMPILER_VC15_7 0x00010005
#define TST_COMPILER_VC15_8 0x00010006
#define TST_COMPILER_VC15_9 0x00010007

#define TST_COMPILER_VC16 0x00010010
#define TST_COMPILER_VC16_1 0x00010011
#define TST_COMPILER_VC16_2 0x00010012
#define TST_COMPILER_VC16_3 0x00010013
#define TST_COMPILER_VC16_4 0x00010014
#define TST_COMPILER_VC16_5 0x00010015
#define TST_COMPILER_VC16_6 0x00010016
#define TST_COMPILER_VC16_7 0x00010017

#define TST_COMPILER_GCC 0x00020000
#define TST_COMPILER_GCC7 0x00020001
#define TST_COMPILER_GCC8 0x00020002
#define TST_COMPILER_GCC9 0x00020003
#define TST_COMPILER_GCC10 0x00020004

#define TST_COMPILER_CLANG 0x00040000
#define TST_COMPILER_CLANG5 0x00040001
#define TST_COMPILER_CLANG6 0x00040002
#define TST_COMPILER_CLANG7 0x00040003
#define TST_COMPILER_CLANG8 0x00040004
#define TST_COMPILER_CLANG9 0x00040005
#define TST_COMPILER_CLANG10 0x00040006

// Detect compiler
#ifdef _MSC_VER
#if _MSC_VER == 1910
#define TST_COMPILER TST_COMPILER_VC15
#elif _MSC_VER == 1911
#define TST_COMPILER TST_COMPILER_VC15_3
#elif _MSC_VER == 1912
#define TST_COMPILER TST_COMPILER_VC15_5
#elif _MSC_VER == 1913
#define TST_COMPILER TST_COMPILER_VC15_6
#elif _MSC_VER == 1914
#define TST_COMPILER TST_COMPILER_VC15_7
#elif _MSC_VER == 1915
#define TST_COMPILER TST_COMPILER_VC15_8
#elif _MSC_VER == 1916
#define TST_COMPILER TST_COMPILER_VC15_9
#elif _MSC_VER == 1920
#define TST_COMPILER TST_COMPILER_VC16
#elif _MSC_VER == 1921
#define TST_COMPILER TST_COMPILER_VC16_1
#elif _MSC_VER == 1922
#define TST_COMPILER TST_COMPILER_VC16_2
#elif _MSC_VER == 1923
#define TST_COMPILER TST_COMPILER_VC16_3
#elif _MSC_VER == 1924
#define TST_COMPILER TST_COMPILER_VC16_4
#elif _MSC_VER == 1925
#define TST_COMPILER TST_COMPILER_VC16_5
#elif _MSC_VER == 1926
#define TST_COMPILER TST_COMPILER_VC16_6
#elif _MSC_VER >= 1927
#define TST_COMPILER TST_COMPILER_VC16_7
#endif //_MSC_VER

#elif defined(__clang__) // This one had to be before GCC because Clang tries to define GCC macros (Google monkeys)
#if __clang_major__ == 5
#define TST_COMPILER TST_COMPILER_CLANG5
#elif __clang_major__ == 6
#define TST_COMPILER TST_COMPILER_CLANG6
#elif __clang_major__ == 7
#define TST_COMPILER TST_COMPILER_CLANG7
#elif __clang_major__ == 8
#define TST_COMPILER TST_COMPILER_CLANG8
#elif __clang_major__ == 9
#define TST_COMPILER TST_COMPILER_CLANG9
#elif __clang_major__ == 10
#define TST_COMPILER TST_COMPILER_CLANG10
#else
// Unknown Clang version
#define TST_COMPILER TST_COMPILER_CLANG
#endif

#elif defined(__GNUC__)
#if __GNUC__ == 7
#define TST_COMPILER TST_COMPILER_GCC7
#elif __GNUC__ == 8
#define TST_COMPILER TST_COMPILER_GCC8
#elif __GNUC__ == 9
#define TST_COMPILER TST_COMPILER_GCC9
#elif __GNUC__ >= 10
#define TST_COMPILER TST_COMPILER_GCC10
#endif

#else
#pragma message("Unknown compiler")
#define TST_COMPILER TST_COMPILER_UNKNOWN
#endif

#if TST_COMPILER & TST_COMPILER_VC
#define TST_INLINE __forceinline
#define TST_NEVER_INLINE __declspec((noinline))
#define TST_CALL __vectorcall
#elif TST_COMPILER & TST_COMPILER_GCC
#define TST_INLINE inline __attribute__((__always_inline__))
#define TST_NEVER_INLINE __attribute__((__noinline__))
#define TST_FUNC_SPEC TST_CONSTEXPR
#ifdef TST_BUILD_32
#define TST_CALL __attribute__((fastcall)) // GCC probably don't have vectorcall
#else
#define TST_CALL
#endif
#elif TST_COMPILER & TST_COMPILER_CLANG
#define TST_INLINE inline __attribute__((__always_inline__))
#define TST_NEVER_INLINE __attribute__((__noinline__))
#define TST_FUNC_SPEC TST_CONSTEXPR
#define TST_CALL
#else
#define TST_INLINE inline
#define TST_NEVER_INLINE
#define TST_FUNC_SPEC TST_INLINE
#define TST_CALL
#endif
