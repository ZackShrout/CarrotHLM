//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#if defined(_MSC_VER)
    #define CHLM_COMPILER_MSVC 1
#else
    #define CHLM_COMPILER_MSVC 0
#endif

#if defined(__clang__)
    #define CHLM_COMPILER_CLANG 1
#else
    #define CHLM_COMPILER_CLANG 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
    #define CHLM_COMPILER_GCC 1
#else
    #define CHLM_COMPILER_GCC 0
#endif
