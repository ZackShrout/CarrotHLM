//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "detail/math/ScalarMath.h"

namespace chlm {
    /** @brief Paired sine and cosine values returned by `sin_cos`. */
    struct sin_cos_result
    {
        float sine;
        float cosine;
    };

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
     * @brief Computes two raised to a power.
     *
     * Uses explicit binary range reduction and reconstruction. Positive finite
     * normal results have a maximum relative error target of 5e-7, while all
     * finite results have a maximum error target of 4 ULP. Integer inputs in
     * [-149, 127] produce exact powers of two, including subnormals. Results
     * overflow to infinity and underflow to zero according to float range.
     * Either sign of zero returns one, positive infinity returns infinity,
     * negative infinity returns zero, and NaN is propagated.
     *
     * @param x Exponent value.
     * @return Two raised to @p x.
     */
    [[nodiscard]] inline float exp2(const float x) noexcept
    {
        return detail::math::exp2(x);
    }

    /**
     * @brief Computes two raised to a power using the higher-accuracy path.
     *
     * Uses a higher-degree polynomial evaluated in double with a maximum error
     * target of 1 ULP. Special values and range limits match `exp2`.
     *
     * @param x Exponent value.
     * @return Two raised to @p x.
     */
    [[nodiscard]] inline float exp2_precise(const float x) noexcept
    {
        return detail::math::exp2_precise_fn(x);
    }

    /**
     * @brief Computes the base-two logarithm of a value.
     *
     * Uses explicit exponent and mantissa decomposition with a maximum error
     * target of 4 ULP. Exact powers of two, including subnormals, return their
     * exact integer exponents. Zero returns negative infinity, negative inputs
     * return NaN, positive infinity returns infinity, and NaN is propagated.
     *
     * @param x Positive input value.
     * @return Base-two logarithm of @p x.
     */
    [[nodiscard]] inline float log2(const float x) noexcept
    {
        return detail::math::log2(x);
    }

    /**
     * @brief Computes the base-two logarithm using the higher-accuracy path.
     *
     * Evaluates the reduced series in double with a maximum error target of
     * 1 ULP. Special values and exact powers match `log2`.
     *
     * @param x Positive input value.
     * @return Base-two logarithm of @p x.
     */
    [[nodiscard]] inline float log2_precise(const float x) noexcept
    {
        return detail::math::log2_precise_fn(x);
    }

    /**
     * @brief Computes the natural exponential of a value.
     *
     * Uses natural-base range reduction with the owned exponential kernel.
     * Normal results have a maximum relative error target of 5e-7, while all
     * finite results have a maximum error target of 4 ULP. Either sign of zero
     * returns one, positive infinity returns infinity, negative infinity returns
     * zero, and NaN is propagated.
     *
     * @param x Exponent value.
     * @return Euler's number raised to @p x.
     */
    [[nodiscard]] inline float exp(const float x) noexcept
    {
        return detail::math::exp(x);
    }

    /**
     * @brief Computes the natural exponential using the higher-accuracy path.
     *
     * Uses double-precision reduction and polynomial evaluation with a maximum
     * error target of 1 ULP. Special values and range limits match `exp`.
     *
     * @param x Exponent value.
     * @return Euler's number raised to @p x.
     */
    [[nodiscard]] inline float exp_precise(const float x) noexcept
    {
        return detail::math::exp_precise_fn(x);
    }

    /**
     * @brief Computes the natural logarithm of a value.
     *
     * Uses explicit exponent and mantissa decomposition with a maximum error
     * target of 4 ULP. Zero returns negative infinity, negative inputs return
     * NaN, positive infinity returns infinity, and NaN is propagated.
     *
     * @param x Positive input value.
     * @return Natural logarithm of @p x.
     */
    [[nodiscard]] inline float log(const float x) noexcept
    {
        return detail::math::log(x);
    }

    /**
     * @brief Computes the natural logarithm using the higher-accuracy path.
     *
     * Evaluates exponent reconstruction and the reduced series in double with
     * a maximum error target of 1 ULP. Special values match `log`.
     *
     * @param x Positive input value.
     * @return Natural logarithm of @p x.
     */
    [[nodiscard]] inline float log_precise(const float x) noexcept
    {
        return detail::math::log_precise_fn(x);
    }

    /**
     * @brief Raises a base to an exponent.
     *
     * Integral exponents use a specialized squaring path when practical;
     * general magnitudes use the owned logarithm and exponential kernels.
     * Normal finite results have a maximum relative error target of 1.5e-6;
     * subnormal results have a maximum error target of 4 ULP. Negative finite
     * bases require an integral exponent. Signed zero and infinity preserve
     * their sign only for odd integral exponents.
     *
     * An exponent of zero or a base of one returns one even when the other
     * argument is NaN. `pow(-1, +/-infinity)` also returns one. Other NaN
     * arguments propagate, and infinite exponents are resolved from `abs(base)`
     * relative to one.
     *
     * @param base Base value.
     * @param exponent Exponent value.
     * @return @p base raised to @p exponent.
     */
    [[nodiscard]] inline float pow(const float base, const float exponent) noexcept
    {
        return detail::math::pow(base, exponent);
    }

