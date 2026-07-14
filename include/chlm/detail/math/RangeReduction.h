//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "MathCommon.h"

#include <cstdint>

namespace chlm::detail::math {
    struct trig_range_t
    {
        double reduced;
        int quadrant;
    };

    namespace range_reduction_detail {
        constexpr double k_inverse_half_pi{ 6.36619772367581382433e-01 };
        constexpr double k_half_pi_high{ 1.57079631090164184570e+00 };
        constexpr double k_half_pi_tail{ 1.58932547735281966916e-08 };
        constexpr double k_quarter_pi{ 7.85398163397448278999e-01 };
        constexpr float k_medium_reduction_limit{ 1048576.f };
        constexpr double k_two_pow_minus_64{ 1.0 / 18446744073709551616.0 };

        // floor((2/pi) * 2^192), stored as little-endian 32-bit limbs.
        constexpr std::uint32_t k_two_over_pi[]{
            0x3C439041u, 0xDB629599u, 0xF534DDC0u,
            0xFC2757D1u, 0x4E441529u, 0xA2F9836Eu
        };

        [[nodiscard]] constexpr std::uint32_t product_bit(const std::uint32_t (&product)[7], const int index) noexcept
        {
            if (index < 0 || index >= 224) return 0u;

            return product[index / 32] >> index % 32 & 1u;
        }

        inline void multiply_two_over_pi(const std::uint32_t mantissa, std::uint32_t (&product)[7]) noexcept
        {
            std::uint64_t carry{ 0u };
            for (int i{ 0 }; i < 6; ++i)
            {
                const std::uint64_t term{ static_cast<std::uint64_t>(k_two_over_pi[i]) * mantissa + carry };
                product[i] = static_cast<std::uint32_t>(term);
                carry = term >> 32u;
            }
            product[6] = static_cast<std::uint32_t>(carry);
        }

        [[nodiscard]] inline trig_range_t reduce_large_positive(const float value) noexcept
        {
            const std::uint32_t bits{ float_to_bits(value) };
            const std::uint32_t mantissa{ bits & 0x007FFFFFu | 0x00800000u };
            const int exponent{ static_cast<int>(bits >> 23u & 0xFFu) - 127 };
            const int binary_point{ 215 - exponent };

            std::uint32_t product[7]{ };
            multiply_two_over_pi(mantissa, product);

            // Keep the quotient mod 4 for quadrant selection and the leading
            // 64 fractional bits for a reduced argument in [-pi/4, pi/4].
            int quadrant{
                static_cast<int>(product_bit(product, binary_point)) |
                static_cast<int>(product_bit(product, binary_point + 1)) << 1
            };

            std::uint64_t fraction_bits{ 0u };

            for (int i{ 1 }; i <= 64; ++i)
                fraction_bits = fraction_bits << 1u | product_bit(product, binary_point - i);

            double fraction{ static_cast<double>(fraction_bits) * k_two_pow_minus_64 };

            if (product_bit(product, binary_point - 1) != 0u)
            {
                fraction -= 1.0;
                quadrant = quadrant + 1 & 3;
            }

            return trig_range_t{ fraction * (k_half_pi_high + k_half_pi_tail), quadrant };
        }

        [[nodiscard]] inline trig_range_t reduce_medium_positive(const float value) noexcept
        {
            const double scaled{ static_cast<double>(value) * k_inverse_half_pi };
            int quadrant_count{ static_cast<int>(scaled + 0.5) };
            double reduced{
                static_cast<double>(value) - quadrant_count * k_half_pi_high - quadrant_count * k_half_pi_tail
            };

            if (reduced < -k_quarter_pi)
            {
                --quadrant_count;
                reduced += k_half_pi_high + k_half_pi_tail;
            }
            else if (reduced > k_quarter_pi)
            {
                ++quadrant_count;
                reduced -= k_half_pi_high + k_half_pi_tail;
            }

            return trig_range_t{ reduced, quadrant_count & 3 };
        }
    } // namespace range_reduction_detail

    [[nodiscard]] inline trig_range_t reduce_angle(const float value) noexcept
    {
        const bool negative{ signbit(value) };
        const float magnitude{ bits_to_float(float_to_bits(value) & 0x7FFFFFFFu) };

        trig_range_t result;

        if (magnitude <= static_cast<float>(range_reduction_detail::k_quarter_pi))
            result = trig_range_t{ magnitude, 0 };
        else if (magnitude < range_reduction_detail::k_medium_reduction_limit)
            result = range_reduction_detail::reduce_medium_positive(magnitude);
        else
            result = range_reduction_detail::reduce_large_positive(magnitude);

        if (negative)
        {
            result.reduced = -result.reduced;
            result.quadrant = 4 - result.quadrant & 3;
        }

        return result;
    }
} // namespace chlm::detail::math
