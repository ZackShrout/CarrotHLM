//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Exponential.h"
#include "InverseTrig.h"
#include "MathCommon.h"
#include "Power.h"
#include "Rounding.h"
#include "Sqrt.h"
#include "Trig.h"

namespace chlm::detail::math {
    [[nodiscard]] inline float exp2(const float value) noexcept
    {
        return exp2_fast(value);
    }

    [[nodiscard]] inline float exp2_precise_fn(const float value) noexcept
    {
        return exp2_precise(value);
    }

    [[nodiscard]] inline float log2(const float value) noexcept
    {
        return log2_fast(value);
    }

    [[nodiscard]] inline float log2_precise_fn(const float value) noexcept
    {
        return log2_precise(value);
    }

    [[nodiscard]] inline float exp(const float value) noexcept
    {
        return exp_fast(value);
    }

    [[nodiscard]] inline float exp_precise_fn(const float value) noexcept
    {
        return exp_precise(value);
    }

    [[nodiscard]] inline float log(const float value) noexcept
    {
        return log_fast(value);
    }

    [[nodiscard]] inline float log_precise_fn(const float value) noexcept
    {
        return log_precise(value);
    }

    [[nodiscard]] inline float pow(const float base, const float exponent) noexcept
    {
        return pow_fast(base, exponent);
    }

    [[nodiscard]] inline float pow_precise_fn(const float base, const float exponent) noexcept
    {
        return pow_precise(base, exponent);
    }

    [[nodiscard]] inline float sin(const float value) noexcept
    {
        return sin_fast(value);
    }

    [[nodiscard]] inline float sin_precise_fn(const float value) noexcept
    {
        return sin_precise(value);
    }

    [[nodiscard]] inline float cos(const float value) noexcept
    {
        return cos_fast(value);
    }

    [[nodiscard]] inline float cos_precise_fn(const float value) noexcept
    {
        return cos_precise(value);
    }

    [[nodiscard]] inline float tan(const float value) noexcept
    {
        return tan_fast(value);
    }

    [[nodiscard]] inline float tan_precise_fn(const float value) noexcept
    {
        return tan_precise(value);
    }

    [[nodiscard]] inline float acos(const float value) noexcept
    {
        return acos_fast(value);
    }

    [[nodiscard]] inline float acos_precise_fn(const float value) noexcept
    {
        return acos_precise(value);
    }

    [[nodiscard]] inline float asin(const float value) noexcept
    {
        return asin_fast(value);
    }

    [[nodiscard]] inline float asin_precise_fn(const float value) noexcept
    {
        return asin_precise(value);
    }

    [[nodiscard]] inline float atan(const float value) noexcept
    {
        return atan_fast(value);
    }

    [[nodiscard]] inline float atan_precise_fn(const float value) noexcept
    {
        return atan_precise(value);
    }

    [[nodiscard]] inline float atan2(const float y, const float x) noexcept
    {
        return atan2_fast(y, x);
    }

    [[nodiscard]] inline float atan2_precise_fn(const float y, const float x) noexcept
    {
        return atan2_precise(y, x);
    }

    [[nodiscard]] inline float sqrt(const float value) noexcept
    {
        return sqrt_fast(value);
    }

    [[nodiscard]] inline float sqrt_precise_fn(const float value) noexcept
    {
        return sqrt_precise(value);
    }

    [[nodiscard]] inline float rsqrt(const float value) noexcept
    {
        return rsqrt_fast(value);
    }

    [[nodiscard]] inline float rsqrt_precise_fn(const float value) noexcept
    {
        return rsqrt_precise(value);
    }
} // namespace chlm::detail::math
