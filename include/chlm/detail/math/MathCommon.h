//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "FloatBits.h"

#include <limits>

namespace chlm::detail::math {
    constexpr float k_pi{ 3.1415926535897932384626433832795f };
    constexpr float k_half_pi{ k_pi * 0.5f };
    constexpr float k_two_pi{ k_pi * 2.f };

    [[nodiscard]] constexpr float quiet_nan() noexcept
    {
        return std::numeric_limits<float>::quiet_NaN();
    }

    [[nodiscard]] constexpr float infinity() noexcept
    {
        return std::numeric_limits<float>::infinity();
    }

    [[nodiscard]] constexpr bool isfinite(const float value) noexcept
    {
        return !isnan(value) && !isinf(value);
    }
} // namespace chlm::detail::math
