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
    constexpr float pi{ 3.1415926535897932384626433832795f };
    constexpr float half_pi{ pi * .5f };
    constexpr float two_pi{ pi * 2.f };
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

    // ========================================
    // Scalar math wrappers
    // ========================================
    // These currently forward to the standard library.
    // They exist so engine code can depend on chlm only,
    // allowing the implementation to be replaced later.

    /**
     * @brief Computes the sine of an angle in radians.
     *
     * Currently wraps the standard library implementation.
     *
     * @param x Angle in radians.
     * @return Sine of @p x.
     */
    [[nodiscard]] inline float sin(const float x) noexcept
    {
        return std::sin(x);
    }

    /**
     * @brief Computes the cosine of an angle in radians.
     *
     * Currently wraps the standard library implementation.
     *
     * @param x Angle in radians.
     * @return Cosine of @p x.
     */
    [[nodiscard]] inline float cos(const float x) noexcept
    {
        return std::cos(x);
    }

    /**
     * @brief Computes the tangent of an angle in radians.
     *
     * Currently wraps the standard library implementation.
     *
     * @param x Angle in radians.
     * @return Tangent of @p x.
     */
    [[nodiscard]] inline float tan(const float x) noexcept
    {
        return std::tan(x);
    }

    /**
     * @brief Computes the inverse cosine of a value.
     *
     * Input is typically expected to be in the range [-1, 1].
     * Currently wraps the standard library implementation.
     *
     * @param x Input value.
     * @return Angle in radians whose cosine is @p x.
     */
    [[nodiscard]] inline float acos(const float x) noexcept
    {
        return std::acos(x);
    }

    /**
     * @brief Computes the square root of a value.
     *
     * Input is typically expected to be non-negative.
     * Currently wraps the standard library implementation.
     *
     * @param x Input value.
     * @return Square root of @p x.
     */
    [[nodiscard]] inline float sqrt(const float x) noexcept
    {
        return std::sqrt(x);
    }
} // namespace chlm
