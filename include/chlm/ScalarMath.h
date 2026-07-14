//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "detail/math/ScalarMath.h"

namespace chlm {
    // These forward to CarrotHLM's owned, platform-independent math layer.
    // The default functions favor game-runtime throughput; `_precise`
    // variants trade additional work for tighter error bounds.

    // ========================================
    // Classification and sign
    // ========================================

    /**
     * @brief Determines whether a value is NaN.
     *
     * @param x Input value.
     * @return true when @p x has an IEEE 754 NaN encoding.
     */
    [[nodiscard]] constexpr bool isnan(const float x) noexcept
    {
        return detail::math::isnan(x);
    }

    /**
     * @brief Determines whether a value is positive or negative infinity.
     *
     * @param x Input value.
     * @return true when @p x has an IEEE 754 infinity encoding.
     */
    [[nodiscard]] constexpr bool isinf(const float x) noexcept
    {
        return detail::math::isinf(x);
    }

    /**
     * @brief Determines whether a value is finite.
     *
     * Finite values include normal numbers, subnormal numbers, and signed zero.
     *
     * @param x Input value.
     * @return true when @p x is neither infinity nor NaN.
     */
    [[nodiscard]] constexpr bool isfinite(const float x) noexcept
    {
        return detail::math::isfinite(x);
    }

    /**
     * @brief Determines whether the sign bit of a value is set.
     *
     * This detects negative zero and signed NaN encodings as well as negative values.
     *
     * @param x Input value.
     * @return true when the sign bit of @p x is set.
     */
    [[nodiscard]] constexpr bool signbit(const float x) noexcept
    {
        return detail::math::signbit(x);
    }

    /**
     * @brief Returns the magnitude of one value with the sign of another.
     *
     * The operation changes only the sign bit. All exponent and mantissa bits,
     * including a NaN payload, are preserved from @p magnitude.
     *
     * @param magnitude Value providing the exponent and mantissa bits.
     * @param sign Value providing the sign bit.
     * @return @p magnitude with the sign bit of @p sign.
     */
    [[nodiscard]] constexpr float copysign(const float magnitude, const float sign) noexcept
    {
        return detail::math::copysign(magnitude, sign);
    }

    // ========================================
    // Rounding and fractional operations
    // ========================================

    /**
     * @brief Rounds a value downward to the nearest integer value.
     *
     * NaN, infinity, and signed zero are returned unchanged.
     *
     * @param x Input value.
     * @return Greatest integer value not greater than @p x.
     */
    [[nodiscard]] constexpr float floor(const float x) noexcept
    {
        return detail::math::floor(x);
    }

    /**
     * @brief Rounds a value upward to the nearest integer value.
     *
     * NaN, infinity, and signed zero are returned unchanged.
     *
     * @param x Input value.
     * @return Smallest integer value not less than @p x.
     */
    [[nodiscard]] constexpr float ceil(const float x) noexcept
    {
        return detail::math::ceil(x);
    }

    /**
     * @brief Rounds a value toward zero to an integer value.
     *
     * NaN, infinity, and signed zero are returned unchanged.
     *
     * @param x Input value.
     * @return Integer portion of @p x with fractional bits discarded.
     */
    [[nodiscard]] constexpr float trunc(const float x) noexcept
    {
        return detail::math::trunc(x);
    }

    /**
     * @brief Rounds a value to the nearest integer value.
     *
     * Halfway cases round away from zero. NaN, infinity, and signed zero are
     * returned unchanged.
     *
     * @param x Input value.
     * @return Nearest integer value to @p x.
     */
    [[nodiscard]] constexpr float round(const float x) noexcept
    {
        return detail::math::round(x);
    }

    /**
     * @brief Computes the floating-point remainder after truncating division.
     *
     * A nonzero result has the sign of @p dividend and a magnitude less than
     * `abs(divisor)`. A zero divisor or infinite dividend returns NaN. A finite
     * dividend with an infinite divisor is returned unchanged.
     *
     * @param dividend Value to divide.
     * @param divisor Value by which to divide.
     * @return Remainder of @p dividend / @p divisor.
     */
    [[nodiscard]] constexpr float fmod(const float dividend, const float divisor) noexcept
    {
        return detail::math::fmod(dividend, divisor);
    }

    /**
     * @brief Returns the HLSL-style fractional part of a value.
     *
     * Finite results are in [0, 1) and are computed as `x - floor(x)`.
     * NaN is propagated and infinity returns NaN.
     *
     * @param x Input value.
     * @return Nonnegative fractional component of @p x.
     */
    [[nodiscard]] constexpr float frac(const float x) noexcept
    {
        return detail::math::frac(x);
    }

