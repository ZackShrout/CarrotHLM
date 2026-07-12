//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "../CompilerConfig.h"
#include "../PlatformConfig.h"
#include "SimdScalar.h"

#if CHLM_ARCH_X86_FAMILY && CHLM_HAS_SSE2
    #include "SimdSse.h"
#endif

#if CHLM_ARCH_ARM64 && CHLM_HAS_NEON
    #include "SimdNeon.h"
#endif

namespace chlm::detail {
    using scalar_simd = simd_scalar;

#if defined(CHLM_FORCE_SCALAR) && CHLM_FORCE_SCALAR
    using active_simd = simd_scalar;
#elif defined(CHLM_FORCE_NEON) && CHLM_FORCE_NEON
    static_assert(CHLM_ARCH_ARM64, "CHLM_FORCE_NEON requires an ARM64 target.");
    static_assert(CHLM_HAS_NEON, "CHLM_FORCE_NEON requires NEON support for the current build target.");
    using active_simd = simd_neon;
#elif defined(CHLM_FORCE_SSE2) && CHLM_FORCE_SSE2
    static_assert(CHLM_ARCH_X86_FAMILY, "CHLM_FORCE_SSE2 requires an x86/x64 target.");
    static_assert(CHLM_HAS_SSE2, "CHLM_FORCE_SSE2 requires SSE2 support for the current build target.");
    using active_simd = simd_sse2;
#elif CHLM_ARCH_ARM64 && CHLM_HAS_NEON
    using active_simd = simd_neon;
#elif CHLM_ARCH_X86_FAMILY && CHLM_HAS_SSE2
    using active_simd = simd_sse2;
#else
    using active_simd = simd_scalar;
#endif

    inline constexpr simd_backend_kind active_simd_backend{ active_simd::kind };
} // namespace chlm::detail
