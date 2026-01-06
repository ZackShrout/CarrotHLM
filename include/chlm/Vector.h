//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Core.h"

namespace chlm {
    // ========================================
    // Core vector functions
    // ========================================
    inline float dot(const float2 a, const float2 b) noexcept { return (a * b).x + (a * b).y; }
    inline float dot(const float3 a, const float3 b) noexcept { return (a * b).x + (a * b).y + (a * b).z; }
    inline float dot(const float4 a, const float4 b) noexcept { return (a * b).x + (a * b).y + (a * b).z + (a * b).w; }

    inline float length_squared(const float2 v) noexcept { return dot(v, v); }
    inline float length_squared(const float3 v) noexcept { return dot(v, v); }
    inline float length_squared(const float4 v) noexcept { return dot(v, v); }

    inline float length(const float2 v) noexcept { return std::sqrt(length_squared(v)); }
    inline float length(const float3 v) noexcept { return std::sqrt(length_squared(v)); }
    inline float length(const float4 v) noexcept { return std::sqrt(length_squared(v)); }

    inline float2 normalize(const float2 v) noexcept
    {
        const float len{ length(v) };
        return almost_equal(len, .0f) ? float2{ .0f, .0f } : v * (1.f / len);
    }

    inline float3 normalize(const float3 v) noexcept
    {
        const float len{ length(v) };
        return almost_equal(len, .0f) ? float3{ .0f, .0f, .0f } : v * (1.f / len);
    }

    inline float4 normalize(const float4 v) noexcept
    {
        const float len{ length(v) };
        return almost_equal(len, .0f) ? float4{ .0f, .0f, .0f, .0f } : v * (1.f / len);
    }

    inline float3 cross(float3 a, float3 b) noexcept
    {
        return a.yzx * b.zxy - a.zxy * b.yzx;
    }

    inline float2 lerp(const float2 a, const float2 b, const float t) noexcept { return a + (b - a) * t; }
    inline float3 lerp(const float3 a, const float3 b, const float t) noexcept { return a + (b - a) * t; }
    inline float4 lerp(const float4 a, const float4 b, const float t) noexcept { return a + (b - a) * t; }
} // namespace chlm
