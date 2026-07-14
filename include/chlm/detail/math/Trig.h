//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "MathCommon.h"
#include "RangeReduction.h"

namespace chlm::detail::math {
    namespace trig_detail {
        /*
         * Polynomial coefficients are adapted from the FreeBSD msun float kernels.
         * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
         * Developed at SunPro. Permission to use, copy, modify, and distribute this
         * software is freely granted, provided that this notice is preserved.
         */
        constexpr float k_fast_sin_1{ -0.166666666416265235595f };
        constexpr float k_fast_sin_2{ 0.0083333293858894631756f };
        constexpr float k_fast_sin_3{ -0.000198393348360966317347f };
        constexpr float k_fast_cos_0{ -0.499999997251031003120f };
        constexpr float k_fast_cos_1{ 0.0416666233237390631894f };
        constexpr float k_fast_cos_2{ -0.00138867637746099294692f };

        constexpr double k_precise_sin_1{ -0.166666666416265235595 };
        constexpr double k_precise_sin_2{ 0.0083333293858894631756 };
        constexpr double k_precise_sin_3{ -0.000198393348360966317347 };
        constexpr double k_precise_sin_4{ 0.0000027183114939898219064 };
        constexpr double k_precise_cos_0{ -0.499999997251031003120 };
        constexpr double k_precise_cos_1{ 0.0416666233237390631894 };
        constexpr double k_precise_cos_2{ -0.00138867637746099294692 };
        constexpr double k_precise_cos_3{ 0.0000243904487962774090654 };

        [[nodiscard]] inline float sine_kernel_fast(const double value) noexcept
        {
            const float x{ static_cast<float>(value) };
            const float squared{ x * x };

            return x + x * squared * (k_fast_sin_1 + squared * (k_fast_sin_2 + squared * k_fast_sin_3));
        }

        [[nodiscard]] inline float cosine_kernel_fast(const double value) noexcept
        {
            const float x{ static_cast<float>(value) };
            const float squared{ x * x };

            return 1.f + squared * (k_fast_cos_0 + squared * (k_fast_cos_1 + squared * k_fast_cos_2));
        }

        [[nodiscard]] inline float sine_kernel_precise(const double value) noexcept
        {
            const double squared{ value * value };

            return static_cast<float>(value + value * squared *
                                      (k_precise_sin_1 + squared *
                                       (k_precise_sin_2 + squared *
                                        (k_precise_sin_3 + squared * k_precise_sin_4))));
        }

        [[nodiscard]] inline float cosine_kernel_precise(const double value) noexcept
        {
            const double squared{ value * value };

            return static_cast<float>(1.0 + squared *
                                      (k_precise_cos_0 + squared *
                                       (k_precise_cos_1 + squared *
                                        (k_precise_cos_2 + squared * k_precise_cos_3))));
        }

        template<bool Precise>
        [[nodiscard]] inline float evaluate_sine(const double value) noexcept
        {
            if constexpr (Precise)
                return sine_kernel_precise(value);
            else
                return sine_kernel_fast(value);
        }

        template<bool Precise>
        [[nodiscard]] inline float evaluate_cosine(const double value) noexcept
        {
            if constexpr (Precise)
                return cosine_kernel_precise(value);
            else
                return cosine_kernel_fast(value);
        }

        template<bool Precise, bool Cosine>
        [[nodiscard]] inline float trig_impl(const float value) noexcept
        {
            if (isnan(value))
                return value;
            if (isinf(value))
                return quiet_nan();

            const std::uint32_t magnitude_bits{ float_to_bits(value) & 0x7FFFFFFFu };
            if (magnitude_bits < 0x39800000u) // |x| < 2^-12
            {
                if constexpr (Cosine)
                    return 1.f;
                else
                    return value;
            }

            const trig_range_t range{ reduce_angle(value) };

            if constexpr (Cosine)
            {
                switch (range.quadrant)
                {
                    case 0: return evaluate_cosine<Precise>(range.reduced);
                    case 1: return -evaluate_sine<Precise>(range.reduced);
                    case 2: return -evaluate_cosine<Precise>(range.reduced);
                    default: return evaluate_sine<Precise>(range.reduced);
                }
            }
            else
            {
                switch (range.quadrant)
                {
                    case 0: return evaluate_sine<Precise>(range.reduced);
                    case 1: return evaluate_cosine<Precise>(range.reduced);
                    case 2: return -evaluate_sine<Precise>(range.reduced);
                    default: return -evaluate_cosine<Precise>(range.reduced);
                }
            }
        }

        template<bool Precise>
        [[nodiscard]] inline float tangent_impl(const float value) noexcept
        {
            if (isnan(value))
                return value;
            if (isinf(value))
                return quiet_nan();

            const std::uint32_t magnitude_bits{ float_to_bits(value) & 0x7FFFFFFFu };

            if (magnitude_bits < 0x39800000u) // |x| < 2^-12
                return value;

            const trig_range_t range{ reduce_angle(value) };
            const float sine{ evaluate_sine<Precise>(range.reduced) };
            const float cosine{ evaluate_cosine<Precise>(range.reduced) };

            if ((range.quadrant & 1) == 0) return sine / cosine;

            return -cosine / sine;
        }
    } // namespace trig_detail

    [[nodiscard]] inline float sin_fast(const float value) noexcept
    {
        return trig_detail::trig_impl<false, false>(value);
    }

    [[nodiscard]] inline float sin_precise(const float value) noexcept
    {
        return trig_detail::trig_impl<true, false>(value);
    }

    [[nodiscard]] inline float cos_fast(const float value) noexcept
    {
        return trig_detail::trig_impl<false, true>(value);
    }

    [[nodiscard]] inline float cos_precise(const float value) noexcept
    {
        return trig_detail::trig_impl<true, true>(value);
    }

    [[nodiscard]] inline float tan_fast(const float value) noexcept
    {
        return trig_detail::tangent_impl<false>(value);
    }

    [[nodiscard]] inline float tan_precise(const float value) noexcept
    {
        return trig_detail::tangent_impl<true>(value);
    }
} // namespace chlm::detail::math
