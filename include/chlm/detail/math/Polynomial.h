//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

namespace chlm::detail::math {
    template<typename T>
    [[nodiscard]] constexpr T horner(const T x, const T c0) noexcept
    {
        return c0;
    }

    template<typename T, typename... Coefficients>
    [[nodiscard]] constexpr T horner(const T x, const T c0, const Coefficients... coefficients) noexcept
    {
        return c0 + x * horner(x, coefficients...);
    }
} // namespace chlm::detail::math
