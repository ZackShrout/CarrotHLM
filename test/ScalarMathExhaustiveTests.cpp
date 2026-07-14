//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <bit>
#include <cstdint>
#include <cmath>
#include <limits>

namespace {
    [[nodiscard]] std::uint32_t ulp_distance(const float lhs, const float rhs)
    {
        const std::uint32_t lhs_bits{ std::bit_cast<std::uint32_t>(lhs) };
        const std::uint32_t rhs_bits{ std::bit_cast<std::uint32_t>(rhs) };
        return lhs_bits > rhs_bits ? lhs_bits - rhs_bits : rhs_bits - lhs_bits;
    }

    void run_trig_oracle_checks(test_context& ctx)
    {
        constexpr float fast_error_limit{ 5e-6f };
        constexpr float precise_error_limit{ 1.5e-7f };
        constexpr std::uint32_t wide_stride{ 0x00010101u };

        float worst_fast_sin_error{ 0.f };
        float worst_precise_sin_error{ 0.f };
        float worst_fast_cos_error{ 0.f };
        float worst_precise_cos_error{ 0.f };
        float worst_fast_sin_input{ 0.f };
        float worst_precise_sin_input{ 0.f };
        float worst_fast_cos_input{ 0.f };
        float worst_precise_cos_input{ 0.f };

        const auto measure = [&](const float angle)
        {
            const float sin_oracle{ std::sin(angle) };
            const float cos_oracle{ std::cos(angle) };
            const float fast_sin_error{ std::abs(chlm::sin(angle) - sin_oracle) };
            const float precise_sin_error{ std::abs(chlm::sin_precise(angle) - sin_oracle) };
            const float fast_cos_error{ std::abs(chlm::cos(angle) - cos_oracle) };
            const float precise_cos_error{ std::abs(chlm::cos_precise(angle) - cos_oracle) };

            if (fast_sin_error > worst_fast_sin_error)
            {
                worst_fast_sin_error = fast_sin_error;
                worst_fast_sin_input = angle;
            }
            if (precise_sin_error > worst_precise_sin_error)
            {
                worst_precise_sin_error = precise_sin_error;
                worst_precise_sin_input = angle;
            }
            if (fast_cos_error > worst_fast_cos_error)
            {
                worst_fast_cos_error = fast_cos_error;
                worst_fast_cos_input = angle;
            }
            if (precise_cos_error > worst_precise_cos_error)
            {
                worst_precise_cos_error = precise_cos_error;
                worst_precise_cos_input = angle;
            }
        };

        for (float angle{ -8192.f }; angle <= 8192.f; angle += 0.0625f)
            measure(angle);

        for (std::uint32_t bits{ 1u }; bits < 0x7F800000u; bits += wide_stride)
        {
            const float angle{ std::bit_cast<float>(bits) };
            measure(angle);
            measure(-angle);
        }

        std::uint32_t random_bits{ 0xC001D00Du };
        for (int sample{ 0 }; sample < 250000; ++sample)
        {
            random_bits = random_bits * 1664525u + 1013904223u;
            const std::uint32_t magnitude_bits{ random_bits & 0x7FFFFFFFu };
            if (magnitude_bits >= 0x7F800000u)
                continue;

            const float angle{ std::bit_cast<float>(magnitude_bits) };
            measure(angle);
            measure(-angle);
        }

        for (int quadrant{ -4096 }; quadrant <= 4096; ++quadrant)
        {
            const float boundary{ static_cast<float>(quadrant) * chlm::half_pi };
            measure(boundary);
            measure(std::nextafter(boundary, -std::numeric_limits<float>::infinity()));
            measure(std::nextafter(boundary, std::numeric_limits<float>::infinity()));
        }

        test_println("  sin worst fast absolute error: {} at {} (bits {})",
                     worst_fast_sin_error, worst_fast_sin_input,
                     std::bit_cast<std::uint32_t>(worst_fast_sin_input));
        test_println("  sin worst precise absolute error: {} at {} (bits {})",
                     worst_precise_sin_error, worst_precise_sin_input,
                     std::bit_cast<std::uint32_t>(worst_precise_sin_input));
        test_println("  cos worst fast absolute error: {} at {} (bits {})",
                     worst_fast_cos_error, worst_fast_cos_input,
                     std::bit_cast<std::uint32_t>(worst_fast_cos_input));
        test_println("  cos worst precise absolute error: {} at {} (bits {})",
                     worst_precise_cos_error, worst_precise_cos_input,
                     std::bit_cast<std::uint32_t>(worst_precise_cos_input));

        ctx.expect(worst_fast_sin_error <= fast_error_limit, "sin fast absolute-error sweep");
        ctx.expect(worst_precise_sin_error <= precise_error_limit,
                   "sin_precise absolute-error sweep");
        ctx.expect(worst_fast_cos_error <= fast_error_limit, "cos fast absolute-error sweep");
        ctx.expect(worst_precise_cos_error <= precise_error_limit,
                   "cos_precise absolute-error sweep");
        ctx.expect(worst_precise_sin_error < worst_fast_sin_error,
                   "sin_precise worst-case improvement");
        ctx.expect(worst_precise_cos_error < worst_fast_cos_error,
                   "cos_precise worst-case improvement");
    }

