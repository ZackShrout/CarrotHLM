//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "MathCommon.h"

#include <cstdint>

namespace chlm::detail::math {
    namespace rounding_detail {
        constexpr std::uint32_t k_sign_mask{ 0x80000000u };
        constexpr std::uint32_t k_magnitude_mask{ 0x7FFFFFFFu };
        constexpr std::uint32_t k_mantissa_mask{ 0x007FFFFFu };
        constexpr std::uint32_t k_hidden_bit{ 0x00800000u };

        [[nodiscard]] constexpr int unbiased_exponent(const std::uint32_t bits) noexcept
        {
            return static_cast<int>(bits >> 23u & 0xFFu) - 127;
        }

        [[nodiscard]] constexpr std::uint32_t fractional_mask(const int exponent) noexcept
        {
            return k_mantissa_mask >> exponent;
        }

        [[nodiscard]] constexpr float signed_zero(const std::uint32_t bits) noexcept
        {
            return bits_to_float(bits & k_sign_mask);
        }
    } // namespace rounding_detail

    [[nodiscard]] constexpr float trunc(const float value) noexcept
    {
        const std::uint32_t bits{ float_to_bits(value) };
        const int exponent{ rounding_detail::unbiased_exponent(bits) };

        if (exponent < 0) return rounding_detail::signed_zero(bits);
        if (exponent >= 23) return value;

        return bits_to_float(bits & ~rounding_detail::fractional_mask(exponent));
    }

    [[nodiscard]] constexpr float floor(const float value) noexcept
    {
        const std::uint32_t bits{ float_to_bits(value) };
        const int exponent{ rounding_detail::unbiased_exponent(bits) };

        if (exponent < 0)
        {
            if ((bits & rounding_detail::k_magnitude_mask) == 0u) return value;

            return (bits & rounding_detail::k_sign_mask) == 0u ? 0.f : -1.f;
        }

        if (exponent >= 23) return value;

        const std::uint32_t mask{ rounding_detail::fractional_mask(exponent) };

        if ((bits & mask) == 0u) return value;

        const std::uint32_t integral_bits{ bits & ~mask };

        if ((bits & rounding_detail::k_sign_mask) == 0u)
            return bits_to_float(integral_bits);

        return bits_to_float(integral_bits) - 1.f;
    }

    [[nodiscard]] constexpr float ceil(const float value) noexcept
    {
        const std::uint32_t bits{ float_to_bits(value) };
        const int exponent{ rounding_detail::unbiased_exponent(bits) };

        if (exponent < 0)
        {
            if ((bits & rounding_detail::k_magnitude_mask) == 0u) return value;
            if ((bits & rounding_detail::k_sign_mask) == 0u) return 1.f;

            return rounding_detail::signed_zero(bits);
        }

        if (exponent >= 23) return value;

        const std::uint32_t mask{ rounding_detail::fractional_mask(exponent) };

        if ((bits & mask) == 0u) return value;

        const std::uint32_t integral_bits{ bits & ~mask };

        if ((bits & rounding_detail::k_sign_mask) != 0u)
            return bits_to_float(integral_bits);

        return bits_to_float(integral_bits) + 1.f;
    }

    [[nodiscard]] constexpr float round(const float value) noexcept
    {
        const std::uint32_t bits{ float_to_bits(value) };
        const std::uint32_t magnitude{ bits & rounding_detail::k_magnitude_mask };
        const int exponent{ rounding_detail::unbiased_exponent(bits) };

        if (exponent < -1) return rounding_detail::signed_zero(bits);
        if (exponent < 0) return copysign(1.f, value);
        if (exponent >= 23) return value;

        const std::uint32_t mask{ rounding_detail::fractional_mask(exponent) };

        if ((magnitude & mask) == 0u) return value;

        const std::uint32_t halfway{ 1u << (22 - exponent) };
        const std::uint32_t rounded_magnitude{ magnitude + halfway & ~mask };

        return bits_to_float(bits & rounding_detail::k_sign_mask | rounded_magnitude);
    }

