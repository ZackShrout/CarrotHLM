//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "detail/PublicFloatVectors.h"
#include "detail/PublicIntVectors.h"
#include "detail/PublicUintVectors.h"
#include "detail/simd/SimdSelect.h"

#include <cstdint>
#include <cassert>

namespace chlm {
    // ========================================
    // Unit vectors
    // ========================================

    /**
     * @brief Positive X-axis unit vector (right).
     * @return {1, 0, 0}
     */
    constexpr float3 right() { return { 1.f, 0.f, 0.f }; }

    /**
     * @brief Negative X-axis unit vector (left).
     * @return {-1, 0, 0}
     */
    constexpr float3 left() { return { -1.f, 0.f, 0.f }; }

    /**
     * @brief Positive Y-axis unit vector (up).
     * @return {0, 1, 0}
     */
    constexpr float3 up() { return { 0.f, 1.f, 0.f }; }

    /**
     * @brief Negative Y-axis unit vector (down).
     * @return {0, -1, 0}
     */
    constexpr float3 down() { return { 0.f, -1.f, 0.f }; }

    /**
     * @brief Positive Z-axis unit vector (forward).
     *
     * Follows DirectX (Left-handed) convention (+Z into the screen).
     *
     * @return {0, 0, 1}
     */
    constexpr float3 forward() { return { 0.f, 0.f, 1.f }; }

    /**
     * @brief Negative Z-axis unit vector (back).
     * @return {0, 0, -1}
     */
    constexpr float3 back() { return { 0.f, 0.f, -1.f }; }

    // ========================================
    // Constants
    // ========================================
    /**
     * @brief Ratio of a circle's circumference to its diameter.
     */
    constexpr float pi{ 3.1415926535897932384626433832795f };

    /**
     * @brief One half of pi, commonly used for quarter turns.
     */
    constexpr float half_pi{ pi * .5f };

    /**
     * @brief Two times pi, commonly used for full rotations.
     */
    constexpr float two_pi{ pi * 2.f };

    /**
     * @brief Conversion factor from degrees to radians.
     */
    constexpr float deg_to_rad{ pi / 180.f };

    /**
     * @brief Conversion factor from radians to degrees.
     */
    constexpr float rad_to_deg{ 180.f / pi };

    /**
     * @brief Default epsilon used for approximate floating-point comparisons.
     */
    constexpr float epsilon{ 1e-6f };

    /**
     * @brief All-zero 4D float vector.
     */
    inline const float4 float4_zero{ 0.f, 0.f, 0.f, 0.f };

    /**
     * @brief All-one 4D float vector.
     */
    inline const float4 float4_one{ 1.f, 1.f, 1.f, 1.f };

    /**
     * @brief All-zero 3D float vector.
     */
    constexpr float3 float3_zero{ 0.f, 0.f, 0.f };

    /**
     * @brief All-one 3D float vector.
     */
    constexpr float3 float3_one{ 1.f, 1.f, 1.f };

    // ========================================
    // Helper functions
    // ========================================

    /**
     * @brief Returns the absolute value.
     *
     * @tparam T Arithmetic type supporting comparison with zero and unary negation.
     * @param v Input value.
     * @return Absolute value of @p v.
     */
    template<typename T>
    [[nodiscard]] constexpr T abs(const T v) noexcept
    {
        return (v < T{ }) ? -v : v;
    }

    /**
     * @brief Checks if two floating-point values are approximately equal.
     *
     * Uses absolute difference with a customizable tolerance.
     *
     * @param a First value.
     * @param b Second value.
     * @param eps Tolerance (defaults to chlm::epsilon = 1e-6f).
     * @return true if |a - b| <= eps.
     */
    [[nodiscard]] constexpr bool almost_equal(const float a, const float b, const float eps = epsilon) noexcept
    {
        return abs(a - b) <= eps;
    }

    /**
     * @brief Returns the smaller of two values.
     *
     * @tparam T Comparable type.
     * @param a First value.
     * @param b Second value.
     * @return Reference to the smaller value.
     */
    template<typename T>
    [[nodiscard]] constexpr const T& min(const T& a, const T& b) noexcept
    {
        return b < a ? b : a;
    }

    /**
     * @brief Returns the larger of two values.
     *
     * @tparam T Comparable type.
     * @param a First value.
     * @param b Second value.
     * @return Reference to the larger value.
     */
    template<typename T>
    [[nodiscard]] constexpr const T& max(const T& a, const T& b) noexcept
    {
        return a < b ? b : a;
    }

    /**
     * @brief Clamps a value to the inclusive range [lo, hi].
     *
     * @tparam T Comparable type.
     * @param v Value to clamp.
     * @param lo Lower bound.
     * @param hi Upper bound.
     * @return lo if v < lo, hi if v > hi, otherwise v.
     */
    template<typename T>
    [[nodiscard]] constexpr T clamp(const T v, const T lo, const T hi) noexcept
    {
        return (v < lo) ? lo : ((v > hi) ? hi : v);
    }

    /**
     * @brief Clamps a floating-point value to the inclusive range [0, 1].
     *
     * Commonly used for normalized values such as interpolation factors,
     * blend weights, and color channels.
     *
     * @param v Value to clamp.
     * @return Clamped value in the range [0, 1].
     */
    [[nodiscard]] constexpr float saturate(const float v) noexcept
    {
        return clamp(v, 0.0f, 1.0f);
    }

    /**
     * @brief Linearly interpolates between two values.
     *
     * When @p t is 0, returns @p a. When @p t is 1, returns @p b.
     * Values outside [0, 1] will extrapolate.
     *
     * @param a Start value.
     * @param b End value.
     * @param t Interpolation factor.
     * @return Interpolated value.
     */
    [[nodiscard]] constexpr float lerp(const float a, const float b, const float t) noexcept
    {
        return a + (b - a) * t;
    }

    /**
     * @brief Converts degrees to radians.
     *
     * @param degrees Angle in degrees.
     * @return Angle in radians.
     */
    [[nodiscard]] constexpr float to_radians(const float degrees) noexcept
    {
        return degrees * deg_to_rad;
    }

    /**
     * @brief Converts radians to degrees.
     *
     * @param radians Angle in radians.
     * @return Angle in degrees.
     */
    [[nodiscard]] constexpr float to_degrees(const float radians) noexcept
    {
        return radians * rad_to_deg;
    }

} // namespace chlm