    void run_sqrt_oracle_checks(test_context& ctx)
    {
        constexpr std::uint32_t sample_stride{ 0x00010101u };
        constexpr std::uint32_t fast_ulp_limit{ 4u };
        constexpr std::uint32_t precise_ulp_limit{ 1u };
        constexpr float fast_relative_error_limit{ 5e-7f };
        constexpr float precise_relative_error_limit{ 1.3e-7f };

        float worst_fast_relative_error{ 0.f };
        float worst_precise_relative_error{ 0.f };
        std::uint32_t worst_fast_ulp{ 0u };
        std::uint32_t worst_precise_ulp{ 0u };
        float worst_fast_ulp_input{ 0.f };
        float worst_precise_ulp_input{ 0.f };
        bool precise_improved_a_sample{ false };

        for (std::uint32_t bits{ 1u }; bits < 0x7F800000u; bits += sample_stride)
        {
            const float value{ std::bit_cast<float>(bits) };
            const float oracle{ std::sqrt(value) };
            const float fast_error{ std::abs(chlm::sqrt(value) - oracle) / oracle };
            const float precise_error{ std::abs(chlm::sqrt_precise(value) - oracle) / oracle };
            const std::uint32_t fast_ulp{ ulp_distance(chlm::sqrt(value), oracle) };
            const std::uint32_t precise_ulp{ ulp_distance(chlm::sqrt_precise(value), oracle) };

            if (fast_error > worst_fast_relative_error)
                worst_fast_relative_error = fast_error;
            if (precise_error > worst_precise_relative_error)
                worst_precise_relative_error = precise_error;
            if (fast_ulp > worst_fast_ulp)
            {
                worst_fast_ulp = fast_ulp;
                worst_fast_ulp_input = value;
            }
            if (precise_ulp > worst_precise_ulp)
            {
                worst_precise_ulp = precise_ulp;
                worst_precise_ulp_input = value;
            }
            if (precise_error < fast_error)
                precise_improved_a_sample = true;
        }

        // Preserve the worst cases found by the full mantissa/exponent-parity verification.
        constexpr std::uint32_t regression_inputs[]{ 0x0008AA52u, 0x40000002u };
        for (const std::uint32_t bits : regression_inputs)
        {
            const float value{ std::bit_cast<float>(bits) };
            const float oracle{ std::sqrt(value) };
            const std::uint32_t fast_ulp{ ulp_distance(chlm::sqrt(value), oracle) };
            const std::uint32_t precise_ulp{ ulp_distance(chlm::sqrt_precise(value), oracle) };

            if (fast_ulp > worst_fast_ulp)
            {
                worst_fast_ulp = fast_ulp;
                worst_fast_ulp_input = value;
            }
            if (precise_ulp > worst_precise_ulp)
            {
                worst_precise_ulp = precise_ulp;
                worst_precise_ulp_input = value;
            }
        }

        test_println("  sqrt worst fast error: {} ULP at {} (bits {})",
                     worst_fast_ulp, worst_fast_ulp_input, std::bit_cast<std::uint32_t>(worst_fast_ulp_input));
        test_println("  sqrt worst precise error: {} ULP at {} (bits {})",
                     worst_precise_ulp, worst_precise_ulp_input, std::bit_cast<std::uint32_t>(worst_precise_ulp_input));

        ctx.expect(worst_fast_relative_error <= fast_relative_error_limit,
                   "sqrt fast relative-error sweep");
        ctx.expect(worst_precise_relative_error <= precise_relative_error_limit,
                   "sqrt_precise relative-error sweep");
        ctx.expect(worst_fast_ulp <= fast_ulp_limit, "sqrt fast ULP sweep");
        ctx.expect(worst_precise_ulp <= precise_ulp_limit, "sqrt_precise ULP sweep");
        ctx.expect(worst_precise_relative_error < worst_fast_relative_error,
                   "sqrt_precise worst-case improvement");
        ctx.expect(precise_improved_a_sample, "sqrt_precise sample improvement");

        constexpr float edge_values[]{
            std::numeric_limits<float>::denorm_min(),
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::max()
        };

        for (const float value : edge_values)
        {
            const float oracle{ std::sqrt(value) };
            ctx.expect(test_almost_equal(chlm::sqrt_precise(value), oracle, oracle * precise_relative_error_limit),
                       "sqrt_precise exponent edge");
        }
    }

