//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Exponential.h"

namespace chlm::detail::math {
    namespace power_detail {
        struct exponent_class_t
        {
            bool integral;
            bool odd;
        };

        [[nodiscard]] inline exponent_class_t classify_exponent(const float value) noexcept
        {
            const float magnitude{ bits_to_float(float_to_bits(value) & 0x7FFFFFFFu) };

            if (magnitude >= 16777216.f) // Every finite float >= 2^24 is an even integer.
                return { true, false };

            if (magnitude < 1.f) return { false, false };

            const int integer{ static_cast<int>(magnitude) };

            if (static_cast<float>(integer) != magnitude)
                return { false, false };

            return { true, (integer & 1) != 0 };
        }

        template<bool Precise>
        [[nodiscard]] inline float integer_power(const float base, const int exponent) noexcept
        {
            unsigned remaining{ static_cast<unsigned>(exponent < 0 ? -exponent : exponent) };

            if constexpr (Precise)
            {
                double factor{ base };
                double result{ 1.0 };

                while (remaining != 0u)
                {
                    if ((remaining & 1u) != 0u) result *= factor;

                    remaining >>= 1u;

                    if (remaining != 0u) factor *= factor;
                }

                return static_cast<float>(exponent < 0 ? 1.0 / result : result);
            }
            else
            {
                float factor{ base };
                float result{ 1.f };

                while (remaining != 0u)
                {
                    if ((remaining & 1u) != 0u) result *= factor;

                    remaining >>= 1u;

                    if (remaining != 0u) factor *= factor;
                }

                return exponent < 0 ? 1.f / result : result;
            }
        }

        template<bool Precise>
        [[nodiscard]] inline float pow_impl(const float base, const float exponent) noexcept
        {
            if (exponent == 0.f || base == 1.f) return 1.f;
            if (base == -1.f && isinf(exponent)) return 1.f;
            if (isnan(base)) return base;
            if (isnan(exponent)) return exponent;

            const float base_magnitude{ bits_to_float(float_to_bits(base) & 0x7FFFFFFFu) };

            if (isinf(exponent))
            {
                if (base_magnitude == 1.f) return 1.f;

                const bool grows{ base_magnitude > 1.f };

                return exponent > 0.f == grows ? infinity() : 0.f;
            }

            const exponent_class_t exponent_class{ classify_exponent(exponent) };

            if (base_magnitude == 0.f)
            {
                const float magnitude{ exponent > 0.f ? 0.f : infinity() };

                return signbit(base) && exponent_class.odd ? -magnitude : magnitude;
            }

            if (isinf(base))
            {
                const float magnitude{ exponent > 0.f ? infinity() : 0.f };

                return signbit(base) && exponent_class.odd ? -magnitude : magnitude;
            }

            if (signbit(base) && !exponent_class.integral) return quiet_nan();

            float magnitude;

            if (exponent_class.integral && exponent >= -64.f && exponent <= 64.f)
            {
                magnitude = integer_power<Precise>(base_magnitude, static_cast<int>(exponent));
            }
            else
            {
                const double logarithm{ exponential_detail::log2_positive_finite<Precise>(base_magnitude) };
                magnitude = exponential_detail::exp2_from_double<Precise>(logarithm * static_cast<double>(exponent));
            }

            return signbit(base) && exponent_class.odd ? -magnitude : magnitude;
        }
    } // namespace power_detail

    [[nodiscard]] inline float pow_fast(const float base, const float exponent) noexcept
    {
        return power_detail::pow_impl<false>(base, exponent);
    }

    [[nodiscard]] inline float pow_precise(const float base, const float exponent) noexcept
    {
        return power_detail::pow_impl<true>(base, exponent);
    }
} // namespace chlm::detail::math
