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

        // NVIDIA Cg atan2 reduced-domain polynomial.
        constexpr float k_atan_fast_c0{ -0.013480470f };
        constexpr float k_atan_fast_c1{ 0.057477314f };
        constexpr float k_atan_fast_c2{ -0.121239071f };
        constexpr float k_atan_fast_c3{ 0.195635925f };
        constexpr float k_atan_fast_c4{ -0.332994597f };
        constexpr float k_atan_fast_c5{ 0.999995630f };

        constexpr double k_precise_quarter_pi{ 0.78539816339744830962 };
        constexpr double k_precise_tan_eighth_pi{ 0.41421356237309504880 };
        constexpr double k_precise_tan_three_eighths_pi{ 2.41421356237309504880 };

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

        [[nodiscard]] inline float atan_kernel_fast(const float value) noexcept
        {
            const float squared{ value * value };
            float polynomial{ k_atan_fast_c0 };
            polynomial = polynomial * squared + k_atan_fast_c1;
            polynomial = polynomial * squared + k_atan_fast_c2;
            polynomial = polynomial * squared + k_atan_fast_c3;
            polynomial = polynomial * squared + k_atan_fast_c4;
            polynomial = polynomial * squared + k_atan_fast_c5;

            return polynomial * value;
        }

        [[nodiscard]] inline double atan_kernel_precise(const double value) noexcept
        {
            const double squared{ value * value };
            const double polynomial{
                1.0 + squared *
                (-1.0 / 3.0 + squared *
                 (1.0 / 5.0 + squared *
                  (-1.0 / 7.0 + squared *
                   (1.0 / 9.0 + squared *
                    (-1.0 / 11.0 + squared *
                     (1.0 / 13.0 + squared *
                      (-1.0 / 15.0 + squared *
                       (1.0 / 17.0 + squared * (-1.0 / 19.0)))))))))
            };

            return value * polynomial;
        }

        [[nodiscard]] inline float atan_positive_fast(const float value) noexcept
        {
            if (value > 1.f) return k_half_pi - atan_kernel_fast(1.f / value);

            return atan_kernel_fast(value);
        }

        [[nodiscard]] inline float atan_positive_precise(const float value) noexcept
        {
            const double x{ value };
            double base{ 0.0 };
            double reduced{ x };

            if (x > k_precise_tan_three_eighths_pi)
            {
                base = k_precise_half_pi_high;
                reduced = -1.0 / x;
            }
            else if (x > k_precise_tan_eighth_pi)
            {
                base = k_precise_quarter_pi;
                reduced = (x - 1.0) / (x + 1.0);
            }

            return static_cast<float>(base + atan_kernel_precise(reduced));
        }

        template<bool Precise>
        [[nodiscard]] inline float atan2_impl(const float y, const float x) noexcept
        {
            if (isnan(y)) return y;
            if (isnan(x)) return x;

            constexpr std::uint32_t magnitude_mask{ 0x7FFFFFFFu };
            const std::uint32_t y_bits{ float_to_bits(y) };
            const std::uint32_t x_bits{ float_to_bits(x) };
            const std::uint32_t y_magnitude_bits{ y_bits & magnitude_mask };
            const std::uint32_t x_magnitude_bits{ x_bits & magnitude_mask };
            const bool y_negative{ y_bits >> 31u != 0u };
            const bool x_negative{ x_bits >> 31u != 0u };

            if (y_magnitude_bits == 0u)
                return x_negative ? copysign(k_pi, y) : y;

            if (x_magnitude_bits == 0u) return copysign(k_half_pi, y);

            const bool y_infinite{ y_magnitude_bits == 0x7F800000u };
            const bool x_infinite{ x_magnitude_bits == 0x7F800000u };

            if (y_infinite && x_infinite)
            {
                const float angle{ x_negative ? 3.f * k_pi * .25f : k_pi * .25f };
                return y_negative ? -angle : angle;
            }

            if (x_infinite)
                return x_negative ? copysign(k_pi, y) : copysign(0.f, y);

            if (y_infinite) return copysign(k_half_pi, y);

            const float y_magnitude{ bits_to_float(y_magnitude_bits) };
            const float x_magnitude{ bits_to_float(x_magnitude_bits) };
            const bool steep{ y_magnitude > x_magnitude };
            const float ratio{ steep ? x_magnitude / y_magnitude : y_magnitude / x_magnitude };
            float angle;

            if constexpr (Precise)
                angle = atan_positive_precise(ratio);
            else
                angle = atan_kernel_fast(ratio);

            if (steep)
                angle = k_half_pi - angle;
            if (x_negative)
                angle = k_pi - angle;

            return y_negative ? -angle : angle;
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

    [[nodiscard]] inline float asin_fast(const float value) noexcept
    {
        if (isnan(value)) return value;
        if (value > 1.f || value < -1.f) return quiet_nan();
        if (value == 0.f) return value;
        if (value == 1.f) return k_half_pi;
        if (value == -1.f) return -k_half_pi;

        const float magnitude{ bits_to_float(float_to_bits(value) & 0x7FFFFFFFu) };
        float polynomial{ inverse_trig_detail::k_fast_c0 };
        polynomial = polynomial * magnitude + inverse_trig_detail::k_fast_c1;
        polynomial = polynomial * magnitude + inverse_trig_detail::k_fast_c2;
        polynomial = polynomial * magnitude + inverse_trig_detail::k_fast_c3;
        const float approximation{ k_half_pi - polynomial * sqrt_fast(1.f - magnitude) };

        return signbit(value) ? -approximation : approximation;
    }

    [[nodiscard]] inline float asin_precise(const float value) noexcept
    {
        if (isnan(value)) return value;
        if (value > 1.f || value < -1.f) return quiet_nan();
        if (value == 0.f) return value;
        if (value == 1.f) return k_half_pi;
        if (value == -1.f) return -k_half_pi;

        const double x{ value };
        const double magnitude{ signbit(value) ? -x : x };
        double approximation;

        if (magnitude < 0.5)
        {
            approximation = magnitude + magnitude * inverse_trig_detail::rational_precise(magnitude * magnitude);
        }
        else
        {
            const double squared{ (1.0 - magnitude) * 0.5 };
            const double root{ inverse_trig_detail::corrected_sqrt(squared) };
            approximation = inverse_trig_detail::k_precise_half_pi_high -
                            2.0 * (root + root * inverse_trig_detail::rational_precise(squared));
        }

        const float result{ static_cast<float>(approximation) };

        return signbit(value) ? -result : result;
    }

    [[nodiscard]] inline float atan_fast(const float value) noexcept
    {
        if (isnan(value)) return value;
        if (value == 0.f) return value;
        if (isinf(value)) return copysign(k_half_pi, value);

        const float magnitude{ bits_to_float(float_to_bits(value) & 0x7FFFFFFFu) };
        const float result{ inverse_trig_detail::atan_positive_fast(magnitude) };

        return signbit(value) ? -result : result;
    }

    [[nodiscard]] inline float atan_precise(const float value) noexcept
    {
        if (isnan(value)) return value;
        if (value == 0.f) return value;
        if (isinf(value)) return copysign(k_half_pi, value);

        const float magnitude{ bits_to_float(float_to_bits(value) & 0x7FFFFFFFu) };
        const float result{ inverse_trig_detail::atan_positive_precise(magnitude) };

        return signbit(value) ? -result : result;
    }

    [[nodiscard]] inline float atan2_fast(const float y, const float x) noexcept
    {
        return inverse_trig_detail::atan2_impl<false>(y, x);
    }

    [[nodiscard]] inline float atan2_precise(const float y, const float x) noexcept
    {
        return inverse_trig_detail::atan2_impl<true>(y, x);
    }
} // namespace chlm::detail::math