    void run_tan_oracle_checks(test_context& ctx)
    {
        constexpr float fast_scaled_error_limit{ 6e-6f };
        constexpr float precise_scaled_error_limit{ 2.5e-7f };

        float worst_fast_error{ 0.f };
        float worst_precise_error{ 0.f };
        float worst_fast_input{ 0.f };
        float worst_precise_input{ 0.f };

        const auto measure = [&](const float angle)
        {
            const float oracle{ std::tan(angle) };
            if (!std::isfinite(oracle))
                return;

            const float scale{ std::abs(oracle) > 1.f ? std::abs(oracle) : 1.f };
            const float fast_error{ std::abs(chlm::tan(angle) - oracle) / scale };
            const float precise_error{ std::abs(chlm::tan_precise(angle) - oracle) / scale };

            if (fast_error > worst_fast_error)
            {
                worst_fast_error = fast_error;
                worst_fast_input = angle;
            }
            if (precise_error > worst_precise_error)
            {
                worst_precise_error = precise_error;
                worst_precise_input = angle;
            }
        };

        for (float angle{ -8192.f }; angle <= 8192.f; angle += 0.0625f)
            measure(angle);

        std::uint32_t random_bits{ 0x7A11CAFEu };
        for (int sample{ 0 }; sample < 250000; ++sample)
        {
            random_bits = random_bits * 1664525u + 1013904223u;
            const std::uint32_t magnitude_bits{ random_bits & 0x7FFFFFFFu };
            if (magnitude_bits >= 0x7F800000u)
                continue;

            const float angle{ std::bit_cast<float>(magnitude_bits) };
            measure(angle);
            measure(-angle);
        }

        for (int pole{ -4095 }; pole <= 4095; pole += 2)
        {
            const float boundary{ static_cast<float>(pole) * chlm::half_pi };
            measure(boundary);
            measure(std::nextafter(boundary, -std::numeric_limits<float>::infinity()));
            measure(std::nextafter(boundary, std::numeric_limits<float>::infinity()));
        }

        test_println("  tan worst fast scaled error: {} at {} (bits {})",
                     worst_fast_error, worst_fast_input,
                     std::bit_cast<std::uint32_t>(worst_fast_input));
        test_println("  tan worst precise scaled error: {} at {} (bits {})",
                     worst_precise_error, worst_precise_input,
                     std::bit_cast<std::uint32_t>(worst_precise_input));

        ctx.expect(worst_fast_error <= fast_scaled_error_limit, "tan fast scaled-error sweep");
        ctx.expect(worst_precise_error <= precise_scaled_error_limit,
                   "tan_precise scaled-error sweep");
        ctx.expect(worst_precise_error < worst_fast_error,
                   "tan_precise worst-case improvement");
    }