    /*
     * The shift/subtract remainder implementation is adapted from musl libc.
     * Copyright (c) 2005-2020 Rich Felker, et al.
     *
     * Permission is hereby granted, free of charge, to any person obtaining a
     * copy of this software and associated documentation files (the "Software"),
     * to deal in the Software without restriction, including without limitation
     * the rights to use, copy, modify, merge, publish, distribute, sublicense,
     * and/or sell copies of the Software, and to permit persons to whom the
     * Software is furnished to do so, subject to the following conditions:
     *
     * The above copyright notice and this permission notice shall be included
     * in all copies or substantial portions of the Software.
     *
     * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
     * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
     * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
     * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
     * DEALINGS IN THE SOFTWARE.
     */
    [[nodiscard]] constexpr float fmod(const float dividend, const float divisor) noexcept
    {
        const std::uint32_t dividend_bits{ float_to_bits(dividend) };
        const std::uint32_t divisor_bits{ float_to_bits(divisor) };
        std::uint32_t dividend_magnitude{ dividend_bits & rounding_detail::k_magnitude_mask };
        std::uint32_t divisor_magnitude{ divisor_bits & rounding_detail::k_magnitude_mask };

        if (isnan(dividend)) return dividend;
        if (isnan(divisor)) return divisor;
        if (divisor_magnitude == 0u || isinf(dividend)) return quiet_nan();
        if (dividend_magnitude < divisor_magnitude) return dividend;
        if (dividend_magnitude == divisor_magnitude) return rounding_detail::signed_zero(dividend_bits);

        int dividend_exponent{ static_cast<int>(dividend_magnitude >> 23u & 0xFFu) };
        int divisor_exponent{ static_cast<int>(divisor_magnitude >> 23u & 0xFFu) };
        std::uint32_t shifted;

        if (dividend_exponent == 0)
        {
            for (shifted = dividend_magnitude << 9u; shifted >> 31u == 0u; --dividend_exponent, shifted <<= 1u) {}

            dividend_magnitude <<= static_cast<unsigned>(-dividend_exponent + 1);
        }
        else
        {
            dividend_magnitude = dividend_magnitude & rounding_detail::k_mantissa_mask | rounding_detail::k_hidden_bit;
        }

        if (divisor_exponent == 0)
        {
            for (shifted = divisor_magnitude << 9u; shifted >> 31u == 0u; --divisor_exponent, shifted <<= 1u) {}

            divisor_magnitude <<= static_cast<unsigned>(-divisor_exponent + 1);
        }
        else
        {
            divisor_magnitude = divisor_magnitude & rounding_detail::k_mantissa_mask | rounding_detail::k_hidden_bit;
        }

        for (; dividend_exponent > divisor_exponent; --dividend_exponent)
        {
            shifted = dividend_magnitude - divisor_magnitude;

            if (shifted >> 31u == 0u)
            {
                if (shifted == 0u) return rounding_detail::signed_zero(dividend_bits);

                dividend_magnitude = shifted;
            }

            dividend_magnitude <<= 1u;
        }

        shifted = dividend_magnitude - divisor_magnitude;

        if (shifted >> 31u == 0u)
        {
            if (shifted == 0u) return rounding_detail::signed_zero(dividend_bits);

            dividend_magnitude = shifted;
        }

        while (dividend_magnitude >> 23u == 0u)
        {
            dividend_magnitude <<= 1u;
            --dividend_exponent;
        }

        if (dividend_exponent > 0)
        {
            dividend_magnitude -= rounding_detail::k_hidden_bit;
            dividend_magnitude |= static_cast<std::uint32_t>(dividend_exponent) << 23u;
        }
        else
        {
            dividend_magnitude >>= static_cast<unsigned>(-dividend_exponent + 1);
        }

        return bits_to_float(dividend_magnitude | dividend_bits & rounding_detail::k_sign_mask);
    }

    [[nodiscard]] constexpr float frac(const float value) noexcept
    {
        if (isnan(value)) return value;
        if (isinf(value)) return quiet_nan();

        return value - floor(value);
    }
} // namespace chlm::detail::math
