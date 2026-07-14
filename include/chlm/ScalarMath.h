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
