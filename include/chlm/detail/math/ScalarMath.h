//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "InverseTrig.h"
#include "MathCommon.h"
#include "Rounding.h"
#include "Sqrt.h"
#include "Trig.h"

namespace chlm::detail::math {
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

    [[nodiscard]] inline float sqrt(const float value) noexcept
    {
        return sqrt_fast(value);
    }

    [[nodiscard]] inline float sqrt_precise_fn(const float value) noexcept
    {
        return sqrt_precise(value);
    }
} // namespace chlm::detail::math
