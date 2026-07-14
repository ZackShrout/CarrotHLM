//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include <bit>
#include <cstdint>

namespace chlm::detail::math {
    [[nodiscard]] constexpr std::uint32_t float_to_bits(const float value) noexcept
    {
        return std::bit_cast<std::uint32_t>(value);
    }

    [[nodiscard]] constexpr float bits_to_float(const std::uint32_t bits) noexcept
    {
        return std::bit_cast<float>(bits);
    }

    [[nodiscard]] constexpr bool signbit(const float value) noexcept
    {
        return (float_to_bits(value) & 0x80000000u) != 0u;
    }

    [[nodiscard]] constexpr std::uint32_t exponent_bits(const float value) noexcept
    {
        return float_to_bits(value) & 0x7F800000u;
    }

    [[nodiscard]] constexpr std::uint32_t mantissa_bits(const float value) noexcept
    {
        return float_to_bits(value) & 0x007FFFFFu;
    }

    [[nodiscard]] constexpr bool isnan(const float value) noexcept
    {
        return exponent_bits(value) == 0x7F800000u && mantissa_bits(value) != 0u;
    }

    [[nodiscard]] constexpr bool isinf(const float value) noexcept
    {
        return exponent_bits(value) == 0x7F800000u && mantissa_bits(value) == 0u;
    }

    [[nodiscard]] constexpr float copysign(const float magnitude, const float sign) noexcept
    {
        constexpr std::uint32_t sign_mask{ 0x80000000u };

        return bits_to_float(float_to_bits(magnitude) & ~sign_mask | float_to_bits(sign) & sign_mask);
    }
} // namespace chlm::detail::math
