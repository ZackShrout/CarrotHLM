//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include <cstdint>
#include <cmath>
#include <cassert>

namespace chlm {
    // ========================================
    // Vector types using Clang's ext_vector_type
    // ========================================
    using float2 = float __attribute__((ext_vector_type(2)));
    using float3 = float __attribute__((ext_vector_type(3)));
    using float4 = float __attribute__((ext_vector_type(4)));

    using int2 = int __attribute__((ext_vector_type(2)));
    using int3 = int __attribute__((ext_vector_type(3)));
    using int4 = int __attribute__((ext_vector_type(4)));

    using uint2 = unsigned int __attribute__((ext_vector_type(2)));
    using uint3 = unsigned int __attribute__((ext_vector_type(3)));
    using uint4 = unsigned int __attribute__((ext_vector_type(4)));

    // ========================================
    // Unit vectors
    // ========================================
    constexpr float3 right()    { return { 1.f, 0.f, 0.f }; }
    constexpr float3 left()     { return { -1.f, 0.f, 0.f }; }
    constexpr float3 up()       { return { 0.f, 1.f, 0.f }; }
    constexpr float3 down()     { return { 0.f, -1.f, 0.f }; }
    constexpr float3 forward()  { return { 0.f, 0.f, 1.f }; } // +Z (DirectX style)
    constexpr float3 back()     { return { 0.f, 0.f, -1.f }; }

    // ========================================
    // Constants
    // ========================================
    constexpr float pi{ 3.1415926535897932384626433832795f };
    constexpr float pi_half{ pi * .5f };
    constexpr float pi_2{ pi * 2.f };
    constexpr float deg_to_rad{ pi / 180.f };
    constexpr float rad_to_deg{ 180.f / pi };
    constexpr float epsilon{ 1e-6f };

    constexpr float4 float4_zero{ 0.f, 0.f, 0.f, 0.f };
    constexpr float4 float4_one{ 1.f, 1.f, 1.f, 1.f };
    constexpr float3 float3_zero{ 0.f, 0.f, 0.f };
    constexpr float3 float3_one{ 1.f, 1.f, 1.f };

    // ========================================
    // Helper functions
    // ========================================
    constexpr bool almost_equal(const float a, const float b, const float eps = epsilon) noexcept
    {
        return std::abs(a - b) <= eps;
    }

    inline float clamp(const float v, const float lo, const float hi) noexcept
    {
        return v < lo ? lo : (v > hi ? hi : v);
    }
} // namespace chlm
