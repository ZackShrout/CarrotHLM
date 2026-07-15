//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "MathCommon.h"

namespace chlm::detail::math {
    namespace exponential_detail {
        constexpr double k_ln_two{ 0.69314718055994530942 };
        constexpr double k_inverse_ln_two{ 1.44269504088896340736 };
        constexpr float k_sqrt_two{ 1.41421356237309504880f };
        constexpr float k_subnormal_scale{ 16777216.f }; // 2^24
        constexpr float k_subnormal_result_scale{ 5.9604644775390625e-8f }; // 2^-24

        [[nodiscard]] constexpr float power_of_two(const int exponent) noexcept
        {
            if (exponent > 127) return infinity();

            if (exponent >= -126)
                return bits_to_float(static_cast<std::uint32_t>(exponent + 127) << 23u);

            if (exponent >= -149)
                return bits_to_float(1u << static_cast<unsigned>(exponent + 149));

            return 0.f;
        }

        [[nodiscard]] inline float scale_by_power_of_two(const float value, const int exponent) noexcept
        {
            if (exponent == 128)
                return value * 2.f * power_of_two(127);
            if (exponent >= -126)
                return value * power_of_two(exponent);

            return value * power_of_two(exponent + 24) * k_subnormal_result_scale;
        }

        [[nodiscard]] inline float exp_kernel_fast(const float value) noexcept
        {
            return 1.f + value *
                   (1.f + value *
                    (.5f + value *
                     (1.f / 6.f + value *
                      (1.f / 24.f + value *
                       (1.f / 120.f + value * (1.f / 720.f))))));
        }

        [[nodiscard]] inline double exp_kernel_precise(const double value) noexcept
        {
            return 1.0 + value *
                   (1.0 + value *
                    (1.0 / 2.0 + value *
                     (1.0 / 6.0 + value *
                      (1.0 / 24.0 + value *
                       (1.0 / 120.0 + value *
                        (1.0 / 720.0 + value *
                         (1.0 / 5040.0 + value *
                          (1.0 / 40320.0 + value *
                           (1.0 / 362880.0 + value *
                            (1.0 / 3628800.0 + value * (1.0 / 39916800.0)))))))))));
        }

        template<bool Precise>
        [[nodiscard]] inline float exp2_impl(const float value) noexcept
        {
            if (isnan(value)) return value;
            if (isinf(value)) return value > 0.f ? infinity() : 0.f;
            if (value >= 128.f) return infinity();
            if (value <= -150.f) return 0.f;

            const int nearest_integer{ static_cast<int>(value + (value >= 0.f ? .5f : -.5f)) };
            const float reduced{ value - static_cast<float>(nearest_integer) };

            if (reduced == 0.f) return power_of_two(nearest_integer);

            float approximation;

            if constexpr (Precise)
                approximation = static_cast<float>(exp_kernel_precise(
                    static_cast<double>(reduced) * k_ln_two));
            else
                approximation = exp_kernel_fast(reduced * static_cast<float>(k_ln_two));

            return scale_by_power_of_two(approximation, nearest_integer);
        }

        template<bool Precise>
        [[nodiscard]] inline float exp_impl(const float value) noexcept
        {
            if (isnan(value)) return value;
            if (isinf(value)) return value > 0.f ? infinity() : 0.f;
            if (value >= 89.f) return infinity();
            if (value <= -104.f) return 0.f;
            if (value == 0.f) return 1.f;

            const double scaled{ static_cast<double>(value) * k_inverse_ln_two };
            const int nearest_integer{ static_cast<int>(scaled + (scaled >= 0.0 ? .5 : -.5)) };
            const double reduced{ static_cast<double>(value) - static_cast<double>(nearest_integer) * k_ln_two };
            float approximation;

            if constexpr (Precise)
                approximation = static_cast<float>(exp_kernel_precise(reduced));
            else
                approximation = exp_kernel_fast(static_cast<float>(reduced));

            return scale_by_power_of_two(approximation, nearest_integer);
        }

        struct log_range_t
        {
            float mantissa;
            int exponent;
        };

        [[nodiscard]] inline log_range_t reduce_log(const float value) noexcept
        {
            const bool subnormal{ exponent_bits(value) == 0u };
            const float normalized{ subnormal ? value * k_subnormal_scale : value };
            const std::uint32_t bits{ float_to_bits(normalized) };
            int exponent{ static_cast<int>(bits >> 23u & 0xFFu) - 127 - (subnormal ? 24 : 0) };
            float mantissa{ bits_to_float(bits & 0x007FFFFFu | 0x3F800000u) };

            if (mantissa > k_sqrt_two)
            {
                mantissa *= .5f;
                ++exponent;
            }

            return { mantissa, exponent };
        }

        [[nodiscard]] inline float log_kernel_fast(const float mantissa) noexcept
        {
            const float ratio{ (mantissa - 1.f) / (mantissa + 1.f) };
            const float squared{ ratio * ratio };

            const float series{
                ratio * (1.f + squared *
                         (1.f / 3.f + squared *
                          (1.f / 5.f + squared *
                           (1.f / 7.f + squared * (1.f / 9.f)))))
            };

            return 2.f * series;
        }