    void run_acos_oracle_checks(test_context& ctx)
    {
        constexpr float fast_error_limit{ 7.5e-5f };
        constexpr float precise_error_limit{ 2.5e-7f };

        float worst_fast_error{ 0.f };
        float worst_precise_error{ 0.f };
        float worst_fast_input{ 0.f };
        float worst_precise_input{ 0.f };

        const auto measure = [&](const float value)
        {
            // Endpoints have exact library contracts and are tested separately;
            // platform acosf implementations do not all return the same float for pi.
            if (value == -1.f || value == 1.f)
                return;

            const float oracle{ std::acos(value) };
            const float fast_error{ std::abs(chlm::acos(value) - oracle) };
            const float precise_error{ std::abs(chlm::acos_precise(value) - oracle) };

            if (fast_error > worst_fast_error)
            {
                worst_fast_error = fast_error;
                worst_fast_input = value;
            }
            if (precise_error > worst_precise_error)
            {
                worst_precise_error = precise_error;
                worst_precise_input = value;
            }
        };

        float previous_fast{ chlm::acos(-1.f) };
        float previous_precise{ chlm::acos_precise(-1.f) };
        bool fast_monotonic{ true };
        bool precise_monotonic{ true };
        for (int sample{ -1000000 }; sample <= 1000000; ++sample)
        {
            const float value{ static_cast<float>(sample) / 1000000.f };
            measure(value);

            const float fast{ chlm::acos(value) };
            const float precise{ chlm::acos_precise(value) };
            if (fast > previous_fast)
                fast_monotonic = false;
            if (precise > previous_precise)
                precise_monotonic = false;
            previous_fast = fast;
            previous_precise = precise;
        }

        for (std::uint32_t bits{ 0u }; bits < 0x3F800000u; bits += 0x00001001u)
        {
            const float value{ std::bit_cast<float>(bits) };
            measure(value);
            measure(-value);
        }

        for (std::uint32_t offset{ 0u }; offset < 10000u; ++offset)
        {
            const float value{ std::bit_cast<float>(0x3F800000u - offset) };
            measure(value);
            measure(-value);
        }

        test_println("  acos worst fast absolute error: {} at {} (bits {})",
                     worst_fast_error, worst_fast_input,
                     std::bit_cast<std::uint32_t>(worst_fast_input));
        test_println("  acos worst precise absolute error: {} at {} (bits {})",
                     worst_precise_error, worst_precise_input,
                     std::bit_cast<std::uint32_t>(worst_precise_input));

        ctx.expect(worst_fast_error <= fast_error_limit, "acos fast absolute-error sweep");
        ctx.expect(worst_precise_error <= precise_error_limit,
                   "acos_precise absolute-error sweep");
        ctx.expect(worst_precise_error < worst_fast_error,
                   "acos_precise worst-case improvement");
        ctx.expect(fast_monotonic, "acos monotonicity sweep");
        ctx.expect(precise_monotonic, "acos_precise monotonicity sweep");
    }
}

void run_scalar_math_exhaustive_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Scalar Math Exhaustive Tests");

    run_trig_oracle_checks(ctx);
    run_tan_oracle_checks(ctx);
    run_acos_oracle_checks(ctx);
    run_sqrt_oracle_checks(ctx);
}
