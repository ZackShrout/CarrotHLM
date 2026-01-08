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

    /**
     * @brief Computes the dot (scalar) product of two vectors.
     *
     * @param a First vector.
     * @param b Second vector.
     * @return The dot product a · b.
     */
    constexpr float dot(const float2 a, const float2 b) noexcept { return (a * b).x + (a * b).y; }

    /**
     * @brief Computes the dot (scalar) product of two vectors.
     *
     * @param a First vector.
     * @param b Second vector.
     * @return The dot product a · b.
     */
    constexpr float dot(const float3 a, const float3 b) noexcept { return (a * b).x + (a * b).y + (a * b).z; }

    /**
     * @brief Computes the dot (scalar) product of two vectors.
     *
     * @param a First vector.
     * @param b Second vector.
     * @return The dot product a · b.
     */
    constexpr float dot(const float4 a, const float4 b) noexcept { return (a * b).x + (a * b).y + (a * b).z + (a * b).w; }

    /**
     * @brief Computes the squared length (magnitude) of a vector.
     *
     * Equivalent to dot(v, v) but often preferred when only comparing lengths
     * or avoiding a costly square root.
     *
     * @param v The vector.
     * @return The squared length ||v||².
     */
    constexpr float length_squared(const float2 v) noexcept { return dot(v, v); }

    /**
     * @brief Computes the squared length (magnitude) of a vector.
     *
     * Equivalent to dot(v, v) but often preferred when only comparing lengths
     * or avoiding a costly square root.
     *
     * @param v The vector.
     * @return The squared length ||v||².
     */
    constexpr float length_squared(const float3 v) noexcept { return dot(v, v); }

    /**
     * @brief Computes the squared length (magnitude) of a vector.
     *
     * Equivalent to dot(v, v) but often preferred when only comparing lengths
     * or avoiding a costly square root.
     *
     * @param v The vector.
     * @return The squared length ||v||².
     */
    constexpr float length_squared(const float4 v) noexcept { return dot(v, v); }

    /**
     * Computes the length (magnitude) of a vector.
     *
     * @param v The vector.
     * @return The length ||v||.
     */
    constexpr float length(const float2 v) noexcept { return std::sqrt(length_squared(v)); }

    /**
     * Computes the length (magnitude) of a vector.
     *
     * @param v The vector.
     * @return The length ||v||.
     */
    constexpr float length(const float3 v) noexcept { return std::sqrt(length_squared(v)); }

    /**
     * Computes the length (magnitude) of a vector.
     *
     * @param v The vector.
     * @return The length ||v||.
     */
    constexpr float length(const float4 v) noexcept { return std::sqrt(length_squared(v)); }

    /**
     * @brief Normalizes a vector to unit length.
     *
     * If the vector is zero-length, returns a zero vector to avoid division by zero.
     *
     * @param v The vector to normalize.
     * @return The normalized vector (length 1) or zero vector if input was zero.
     */
    constexpr float2 normalize(const float2 v) noexcept
    {
        const float len{ length(v) };
        return !almost_equal(len, .0f) ? v * (1.f / len) : float2{ .0f, .0f };
    }

    /**
     * @brief Normalizes a vector to unit length.
     *
     * If the vector is zero-length, returns a zero vector to avoid division by zero.
     *
     * @param v The vector to normalize.
     * @return The normalized vector (length 1) or zero vector if input was zero.
     */
    constexpr float3 normalize(const float3 v) noexcept
    {
        const float len{ length(v) };
        return !almost_equal(len, .0f) ? v * (1.f / len) : float3{ .0f, .0f, .0f };
    }

    /**
     * @brief Normalizes a vector to unit length.
     *
     * If the vector is zero-length, returns a zero vector to avoid division by zero.
     *
     * @param v The vector to normalize.
     * @return The normalized vector (length 1) or zero vector if input was zero.
     */
    constexpr float4 normalize(const float4 v) noexcept
    {
        const float len{ length(v) };
        return !almost_equal(len, .0f) ? v * (1.f / len) : float4{ .0f, .0f, .0f, .0f };
    }

    /**
     * @brief Computes the cross product of two 3D vectors.
     *
     * The result is a vector perpendicular to both inputs.
     * The direction follows the right-hand rule.
     *
     * @param a First vector.
     * @param b Second vector.
     * @return The cross product a × b.
     */
    constexpr float3 cross(float3 a, float3 b) noexcept
    {
        return a.yzx * b.zxy - a.zxy * b.yzx;
    }

    /**
     * @brief Linearly interpolates between two values.
     *
     * When t = 0, returns a. When t = 1, returns b.
     * Values outside [0, 1] perform extrapolation.
     *
     * @param a Start value.
     * @param b End value.
     * @param t Interpolation factor.
     * @return Interpolated value: a + t*(b - a).
     */
    constexpr float2 lerp(const float2 a, const float2 b, const float t) noexcept { return a + (b - a) * t; }

    /**
     * @brief Linearly interpolates between two values.
     *
     * When t = 0, returns a. When t = 1, returns b.
     * Values outside [0, 1] perform extrapolation.
     *
     * @param a Start value.
     * @param b End value.
     * @param t Interpolation factor.
     * @return Interpolated value: a + t*(b - a).
     */
    constexpr float3 lerp(const float3 a, const float3 b, const float t) noexcept { return a + (b - a) * t; }

    /**
     * @brief Linearly interpolates between two values.
     *
     * When t = 0, returns a. When t = 1, returns b.
     * Values outside [0, 1] perform extrapolation.
     *
     * @param a Start value.
     * @param b End value.
     * @param t Interpolation factor.
     * @return Interpolated value: a + t*(b - a).
     */
    constexpr float4 lerp(const float4 a, const float4 b, const float t) noexcept { return a + (b - a) * t; }
} // namespace chlm
