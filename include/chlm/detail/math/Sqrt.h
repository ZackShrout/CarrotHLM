//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "MathCommon.h"

namespace chlm::detail::math {
    namespace sqrt_detail {
        constexpr std::uint32_t k_sign_mask{ 0x80000000u };
        constexpr std::uint32_t k_magnitude_mask{ 0x7FFFFFFFu };

        // NOTE: Minimax-tuned for the two-step float Newton kernel below.
        constexpr std::uint32_t k_sqrt_seed_bias{ 0x1FBAB000u };

        constexpr float k_subnormal_scale{ 16777216.f }; // 2^24
        constexpr float k_subnormal_result_scale{ 0.000244140625f }; // 2^-12

        [[nodiscard]] inline float initial_estimate(const float value) noexcept
        {
            return bits_to_float((float_to_bits(value) >> 1u) + k_sqrt_seed_bias);
        }

        [[nodiscard]] inline float newton_step(const float value, const float estimate) noexcept
        {
            return 0.5f * (estimate + value / estimate);
        }

        [[nodiscard]] inline float sqrt_finite_positive_fast(const float value) noexcept
        {
            float estimate{ initial_estimate(value) };
            estimate = newton_step(value, estimate);

            return newton_step(value, estimate);
        }

        [[nodiscard]] inline float sqrt_finite_positive_precise(const float value) noexcept
        {
            const float estimate{ sqrt_finite_positive_fast(value) };

            return newton_step(value, estimate);
        }

        template<bool Precise>
        [[nodiscard]] inline float sqrt_impl(const float value) noexcept
        {
            const std::uint32_t bits{ float_to_bits(value) };
            const std::uint32_t magnitude{ bits & k_magnitude_mask };

            // Returning the input preserves signed zero and NaN payloads.
            if (magnitude == 0u || isnan(value)) return value;

            if ((bits & k_sign_mask) != 0u) return quiet_nan();
            if (isinf(value)) return value;

            const bool subnormal{ exponent_bits(value) == 0u };
            const float scaled_value{ subnormal ? value * k_subnormal_scale : value };

            float result;

            if constexpr (Precise)
                result = sqrt_finite_positive_precise(scaled_value);
            else
                result = sqrt_finite_positive_fast(scaled_value);

            return subnormal ? result * k_subnormal_result_scale : result;
        }
    } // namespace sqrt_detail

    [[nodiscard]] inline float sqrt_fast(const float value) noexcept
    {
        return sqrt_detail::sqrt_impl<false>(value);
    }

    [[nodiscard]] inline float sqrt_precise(const float value) noexcept
    {
        return sqrt_detail::sqrt_impl<true>(value);
    }
} // namespace chlm::detail::math