    /**
     * @brief Raises a base to an exponent using the higher-accuracy path.
     *
     * Uses double-precision intermediate evaluation with a maximum relative
     * error target of 5e-7 for normal finite results and a maximum subnormal
     * error target of 1 ULP. Domain and special-value behavior match `pow`.
     *
     * @param base Base value.
     * @param exponent Exponent value.
     * @return @p base raised to @p exponent.
     */
    [[nodiscard]] inline float pow_precise(const float base, const float exponent) noexcept
    {
        return detail::math::pow_precise_fn(base, exponent);
    }

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
     * @brief Computes sine and cosine with one shared angle reduction.
     *
     * Each component has a maximum absolute error target of 5e-6. NaN and
     * infinity produce NaN components; signed zero produces `{x, 1}`.
     *
     * @param x Angle in radians.
     * @return Aggregate containing the sine and cosine of @p x.
     */
    [[nodiscard]] inline sin_cos_result sin_cos(const float x) noexcept
    {
        const detail::math::sin_cos_t result{ detail::math::sin_cos_fast(x) };
        return { result.sine, result.cosine };
    }

    /**
     * @brief Computes sine and cosine with one shared higher-accuracy reduction.
     *
     * Each component has a maximum absolute error target of 1.5e-7. NaN and
     * infinity produce NaN components; signed zero produces `{x, 1}`.
     *
     * @param x Angle in radians.
     * @return Aggregate containing the sine and cosine of @p x.
     */
    [[nodiscard]] inline sin_cos_result sin_cos_precise(const float x) noexcept
    {
        const detail::math::sin_cos_t result{ detail::math::sin_cos_precise(x) };
        return { result.sine, result.cosine };
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
     * @brief Computes the inverse sine of a value.
     *
     * Uses a cubic-times-square-root approximation with a maximum absolute
     * error target of 7.5e-5. Inputs outside [-1, 1] return NaN.
     *
     * @param x Input value.
     * @return Angle in radians whose sine is @p x.
     */
    [[nodiscard]] inline float asin(const float x) noexcept
    {
        return detail::math::asin(x);
    }

    /**
     * @brief Computes the inverse sine using the higher-accuracy path.
     *
     * Uses an endpoint-stable higher-order rational approximation with a
     * maximum absolute error target of 2.5e-7. Inputs outside [-1, 1] return NaN.
     *
     * @param x Input value.
     * @return Angle in radians whose sine is @p x.
     */
    [[nodiscard]] inline float asin_precise(const float x) noexcept
    {
        return detail::math::asin_precise_fn(x);
    }

    /**
     * @brief Computes the inverse tangent of a value.
     *
     * Uses a reduced-domain polynomial with a maximum absolute error target
     * of 6e-6. Signed zero and infinity produce signed exact-limit results.
     *
     * @param x Input value.
     * @return Angle in radians whose tangent is @p x.
     */
    [[nodiscard]] inline float atan(const float x) noexcept
    {
        return detail::math::atan(x);
    }

    /**
     * @brief Computes the inverse tangent using the higher-accuracy path.
     *
     * Uses additional range reduction and a higher-degree polynomial evaluated
     * in double, with a maximum absolute error target of 2.5e-7.
     *
     * @param x Input value.
     * @return Angle in radians whose tangent is @p x.
     */
    [[nodiscard]] inline float atan_precise(const float x) noexcept
    {
        return detail::math::atan_precise_fn(x);
    }

    /**
     * @brief Computes the quadrant-aware inverse tangent of @p y / @p x.
     *
     * Uses a bounded-ratio polynomial and explicit IEEE 754 handling for signed
     * zero and infinity. The maximum absolute error target is 6e-6.
     *
     * For either sign of zero in @p y, positive @p x produces the same signed
     * zero and negative @p x produces signed pi. A nonzero @p y with zero @p x
     * produces signed half-pi. Infinite @p y produces signed half-pi when @p x
     * is finite; infinite @p x produces signed zero or signed pi according to
     * its sign. When both arguments are infinite, the result is signed pi/4 or
     * signed 3pi/4 according to the quadrant. A NaN argument returns NaN.
     *
     * @param y Signed vertical component.
     * @param x Signed horizontal component.
     * @return Angle in radians in [-pi, pi].
     */
    [[nodiscard]] inline float atan2(const float y, const float x) noexcept
    {
        return detail::math::atan2(y, x);
    }

    /**
     * @brief Computes the quadrant-aware inverse tangent using the higher-accuracy path.
     *
     * Uses double-precision range reduction and polynomial evaluation, with a
     * maximum absolute error target of 2.5e-7. Signed zero and infinity follow
     * the same explicitly documented quadrant conventions as `atan2`.
     *
     * @param y Signed vertical component.
     * @param x Signed horizontal component.
     * @return Angle in radians in [-pi, pi].
     */
    [[nodiscard]] inline float atan2_precise(const float y, const float x) noexcept
    {
        return detail::math::atan2_precise_fn(y, x);
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

    /**
     * @brief Computes the reciprocal square root of a value.
     *
     * Uses an owned inverse-root estimate and two Newton refinement steps.
     * Positive finite results have a maximum relative error target of 5e-6.
     * Positive zero returns positive infinity, negative zero returns negative
     * infinity, positive infinity returns zero, negative values return NaN, and
     * NaN is propagated. Positive subnormals are normalized internally and meet
     * the same error target as normal inputs.
     *
     * @param x Input value.
     * @return Reciprocal square root of @p x.
     */
    [[nodiscard]] inline float rsqrt(const float x) noexcept
    {
        return detail::math::rsqrt(x);
    }

    /**
     * @brief Computes the reciprocal square root using the higher-accuracy path.
     *
     * Adds a double-precision refinement to the default implementation. Positive
     * finite results have a maximum error target of 1 ULP. Special values follow
     * the same explicitly documented behavior as `rsqrt`.
     *
     * @param x Input value.
     * @return Reciprocal square root of @p x.
     */
    [[nodiscard]] inline float rsqrt_precise(const float x) noexcept
    {
        return detail::math::rsqrt_precise_fn(x);
    }
} // namespace chlm