        [[nodiscard]] inline float log2_kernel_fast(const float mantissa) noexcept
        {
            return log_kernel_fast(mantissa) * static_cast<float>(k_inverse_ln_two);
        }

        [[nodiscard]] inline double log2_kernel_fast_wide(const double mantissa) noexcept
        {
            const double ratio{ (mantissa - 1.0) / (mantissa + 1.0) };
            const double squared{ ratio * ratio };
            const double series{
                ratio * (1.0 + squared *
                         (1.0 / 3.0 + squared *
                          (1.0 / 5.0 + squared *
                           (1.0 / 7.0 + squared * (1.0 / 9.0)))))
            };

            return 2.0 * k_inverse_ln_two * series;
        }

        [[nodiscard]] inline double log_kernel_precise(const double mantissa) noexcept
        {
            const double ratio{ (mantissa - 1.0) / (mantissa + 1.0) };
            const double squared{ ratio * ratio };

            const double series{
                ratio * (1.0 + squared *
                         (1.0 / 3.0 + squared *
                          (1.0 / 5.0 + squared *
                           (1.0 / 7.0 + squared *
                            (1.0 / 9.0 + squared *
                             (1.0 / 11.0 + squared *
                              (1.0 / 13.0 + squared *
                               (1.0 / 15.0 + squared * (1.0 / 17.0)))))))))
            };

            return 2.0 * series;
        }

        [[nodiscard]] inline double log2_kernel_precise(const double mantissa) noexcept
        {
            return log_kernel_precise(mantissa) * k_inverse_ln_two;
        }

        template<bool Precise>
        [[nodiscard]] inline float log2_impl(const float value) noexcept
        {
            if (isnan(value)) return value;
            if (value == 0.f) return -infinity();
            if (signbit(value)) return quiet_nan();
            if (isinf(value)) return infinity();

            const log_range_t range{ reduce_log(value) };

            if (range.mantissa == 1.f)
                return static_cast<float>(range.exponent);

            if constexpr (Precise)
                return static_cast<float>(static_cast<double>(range.exponent) + log2_kernel_precise(range.mantissa));
            else
                return static_cast<float>(range.exponent) + log2_kernel_fast(range.mantissa);
        }

        template<bool Precise>
        [[nodiscard]] inline float log_impl(const float value) noexcept
        {
            if (isnan(value)) return value;
            if (value == 0.f) return -infinity();
            if (signbit(value)) return quiet_nan();
            if (isinf(value)) return infinity();

            const log_range_t range{ reduce_log(value) };

            if constexpr (Precise)
                return static_cast<float>(static_cast<double>(range.exponent) * k_ln_two +
                                          log_kernel_precise(range.mantissa));
            else
                return static_cast<float>(static_cast<double>(range.exponent) * k_ln_two +
                    static_cast<double>(log_kernel_fast(range.mantissa)));
        }

        template<bool Precise>
        [[nodiscard]] inline double log2_positive_finite(const float value) noexcept
        {
            const log_range_t range{ reduce_log(value) };

            if constexpr (Precise)
                return static_cast<double>(range.exponent) + log2_kernel_precise(range.mantissa);
            else
                return static_cast<double>(range.exponent) + log2_kernel_fast_wide(range.mantissa);
        }

        template<bool Precise>
        [[nodiscard]] inline float exp2_from_double(const double value) noexcept
        {
            if (value >= 128.0) return infinity();
            if (value <= -150.0) return 0.f;

            const int nearest_integer{ static_cast<int>(value + (value >= 0.0 ? .5 : -.5)) };
            const double reduced{ value - static_cast<double>(nearest_integer) };
            float approximation;

            if constexpr (Precise)
                approximation = static_cast<float>(exp_kernel_precise(reduced * k_ln_two));
            else
                approximation = exp_kernel_fast(static_cast<float>(reduced * k_ln_two));

            return scale_by_power_of_two(approximation, nearest_integer);
        }
    } // namespace exponential_detail

    [[nodiscard]] inline float exp2_fast(const float value) noexcept
    {
        return exponential_detail::exp2_impl<false>(value);
    }

    [[nodiscard]] inline float exp2_precise(const float value) noexcept
    {
        return exponential_detail::exp2_impl<true>(value);
    }

    [[nodiscard]] inline float log2_fast(const float value) noexcept
    {
        return exponential_detail::log2_impl<false>(value);
    }

    [[nodiscard]] inline float log2_precise(const float value) noexcept
    {
        return exponential_detail::log2_impl<true>(value);
    }

    [[nodiscard]] inline float exp_fast(const float value) noexcept
    {
        return exponential_detail::exp_impl<false>(value);
    }

    [[nodiscard]] inline float exp_precise(const float value) noexcept
    {
        return exponential_detail::exp_impl<true>(value);
    }

    [[nodiscard]] inline float log_fast(const float value) noexcept
    {
        return exponential_detail::log_impl<false>(value);
    }

    [[nodiscard]] inline float log_precise(const float value) noexcept
    {
        return exponential_detail::log_impl<true>(value);
    }
} // namespace chlm::detail::math
