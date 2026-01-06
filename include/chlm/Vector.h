//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySofty. All rights reserved.
//

#pragma once

#include "Core.h"

namespace chlm {

    // ========================================
    // Core vector functions
    // ========================================
    inline float dot(float2 a, float2 b) noexcept { return (a * b).x + (a * b).y; }
    inline float dot(float3 a, float3 b) noexcept { return (a * b).x + (a * b).y + (a * b).z; }
    inline float dot(float4 a, float4 b) noexcept { return (a * b).x + (a * b).y + (a * b).z + (a * b).w; }

    inline float length_squared(float2 v) noexcept { return dot(v, v); }
    inline float length_squared(float3 v) noexcept { return dot(v, v); }
    inline float length_squared(float4 v) noexcept { return dot(v, v); }

    inline float length(float2 v) noexcept { return std::sqrt(length_squared(v)); }
    inline float length(float3 v) noexcept { return std::sqrt(length_squared(v)); }
    inline float length(float4 v) noexcept { return std::sqrt(length_squared(v)); }

    inline float2 normalize(float2 v) noexcept {
        float len = length(v);
        return almost_equal(len, 0.0f) ? float2{0.0f, 0.0f} : v * (1.0f / len);
    }

    inline float3 normalize(float3 v) noexcept {
        float len = length(v);
        return almost_equal(len, 0.0f) ? float3{0.0f, 0.0f, 0.0f} : v * (1.0f / len);
    }

    inline float4 normalize(float4 v) noexcept {
        float len = length(v);
        return almost_equal(len, 0.0f) ? float4{0.0f, 0.0f, 0.0f, 0.0f} : v * (1.0f / len);
    }

    inline float3 cross(float3 a, float3 b) noexcept {
        return a.yzx * b.zxy - a.zxy * b.yzx;
    }

    inline float2 lerp(float2 a, float2 b, float t) noexcept { return a + (b - a) * t; }
    inline float3 lerp(float3 a, float3 b, float t) noexcept { return a + (b - a) * t; }
    inline float4 lerp(float4 a, float4 b, float t) noexcept { return a + (b - a) * t; }

} // namespace chlm