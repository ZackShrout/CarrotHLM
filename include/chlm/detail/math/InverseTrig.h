//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "MathCommon.h"
#include "Sqrt.h"

namespace chlm::detail::math {
    namespace inverse_trig_detail {
        // NVIDIA Cg reference approximation, documented for <= 6.7e-5 absolute error.
        constexpr float k_fast_c0{ -0.0187293f };
        constexpr float k_fast_c1{ 0.0742610f };
        constexpr float k_fast_c2{ -0.2121144f };
        constexpr float k_fast_c3{ 1.5707288f };

        /*
         * Precise rational coefficients are adapted from the FreeBSD msun acos kernel.
         * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
         * Developed at SunPro. Permission to use, copy, modify, and distribute this
         * software is freely granted, provided that this notice is preserved.
         */
        constexpr double k_precise_half_pi_high{ 1.57079632679489655800 };
        constexpr double k_precise_half_pi_low{ 6.12323399573676603587e-17 };
        constexpr double k_precise_p0{ 1.66666666666666657415e-01 };
        constexpr double k_precise_p1{ -3.25565818622400915405e-01 };
        constexpr double k_precise_p2{ 2.01212532134862925881e-01 };
        constexpr double k_precise_p3{ -4.00555345006794114027e-02 };
        constexpr double k_precise_p4{ 7.91534994289814532176e-04 };
        constexpr double k_precise_p5{ 3.47933107596021167570e-05 };
        constexpr double k_precise_q1{ -2.40339491173441421878e+00 };
        constexpr double k_precise_q2{ 2.02094576023350569471e+00 };
        constexpr double k_precise_q3{ -6.88283971605453293030e-01 };
        constexpr double k_precise_q4{ 7.70381505559019352791e-02 };

        [[nodiscard]] inline double rational_precise(const double squared) noexcept
        {
            const double numerator{
                squared *
                (k_precise_p0 + squared *
                 (k_precise_p1 + squared *
                  (k_precise_p2 + squared *
                   (k_precise_p3 + squared *
                    (k_precise_p4 + squared * k_precise_p5)))))
            };

            const double denominator{
                1.0 + squared *
                (k_precise_q1 + squared *
                 (k_precise_q2 + squared *
                  (k_precise_q3 + squared * k_precise_q4)))
            };

            return numerator / denominator;
        }

        [[nodiscard]] inline double corrected_sqrt(const double value) noexcept
        {
            const double estimate{ static_cast<double>(sqrt_precise(static_cast<float>(value))) };

            return estimate + (value - estimate * estimate) / (2.0 * estimate);
        }
    } // namespace inverse_trig_detail

    [[nodiscard]] inline float acos_fast(const float value) noexcept
    {
        if (isnan(value)) return value;
        if (value > 1.f || value < -1.f) return quiet_nan();
        if (value == 1.f) return 0.f;
        if (value == -1.f) return k_pi;

        const float magnitude{ bits_to_float(float_to_bits(value) & 0x7FFFFFFFu) };

        if (magnitude == 0.f) return k_half_pi;

        float polynomial{ inverse_trig_detail::k_fast_c0 };
        polynomial = polynomial * magnitude + inverse_trig_detail::k_fast_c1;
        polynomial = polynomial * magnitude + inverse_trig_detail::k_fast_c2;
        polynomial = polynomial * magnitude + inverse_trig_detail::k_fast_c3;

        const float approximation{ polynomial * sqrt_fast(1.f - magnitude) };

        return signbit(value) ? k_pi - approximation : approximation;
    }

    [[nodiscard]] inline float acos_precise(const float value) noexcept
    {
        if (isnan(value)) return value;
        if (value > 1.f || value < -1.f) return quiet_nan();
        if (value == 1.f) return 0.f;
        if (value == -1.f) return k_pi;

        const double x{ value };
        const std::uint32_t magnitude_bits{ float_to_bits(value) & 0x7FFFFFFFu };

        if (magnitude_bits < 0x3F000000u)
        {
            if (magnitude_bits <= 0x32800000u) // |x| <= 2^-26
                return static_cast<float>(inverse_trig_detail::k_precise_half_pi_high);

            return static_cast<float>(inverse_trig_detail::k_precise_half_pi_high -
                (x - (inverse_trig_detail::k_precise_half_pi_low -
                    x * inverse_trig_detail::rational_precise(x * x))));
        }

        const double squared{ (1.0 - (signbit(value) ? -x : x)) * 0.5 };
        const double root{ inverse_trig_detail::corrected_sqrt(squared) };
        const double approximation{ root + root * inverse_trig_detail::rational_precise(squared) };

        if (signbit(value))
            return static_cast<float>(2.0 * (inverse_trig_detail::k_precise_half_pi_high -
                (approximation - inverse_trig_detail::k_precise_half_pi_low)));

        return static_cast<float>(2.0 * approximation);
    }
} // namespace chlm::detail::math
