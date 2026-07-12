//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#if defined(__x86_64__) || defined(_M_X64)
    #define CHLM_ARCH_X86_64 1
#else
    #define CHLM_ARCH_X86_64 0
#endif

#if defined(__i386__) || defined(_M_IX86)
    #define CHLM_ARCH_X86_32 1
#else
    #define CHLM_ARCH_X86_32 0
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
    #define CHLM_ARCH_ARM64 1
#else
    #define CHLM_ARCH_ARM64 0
#endif

#if CHLM_ARCH_X86_64 || CHLM_ARCH_X86_32
    #define CHLM_ARCH_X86_FAMILY 1
#else
    #define CHLM_ARCH_X86_FAMILY 0
#endif

#if CHLM_ARCH_ARM64
    #define CHLM_ARCH_ARM_FAMILY 1
#else
    #define CHLM_ARCH_ARM_FAMILY 0
#endif

#if defined(__SSE2__) || defined(_M_X64) || (_M_IX86_FP >= 2)
    #define CHLM_HAS_SSE2 1
#else
    #define CHLM_HAS_SSE2 0
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
    #define CHLM_HAS_NEON 1
#else
    #define CHLM_HAS_NEON 0
#endif