    // ========================================
    // Transcendental functions
    // ========================================

    /**
     * @brief Computes the sine of an angle in radians.
     *
     * Uses CarrotHLM's owned range reduction and lower-degree polynomial path.
     * Finite results have a maximum absolute error target of 5e-6.
     *
     * @param x Angle in radians.
     * @return Sine of @p x.
     */
    [[nodiscard]] inline float sin(const float x) noexcept
    {
        return detail::math::sin(x);
    }

    /**
     * @brief Computes the sine of an angle in radians using the higher-accuracy path.
     *
     * Uses robust range reduction and a higher-degree polynomial evaluated in double.
     * Finite results have a maximum absolute error target of 1.5e-7.
     *
     * @param x Angle in radians.
     * @return Sine of @p x.
     */
    [[nodiscard]] inline float sin_precise(const float x) noexcept
    {
        return detail::math::sin_precise_fn(x);
    }

    /**
     * @brief Computes the cosine of an angle in radians.
     *
     * Uses CarrotHLM's owned range reduction and lower-degree polynomial path.
     * Finite results have a maximum absolute error target of 5e-6.
     *
     * @param x Angle in radians.
     * @return Cosine of @p x.
     */
    [[nodiscard]] inline float cos(const float x) noexcept
    {
        return detail::math::cos(x);
    }

    /**
     * @brief Computes the cosine of an angle in radians using the higher-accuracy path.
     *
     * Uses robust range reduction and a higher-degree polynomial evaluated in double.
     * Finite results have a maximum absolute error target of 1.5e-7.
     *
     * @param x Angle in radians.
     * @return Cosine of @p x.
     */
    [[nodiscard]] inline float cos_precise(const float x) noexcept
    {
        return detail::math::cos_precise_fn(x);
    }

    /**
     * @brief Computes the tangent of an angle in radians.
     *
     * Uses CarrotHLM's owned range reduction and lower-degree sine/cosine kernels.
     * The scaled-error target is 6e-6: absolute error for result magnitudes up
     * to one, and relative error for larger magnitudes.
     *
     * @param x Angle in radians.
     * @return Tangent of @p x.
     */
    [[nodiscard]] inline float tan(const float x) noexcept
    {
        return detail::math::tan(x);
    }

    /**
     * @brief Computes the tangent of an angle in radians using the higher-accuracy path.
     *
     * Uses the higher-degree sine/cosine kernels evaluated in double.
     * The scaled-error target is 2.5e-7: absolute error for result magnitudes
     * up to one, and relative error for larger magnitudes.
     *
     * @param x Angle in radians.
     * @return Tangent of @p x.
     */
    [[nodiscard]] inline float tan_precise(const float x) noexcept
    {
        return detail::math::tan_precise_fn(x);
    }

    /**
     * @brief Computes the inverse cosine of a value.
     *
     * Uses a cubic-times-square-root approximation with a maximum absolute
     * error target of 7.5e-5. Inputs outside [-1, 1] return NaN.
     *
     * @param x Input value.
     * @return Angle in radians whose cosine is @p x.
     */
    [[nodiscard]] inline float acos(const float x) noexcept
    {
        return detail::math::acos(x);
    }

    /**
     * @brief Computes the inverse cosine using the higher-accuracy path.
     *
     * Uses an endpoint-stable higher-order rational approximation with a
     * maximum absolute error target of 2.5e-7. Inputs outside [-1, 1] return NaN.
     *
     * @param x Input value.
     * @return Angle in radians whose cosine is @p x.
     */
    [[nodiscard]] inline float acos_precise(const float x) noexcept
    {
        return detail::math::acos_precise_fn(x);
    }

    /**
     * @brief Computes the square root of a value.
     *
     * Preserves signed zero and positive infinity. Negative values return NaN.
     * Positive finite results have a maximum error target of 4 ULP.
     *
     * @param x Input value.
     * @return Square root of @p x.
     */
    [[nodiscard]] inline float sqrt(const float x) noexcept
    {
        return detail::math::sqrt(x);
    }

    /**
     * @brief Computes the square root using the higher-accuracy path.
     *
     * Preserves signed zero and positive infinity. Negative values return NaN.
     * Uses an additional refinement step over the default implementation.
     * Positive finite results have a maximum error target of 1 ULP.
     *
     * @param x Input value.
     * @return Square root of @p x.
     */
    [[nodiscard]] inline float sqrt_precise(const float x) noexcept
    {
        return detail::math::sqrt_precise_fn(x);
    }
} // namespace chlm
