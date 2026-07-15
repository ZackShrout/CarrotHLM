//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <algorithm>
#include <bit>
#include <cstdint>
#include <cmath>
#include <limits>

namespace {
    [[nodiscard]] std::uint32_t ulp_distance(const float lhs, const float rhs)
    {
        const std::uint32_t lhs_bits{ std::bit_cast<std::uint32_t>(lhs) };
        const std::uint32_t rhs_bits{ std::bit_cast<std::uint32_t>(rhs) };
        const auto ordered = [](const std::uint32_t bits)
        {
            return (bits & 0x80000000u) != 0u
                ? 0x80000000u - (bits & 0x7FFFFFFFu)
                : 0x80000000u + bits;
        };
        const std::uint32_t lhs_ordered{ ordered(lhs_bits) };
        const std::uint32_t rhs_ordered{ ordered(rhs_bits) };
        return lhs_ordered > rhs_ordered
            ? lhs_ordered - rhs_ordered
            : rhs_ordered - lhs_ordered;
    }

    void run_classification_checks(test_context& ctx)
    {
        constexpr std::uint32_t mantissas[]{ 0u, 1u, 0x00400000u, 0x007FFFFFu };
        bool classifications_match{ true };
        bool copysign_matches{ true };

        for (std::uint32_t sign{ 0u }; sign <= 1u; ++sign)
        {
            for (std::uint32_t exponent{ 0u }; exponent <= 0xFFu; ++exponent)
            {
                for (const std::uint32_t mantissa : mantissas)
                {
                    const std::uint32_t bits{ sign << 31u | exponent << 23u | mantissa };
                    const float value{ std::bit_cast<float>(bits) };
                    const bool expected_nan{ exponent == 0xFFu && mantissa != 0u };
                    const bool expected_inf{ exponent == 0xFFu && mantissa == 0u };
                    const bool expected_finite{ exponent != 0xFFu };

                    classifications_match = classifications_match &&
                        chlm::isnan(value) == expected_nan &&
                        chlm::isinf(value) == expected_inf &&
                        chlm::isfinite(value) == expected_finite &&
                        chlm::signbit(value) == (sign != 0u);

                    const std::uint32_t positive_bits{
                        std::bit_cast<std::uint32_t>(chlm::copysign(value, 0.f))
                    };
                    const std::uint32_t negative_bits{
                        std::bit_cast<std::uint32_t>(chlm::copysign(value, -0.f))
                    };
                    copysign_matches = copysign_matches &&
                        positive_bits == (bits & 0x7FFFFFFFu) &&
                        negative_bits == (bits | 0x80000000u);
                }
            }
        }

        ctx.expect(classifications_match, "float classification category sweep");
        ctx.expect(copysign_matches, "copysign bit-preservation category sweep");
    }

    void run_rounding_oracle_checks(test_context& ctx)
    {
        bool floor_matches{ true };
        bool ceil_matches{ true };
        bool trunc_matches{ true };
        bool round_matches{ true };
        bool frac_matches{ true };
        bool fmod_matches{ true };

        const auto measure_rounding = [&](const float value)
        {
            if (std::isnan(value)) return;

            floor_matches = floor_matches &&
                std::bit_cast<std::uint32_t>(chlm::floor(value)) ==
                std::bit_cast<std::uint32_t>(std::floor(value));
            ceil_matches = ceil_matches &&
                std::bit_cast<std::uint32_t>(chlm::ceil(value)) ==
                std::bit_cast<std::uint32_t>(std::ceil(value));
            trunc_matches = trunc_matches &&
                std::bit_cast<std::uint32_t>(chlm::trunc(value)) ==
                std::bit_cast<std::uint32_t>(std::trunc(value));
            round_matches = round_matches &&
                std::bit_cast<std::uint32_t>(chlm::round(value)) ==
                std::bit_cast<std::uint32_t>(std::round(value));

            if (std::isfinite(value))
            {
                const float oracle_frac{ value - std::floor(value) };
                frac_matches = frac_matches &&
                    std::bit_cast<std::uint32_t>(chlm::frac(value)) ==
                    std::bit_cast<std::uint32_t>(oracle_frac);
            }
        };

        constexpr std::uint32_t mantissas[]{
            0u, 1u, 0x003FFFFFu, 0x00400000u, 0x00400001u, 0x007FFFFFu
        };
        for (std::uint32_t sign{ 0u }; sign <= 1u; ++sign)
        {
            for (std::uint32_t exponent{ 0u }; exponent <= 0xFFu; ++exponent)
            {
                for (const std::uint32_t mantissa : mantissas)
                    measure_rounding(std::bit_cast<float>(sign << 31u | exponent << 23u | mantissa));
            }
        }

        std::uint32_t random_bits{ 0x721A9E3Du };
        for (int sample{ 0 }; sample < 250000; ++sample)
        {
            random_bits = random_bits * 1664525u + 1013904223u;
            measure_rounding(std::bit_cast<float>(random_bits));
        }

        for (int integer{ -4096 }; integer <= 4096; ++integer)
        {
            const float halfway{ static_cast<float>(integer) + .5f };
            measure_rounding(halfway);
            measure_rounding(std::nextafter(halfway, -std::numeric_limits<float>::infinity()));
            measure_rounding(std::nextafter(halfway, std::numeric_limits<float>::infinity()));
        }

        std::uint32_t dividend_bits{ 0x4D0DCAFEu };
        std::uint32_t divisor_bits{ 0x3F123456u };
        for (int sample{ 0 }; sample < 100000; ++sample)
        {
            dividend_bits = dividend_bits * 1664525u + 1013904223u;
            divisor_bits = divisor_bits * 22695477u + 1u;
            const float dividend{ std::bit_cast<float>(dividend_bits) };
            const float divisor{ std::bit_cast<float>(divisor_bits) };

            if (!std::isfinite(dividend) || !std::isfinite(divisor) || divisor == 0.f)
                continue;

            fmod_matches = fmod_matches &&
                std::bit_cast<std::uint32_t>(chlm::fmod(dividend, divisor)) ==
                std::bit_cast<std::uint32_t>(std::fmod(dividend, divisor));
        }

        ctx.expect(floor_matches, "floor exact oracle sweep");
        ctx.expect(ceil_matches, "ceil exact oracle sweep");
        ctx.expect(trunc_matches, "trunc exact oracle sweep");
        ctx.expect(round_matches, "round exact oracle sweep");
        ctx.expect(frac_matches, "frac exact oracle sweep");
        ctx.expect(fmod_matches, "fmod exact oracle sweep");
    }

    void run_base2_oracle_checks(test_context& ctx)
    {
        constexpr float fast_exp2_relative_error_limit{ 5e-7f };
        constexpr std::uint32_t fast_exp2_ulp_limit{ 4u };
        constexpr std::uint32_t precise_exp2_ulp_limit{ 1u };
        constexpr std::uint32_t fast_log2_ulp_limit{ 4u };
        constexpr std::uint32_t precise_log2_ulp_limit{ 1u };

        bool exact_powers_match{ true };
        for (int exponent{ -149 }; exponent <= 127; ++exponent)
        {
            const float expected{
                exponent >= -126
                    ? std::bit_cast<float>(static_cast<std::uint32_t>(exponent + 127) << 23u)
                    : std::bit_cast<float>(1u << static_cast<unsigned>(exponent + 149))
            };
            exact_powers_match = exact_powers_match &&
                std::bit_cast<std::uint32_t>(chlm::exp2(static_cast<float>(exponent))) ==
                    std::bit_cast<std::uint32_t>(expected) &&
                std::bit_cast<std::uint32_t>(chlm::exp2_precise(static_cast<float>(exponent))) ==
                    std::bit_cast<std::uint32_t>(expected) &&
                chlm::log2(expected) == static_cast<float>(exponent) &&
                chlm::log2_precise(expected) == static_cast<float>(exponent);
        }

        float worst_fast_exp2_relative_error{ 0.f };
        std::uint32_t worst_fast_exp2_ulp{ 0u };
        std::uint32_t worst_precise_exp2_ulp{ 0u };
        float worst_fast_exp2_input{ 0.f };
        float worst_fast_exp2_ulp_input{ 0.f };
        float worst_precise_exp2_input{ 0.f };
        bool fast_exp2_monotonic{ true };
        bool precise_exp2_monotonic{ true };
        float previous_fast_exp2{ 0.f };
        float previous_precise_exp2{ 0.f };

        const auto measure_exp2 = [&](const float value)
        {
            const float oracle{ static_cast<float>(std::exp2(static_cast<double>(value))) };
            const float fast{ chlm::exp2(value) };
            const float precise{ chlm::exp2_precise(value) };

            if (std::isfinite(oracle) && oracle != 0.f)
            {
                if (oracle >= std::numeric_limits<float>::min())
                {
                    const float relative_error{ std::abs(fast - oracle) / oracle };
                    if (relative_error > worst_fast_exp2_relative_error)
                    {
                        worst_fast_exp2_relative_error = relative_error;
                        worst_fast_exp2_input = value;
                    }
                }

                const std::uint32_t fast_ulp{ ulp_distance(fast, oracle) };
                if (fast_ulp > worst_fast_exp2_ulp)
                {
                    worst_fast_exp2_ulp = fast_ulp;
                    worst_fast_exp2_ulp_input = value;
                }

                const std::uint32_t precise_ulp{ ulp_distance(precise, oracle) };
                if (precise_ulp > worst_precise_exp2_ulp)
                {
                    worst_precise_exp2_ulp = precise_ulp;
                    worst_precise_exp2_input = value;
                }
            }
        };

        for (int sample{ -614399 }; sample <= 524287; ++sample)
        {
            const float value{ static_cast<float>(sample) / 4096.f };
            measure_exp2(value);
            const float fast{ chlm::exp2(value) };
            const float precise{ chlm::exp2_precise(value) };
            if (fast < previous_fast_exp2)
                fast_exp2_monotonic = false;
            if (precise < previous_precise_exp2)
                precise_exp2_monotonic = false;
            previous_fast_exp2 = fast;
            previous_precise_exp2 = precise;
        }

        for (int exponent{ -150 }; exponent <= 128; ++exponent)
        {
            const float boundary{ static_cast<float>(exponent) };
            measure_exp2(boundary);
            measure_exp2(std::nextafter(boundary, -std::numeric_limits<float>::infinity()));
            measure_exp2(std::nextafter(boundary, std::numeric_limits<float>::infinity()));
        }

        std::uint32_t worst_fast_log2_ulp{ 0u };
        std::uint32_t worst_precise_log2_ulp{ 0u };
        float worst_fast_log2_input{ 0.f };
        float worst_precise_log2_input{ 0.f };
        bool fast_log2_monotonic{ true };
        bool precise_log2_monotonic{ true };
        float previous_fast_log2{ -std::numeric_limits<float>::infinity() };
        float previous_precise_log2{ -std::numeric_limits<float>::infinity() };

        const auto measure_log2 = [&](const float value)
        {
            const float oracle{ static_cast<float>(std::log2(static_cast<double>(value))) };
            const float fast{ chlm::log2(value) };
            const float precise{ chlm::log2_precise(value) };
            const std::uint32_t fast_ulp{ ulp_distance(fast, oracle) };
            const std::uint32_t precise_ulp{ ulp_distance(precise, oracle) };

            if (fast_ulp > worst_fast_log2_ulp)
            {
                worst_fast_log2_ulp = fast_ulp;
                worst_fast_log2_input = value;
            }
            if (precise_ulp > worst_precise_log2_ulp)
            {
                worst_precise_log2_ulp = precise_ulp;
                worst_precise_log2_input = value;
            }

            if (fast < previous_fast_log2)
                fast_log2_monotonic = false;
            if (precise < previous_precise_log2)
                precise_log2_monotonic = false;
            previous_fast_log2 = fast;
            previous_precise_log2 = precise;
        };

        constexpr std::uint32_t log_stride{ 0x00001001u };
        for (std::uint32_t bits{ 1u }; bits < 0x7F800000u; bits += log_stride)
            measure_log2(std::bit_cast<float>(bits));
        measure_log2(std::numeric_limits<float>::max());

        test_println("  exp2 worst fast relative error: {} at {} (bits {})",
                     worst_fast_exp2_relative_error, worst_fast_exp2_input,
                     std::bit_cast<std::uint32_t>(worst_fast_exp2_input));
        test_println("  exp2 worst fast error: {} ULP at {} (bits {})",
                     worst_fast_exp2_ulp, worst_fast_exp2_ulp_input,
                     std::bit_cast<std::uint32_t>(worst_fast_exp2_ulp_input));
        test_println("  exp2 worst precise error: {} ULP at {} (bits {})",
                     worst_precise_exp2_ulp, worst_precise_exp2_input,
                     std::bit_cast<std::uint32_t>(worst_precise_exp2_input));
        test_println("  log2 worst fast error: {} ULP at {} (bits {})",
                     worst_fast_log2_ulp, worst_fast_log2_input,
                     std::bit_cast<std::uint32_t>(worst_fast_log2_input));
        test_println("  log2 worst precise error: {} ULP at {} (bits {})",
                     worst_precise_log2_ulp, worst_precise_log2_input,
                     std::bit_cast<std::uint32_t>(worst_precise_log2_input));

        ctx.expect(exact_powers_match, "exp2/log2 exact power-of-two sweep");
        ctx.expect(worst_fast_exp2_relative_error <= fast_exp2_relative_error_limit,
                   "exp2 fast relative-error sweep");
        ctx.expect(worst_fast_exp2_ulp <= fast_exp2_ulp_limit, "exp2 fast ULP sweep");
        ctx.expect(worst_precise_exp2_ulp <= precise_exp2_ulp_limit,
                   "exp2_precise ULP sweep");
        ctx.expect(worst_fast_log2_ulp <= fast_log2_ulp_limit, "log2 fast ULP sweep");
        ctx.expect(worst_precise_log2_ulp <= precise_log2_ulp_limit,
                   "log2_precise ULP sweep");
        ctx.expect(worst_precise_exp2_ulp < worst_fast_exp2_ulp,
                   "exp2_precise worst-case improvement");
        ctx.expect(worst_precise_log2_ulp < worst_fast_log2_ulp,
                   "log2_precise worst-case improvement");
        ctx.expect(fast_exp2_monotonic, "exp2 monotonicity sweep");
        ctx.expect(precise_exp2_monotonic, "exp2_precise monotonicity sweep");
        ctx.expect(fast_log2_monotonic, "log2 monotonicity sweep");
        ctx.expect(precise_log2_monotonic, "log2_precise monotonicity sweep");
    }

    void run_natural_exponential_oracle_checks(test_context& ctx)
    {
        constexpr float fast_exp_relative_error_limit{ 5e-7f };
        constexpr std::uint32_t fast_exp_ulp_limit{ 4u };
        constexpr std::uint32_t precise_exp_ulp_limit{ 1u };
        constexpr std::uint32_t fast_log_ulp_limit{ 4u };
        constexpr std::uint32_t precise_log_ulp_limit{ 1u };

        float worst_fast_exp_relative_error{ 0.f };
        std::uint32_t worst_fast_exp_ulp{ 0u };
        std::uint32_t worst_precise_exp_ulp{ 0u };
        float worst_fast_exp_input{ 0.f };
        float worst_fast_exp_ulp_input{ 0.f };
        float worst_precise_exp_input{ 0.f };
        bool fast_exp_monotonic{ true };
        bool precise_exp_monotonic{ true };
        float previous_fast_exp{ 0.f };
        float previous_precise_exp{ 0.f };

        const auto measure_exp = [&](const float value)
        {
            const float oracle{ static_cast<float>(std::exp(static_cast<double>(value))) };
            const float fast{ chlm::exp(value) };
            const float precise{ chlm::exp_precise(value) };

            if (std::isfinite(oracle) && oracle != 0.f)
            {
                if (oracle >= std::numeric_limits<float>::min())
                {
                    const float relative_error{ std::abs(fast - oracle) / oracle };
                    if (relative_error > worst_fast_exp_relative_error)
                    {
                        worst_fast_exp_relative_error = relative_error;
                        worst_fast_exp_input = value;
                    }
                }

                const std::uint32_t fast_ulp{ ulp_distance(fast, oracle) };
                const std::uint32_t precise_ulp{ ulp_distance(precise, oracle) };
                if (fast_ulp > worst_fast_exp_ulp)
                {
                    worst_fast_exp_ulp = fast_ulp;
                    worst_fast_exp_ulp_input = value;
                }
                if (precise_ulp > worst_precise_exp_ulp)
                {
                    worst_precise_exp_ulp = precise_ulp;
                    worst_precise_exp_input = value;
                }
            }
        };

        for (int sample{ -425983 }; sample <= 364543; ++sample)
        {
            const float value{ static_cast<float>(sample) / 4096.f };
            measure_exp(value);
            const float fast{ chlm::exp(value) };
            const float precise{ chlm::exp_precise(value) };
            if (fast < previous_fast_exp)
                fast_exp_monotonic = false;
            if (precise < previous_precise_exp)
                precise_exp_monotonic = false;
            previous_fast_exp = fast;
            previous_precise_exp = precise;
        }

        constexpr float exp_boundaries[]{ -104.f, -103.f, 0.f, 88.f, 89.f };
        for (const float boundary : exp_boundaries)
        {
            measure_exp(boundary);
            measure_exp(std::nextafter(boundary, -std::numeric_limits<float>::infinity()));
            measure_exp(std::nextafter(boundary, std::numeric_limits<float>::infinity()));
        }

        std::uint32_t worst_fast_log_ulp{ 0u };
        std::uint32_t worst_precise_log_ulp{ 0u };
        float worst_fast_log_input{ 0.f };
        float worst_precise_log_input{ 0.f };
        bool fast_log_monotonic{ true };
        bool precise_log_monotonic{ true };
        float previous_fast_log{ -std::numeric_limits<float>::infinity() };
        float previous_precise_log{ -std::numeric_limits<float>::infinity() };

        constexpr std::uint32_t log_stride{ 0x00001001u };
        for (std::uint32_t bits{ 1u }; bits < 0x7F800000u; bits += log_stride)
        {
            const float value{ std::bit_cast<float>(bits) };
            const float oracle{ static_cast<float>(std::log(static_cast<double>(value))) };
            const float fast{ chlm::log(value) };
            const float precise{ chlm::log_precise(value) };
            const std::uint32_t fast_ulp{ ulp_distance(fast, oracle) };
            const std::uint32_t precise_ulp{ ulp_distance(precise, oracle) };

            if (fast_ulp > worst_fast_log_ulp)
            {
                worst_fast_log_ulp = fast_ulp;
                worst_fast_log_input = value;
            }
            if (precise_ulp > worst_precise_log_ulp)
            {
                worst_precise_log_ulp = precise_ulp;
                worst_precise_log_input = value;
            }
            if (fast < previous_fast_log)
                fast_log_monotonic = false;
            if (precise < previous_precise_log)
                precise_log_monotonic = false;
            previous_fast_log = fast;
            previous_precise_log = precise;
        }

        test_println("  exp worst fast relative error: {} at {} (bits {})",
                     worst_fast_exp_relative_error, worst_fast_exp_input,
                     std::bit_cast<std::uint32_t>(worst_fast_exp_input));
        test_println("  exp worst fast error: {} ULP at {} (bits {})",
                     worst_fast_exp_ulp, worst_fast_exp_ulp_input,
                     std::bit_cast<std::uint32_t>(worst_fast_exp_ulp_input));
        test_println("  exp worst precise error: {} ULP at {} (bits {})",
                     worst_precise_exp_ulp, worst_precise_exp_input,
                     std::bit_cast<std::uint32_t>(worst_precise_exp_input));
        test_println("  log worst fast error: {} ULP at {} (bits {})",
                     worst_fast_log_ulp, worst_fast_log_input,
                     std::bit_cast<std::uint32_t>(worst_fast_log_input));
        test_println("  log worst precise error: {} ULP at {} (bits {})",
                     worst_precise_log_ulp, worst_precise_log_input,
                     std::bit_cast<std::uint32_t>(worst_precise_log_input));

        ctx.expect(worst_fast_exp_relative_error <= fast_exp_relative_error_limit,
                   "exp fast relative-error sweep");
        ctx.expect(worst_fast_exp_ulp <= fast_exp_ulp_limit, "exp fast ULP sweep");
        ctx.expect(worst_precise_exp_ulp <= precise_exp_ulp_limit,
                   "exp_precise ULP sweep");
        ctx.expect(worst_fast_log_ulp <= fast_log_ulp_limit, "log fast ULP sweep");
        ctx.expect(worst_precise_log_ulp <= precise_log_ulp_limit,
                   "log_precise ULP sweep");
        ctx.expect(worst_precise_exp_ulp < worst_fast_exp_ulp,
                   "exp_precise worst-case improvement");
        ctx.expect(worst_precise_log_ulp < worst_fast_log_ulp,
                   "log_precise worst-case improvement");
        ctx.expect(fast_exp_monotonic, "exp monotonicity sweep");
        ctx.expect(precise_exp_monotonic, "exp_precise monotonicity sweep");
        ctx.expect(fast_log_monotonic, "log monotonicity sweep");
        ctx.expect(precise_log_monotonic, "log_precise monotonicity sweep");

        bool round_trip_matches{ true };
        for (int sample{ -1000 }; sample <= 1000; ++sample)
        {
            const float value{ static_cast<float>(sample) * .01f };
            round_trip_matches = round_trip_matches &&
                std::abs(chlm::log_precise(chlm::exp_precise(value)) - value) <= 2e-6f;
        }
        ctx.expect(round_trip_matches, "log_precise/exp_precise round-trip sweep");
    }

    void run_power_oracle_checks(test_context& ctx)
    {
        constexpr float fast_relative_error_limit{ 1.5e-6f };
        constexpr float precise_relative_error_limit{ 5e-7f };
        constexpr std::uint32_t fast_subnormal_ulp_limit{ 4u };
        constexpr std::uint32_t precise_subnormal_ulp_limit{ 1u };

        float worst_fast_relative_error{ 0.f };
        float worst_precise_relative_error{ 0.f };
        float worst_fast_base{ 0.f };
        float worst_fast_exponent{ 0.f };
        float worst_precise_base{ 0.f };
        float worst_precise_exponent{ 0.f };
        std::uint32_t worst_fast_subnormal_ulp{ 0u };
        std::uint32_t worst_precise_subnormal_ulp{ 0u };
        bool result_classes_match{ true };

        const auto measure = [&](const float base, const float exponent)
        {
            const float oracle{
                static_cast<float>(std::pow(static_cast<double>(base), static_cast<double>(exponent)))
            };
            const float fast{ chlm::pow(base, exponent) };
            const float precise{ chlm::pow_precise(base, exponent) };

            if (std::isnan(oracle))
            {
                result_classes_match = result_classes_match &&
                    std::isnan(fast) && std::isnan(precise);
                return;
            }
            if (std::isinf(oracle))
            {
                result_classes_match = result_classes_match &&
                    std::isinf(fast) && std::isinf(precise) &&
                    std::signbit(fast) == std::signbit(oracle) &&
                    std::signbit(precise) == std::signbit(oracle);
                return;
            }
            if (oracle == 0.f)
            {
                result_classes_match = result_classes_match &&
                    fast == 0.f && precise == 0.f &&
                    std::signbit(fast) == std::signbit(oracle) &&
                    std::signbit(precise) == std::signbit(oracle);
                return;
            }

            if (std::abs(oracle) >= std::numeric_limits<float>::min())
            {
                const float fast_relative_error{ std::abs(fast - oracle) / std::abs(oracle) };
                const float precise_relative_error{ std::abs(precise - oracle) / std::abs(oracle) };

                if (fast_relative_error > worst_fast_relative_error)
                {
                    worst_fast_relative_error = fast_relative_error;
                    worst_fast_base = base;
                    worst_fast_exponent = exponent;
                }
                if (precise_relative_error > worst_precise_relative_error)
                {
                    worst_precise_relative_error = precise_relative_error;
                    worst_precise_base = base;
                    worst_precise_exponent = exponent;
                }
            }
            else
            {
                const std::uint32_t fast_ulp{ ulp_distance(fast, oracle) };
                const std::uint32_t precise_ulp{ ulp_distance(precise, oracle) };
                if (fast_ulp > worst_fast_subnormal_ulp)
                    worst_fast_subnormal_ulp = fast_ulp;
                if (precise_ulp > worst_precise_subnormal_ulp)
                    worst_precise_subnormal_ulp = precise_ulp;

                result_classes_match = result_classes_match &&
                    std::isfinite(fast) && std::isfinite(precise) &&
                    std::signbit(fast) == std::signbit(oracle) &&
                    std::signbit(precise) == std::signbit(oracle);
            }
        };

        std::uint32_t base_bits{ 0x3F123456u };
        std::uint32_t exponent_bits{ 0xBADC0FFEu };
        for (int sample{ 0 }; sample < 500000; ++sample)
        {
            base_bits = base_bits * 1664525u + 1013904223u;
            exponent_bits = exponent_bits * 22695477u + 1u;
            const std::uint32_t magnitude_bits{ base_bits & 0x7FFFFFFFu };
            if (magnitude_bits == 0u || magnitude_bits >= 0x7F800000u)
                continue;

            const float base{ std::bit_cast<float>(magnitude_bits) };
            const int scaled_exponent{ static_cast<int>(exponent_bits % 262145u) - 131072 };
            const float exponent{ static_cast<float>(scaled_exponent) / 2048.f };
            measure(base, exponent);
        }

        for (int base_sample{ 1 }; base_sample <= 4096; ++base_sample)
        {
            const float base{ static_cast<float>(base_sample) / 512.f };
            for (int exponent_sample{ -128 }; exponent_sample <= 128; ++exponent_sample)
                measure(base, static_cast<float>(exponent_sample) * .125f);
        }

        for (int base_sample{ 1 }; base_sample <= 4096; base_sample += 7)
        {
            const float base{ -static_cast<float>(base_sample) / 256.f };
            for (int exponent{ -31 }; exponent <= 31; ++exponent)
                measure(base, static_cast<float>(exponent));
        }

        const float above_one{ std::nextafter(1.f, 2.f) };
        const float below_one{ std::nextafter(1.f, 0.f) };
        constexpr float large_exponents[]{
            -500000000.f, -100000000.f, -10000000.f, -100000.f,
            100000.f, 10000000.f, 100000000.f, 500000000.f
        };
        for (const float exponent : large_exponents)
        {
            measure(above_one, exponent);
            measure(below_one, exponent);
            measure(-above_one, exponent);
            measure(-below_one, exponent);
        }

        test_println("  pow worst fast relative error: {} at ({}, {})",
                     worst_fast_relative_error, worst_fast_base, worst_fast_exponent);
        test_println("  pow worst precise relative error: {} at ({}, {})",
                     worst_precise_relative_error, worst_precise_base, worst_precise_exponent);
        test_println("  pow worst subnormal error: {} ULP fast, {} ULP precise",
                     worst_fast_subnormal_ulp, worst_precise_subnormal_ulp);

        ctx.expect(result_classes_match, "pow result-class and sign sweep");
        ctx.expect(worst_fast_relative_error <= fast_relative_error_limit,
                   "pow fast relative-error sweep");
        ctx.expect(worst_precise_relative_error <= precise_relative_error_limit,
                   "pow_precise relative-error sweep");
        ctx.expect(worst_fast_subnormal_ulp <= fast_subnormal_ulp_limit,
                   "pow fast subnormal ULP sweep");
        ctx.expect(worst_precise_subnormal_ulp <= precise_subnormal_ulp_limit,
                   "pow_precise subnormal ULP sweep");
        ctx.expect(worst_precise_relative_error < worst_fast_relative_error,
                   "pow_precise worst-case improvement");
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

    void run_rsqrt_oracle_checks(test_context& ctx)
    {
        constexpr std::uint32_t sample_stride{ 0x00010101u };
        constexpr float fast_relative_error_limit{ 5e-6f };
        constexpr std::uint32_t precise_ulp_limit{ 1u };

        float worst_fast_relative_error{ 0.f };
        std::uint32_t worst_precise_ulp{ 0u };
        float worst_fast_input{ 0.f };
        float worst_precise_input{ 0.f };
        bool precise_improved_a_sample{ false };

        const auto measure = [&](const float value)
        {
            const float oracle{ static_cast<float>(1.0 / std::sqrt(static_cast<double>(value))) };
            const float fast{ chlm::rsqrt(value) };
            const float precise{ chlm::rsqrt_precise(value) };
            const float fast_relative_error{ std::abs(fast - oracle) / oracle };
            const std::uint32_t fast_ulp{ ulp_distance(fast, oracle) };
            const std::uint32_t precise_ulp{ ulp_distance(precise, oracle) };

            if (fast_relative_error > worst_fast_relative_error)
            {
                worst_fast_relative_error = fast_relative_error;
                worst_fast_input = value;
            }
            if (precise_ulp > worst_precise_ulp)
            {
                worst_precise_ulp = precise_ulp;
                worst_precise_input = value;
            }
            if (precise_ulp < fast_ulp)
                precise_improved_a_sample = true;
        };

        for (std::uint32_t bits{ 1u }; bits < 0x7F800000u; bits += sample_stride)
            measure(std::bit_cast<float>(bits));

        for (std::uint32_t bits{ 1u }; bits < 0x00800000u; bits += 257u)
            measure(std::bit_cast<float>(bits));

        constexpr float edge_values[]{
            std::numeric_limits<float>::denorm_min(),
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::max()
        };
        for (const float value : edge_values)
            measure(value);

        test_println("  rsqrt worst fast relative error: {} at {} (bits {})",
                     worst_fast_relative_error, worst_fast_input,
                     std::bit_cast<std::uint32_t>(worst_fast_input));
        test_println("  rsqrt worst precise error: {} ULP at {} (bits {})",
                     worst_precise_ulp, worst_precise_input,
                     std::bit_cast<std::uint32_t>(worst_precise_input));

        ctx.expect(worst_fast_relative_error <= fast_relative_error_limit,
                   "rsqrt fast relative-error sweep");
        ctx.expect(worst_precise_ulp <= precise_ulp_limit, "rsqrt_precise ULP sweep");
        ctx.expect(precise_improved_a_sample, "rsqrt_precise sample improvement");
    }

    void run_sin_cos_oracle_checks(test_context& ctx)
    {
        constexpr float fast_error_limit{ 5e-6f };
        constexpr float precise_error_limit{ 1.5e-7f };
        constexpr std::uint32_t wide_stride{ 0x00010101u };

        float worst_fast_error{ 0.f };
        float worst_precise_error{ 0.f };
        float worst_fast_input{ 0.f };
        float worst_precise_input{ 0.f };
        bool fast_matches_individual{ true };
        bool precise_matches_individual{ true };

        const auto measure = [&](const float angle)
        {
            const chlm::sin_cos_result fast{ chlm::sin_cos(angle) };
            const chlm::sin_cos_result precise{ chlm::sin_cos_precise(angle) };
            const float sine_oracle{ std::sin(angle) };
            const float cosine_oracle{ std::cos(angle) };
            const float fast_error{
                std::max(std::abs(fast.sine - sine_oracle),
                         std::abs(fast.cosine - cosine_oracle))
            };
            const float precise_error{
                std::max(std::abs(precise.sine - sine_oracle),
                         std::abs(precise.cosine - cosine_oracle))
            };

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

            fast_matches_individual = fast_matches_individual &&
                std::bit_cast<std::uint32_t>(fast.sine) ==
                    std::bit_cast<std::uint32_t>(chlm::sin(angle)) &&
                std::bit_cast<std::uint32_t>(fast.cosine) ==
                    std::bit_cast<std::uint32_t>(chlm::cos(angle));
            precise_matches_individual = precise_matches_individual &&
                std::bit_cast<std::uint32_t>(precise.sine) ==
                    std::bit_cast<std::uint32_t>(chlm::sin_precise(angle)) &&
                std::bit_cast<std::uint32_t>(precise.cosine) ==
                    std::bit_cast<std::uint32_t>(chlm::cos_precise(angle));
        };

        for (float angle{ -8192.f }; angle <= 8192.f; angle += .0625f)
            measure(angle);
        for (std::uint32_t bits{ 1u }; bits < 0x7F800000u; bits += wide_stride)
        {
            const float angle{ std::bit_cast<float>(bits) };
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

        test_println("  sin_cos worst fast absolute error: {} at {} (bits {})",
                     worst_fast_error, worst_fast_input,
                     std::bit_cast<std::uint32_t>(worst_fast_input));
        test_println("  sin_cos worst precise absolute error: {} at {} (bits {})",
                     worst_precise_error, worst_precise_input,
                     std::bit_cast<std::uint32_t>(worst_precise_input));

        ctx.expect(worst_fast_error <= fast_error_limit, "sin_cos fast absolute-error sweep");
        ctx.expect(worst_precise_error <= precise_error_limit,
                   "sin_cos_precise absolute-error sweep");
        ctx.expect(fast_matches_individual, "sin_cos matches individual functions");
        ctx.expect(precise_matches_individual, "sin_cos_precise matches individual functions");
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

    void run_inverse_trig_oracle_checks(test_context& ctx)
    {
        constexpr float fast_error_limit{ 7.5e-5f };
        constexpr float fast_atan_error_limit{ 6e-6f };
        constexpr float precise_error_limit{ 2.5e-7f };

        float worst_fast_asin_error{ 0.f };
        float worst_precise_asin_error{ 0.f };
        float worst_fast_atan_error{ 0.f };
        float worst_precise_atan_error{ 0.f };
        float worst_fast_atan2_error{ 0.f };
        float worst_precise_atan2_error{ 0.f };
        float worst_fast_asin_input{ 0.f };
        float worst_precise_asin_input{ 0.f };
        float worst_fast_atan_input{ 0.f };
        float worst_precise_atan_input{ 0.f };
        float worst_fast_atan2_y{ 0.f };
        float worst_fast_atan2_x{ 0.f };
        float worst_precise_atan2_y{ 0.f };
        float worst_precise_atan2_x{ 0.f };

        const auto measure_asin = [&](const float value)
        {
            const float oracle{ std::asin(value) };
            const float fast_error{ std::abs(chlm::asin(value) - oracle) };
            const float precise_error{ std::abs(chlm::asin_precise(value) - oracle) };

            if (fast_error > worst_fast_asin_error)
            {
                worst_fast_asin_error = fast_error;
                worst_fast_asin_input = value;
            }
            if (precise_error > worst_precise_asin_error)
            {
                worst_precise_asin_error = precise_error;
                worst_precise_asin_input = value;
            }
        };

        float previous_fast_asin{ chlm::asin(-1.f) };
        float previous_precise_asin{ chlm::asin_precise(-1.f) };
        bool fast_asin_monotonic{ true };
        bool precise_asin_monotonic{ true };
        for (int sample{ -1000000 }; sample <= 1000000; ++sample)
        {
            const float value{ static_cast<float>(sample) / 1000000.f };
            measure_asin(value);

            const float fast{ chlm::asin(value) };
            const float precise{ chlm::asin_precise(value) };
            if (fast < previous_fast_asin)
                fast_asin_monotonic = false;
            if (precise < previous_precise_asin)
                precise_asin_monotonic = false;
            previous_fast_asin = fast;
            previous_precise_asin = precise;
        }

        for (std::uint32_t bits{ 0u }; bits < 0x3F800000u; bits += 0x00001001u)
        {
            const float value{ std::bit_cast<float>(bits) };
            measure_asin(value);
            measure_asin(-value);
        }
        for (std::uint32_t offset{ 0u }; offset < 10000u; ++offset)
        {
            const float value{ std::bit_cast<float>(0x3F800000u - offset) };
            measure_asin(value);
            measure_asin(-value);
        }

        const auto measure_atan = [&](const float value)
        {
            const float oracle{ std::atan(value) };
            const float fast_error{ std::abs(chlm::atan(value) - oracle) };
            const float precise_error{ std::abs(chlm::atan_precise(value) - oracle) };

            if (fast_error > worst_fast_atan_error)
            {
                worst_fast_atan_error = fast_error;
                worst_fast_atan_input = value;
            }
            if (precise_error > worst_precise_atan_error)
            {
                worst_precise_atan_error = precise_error;
                worst_precise_atan_input = value;
            }
        };

        constexpr std::uint32_t wide_stride{ 0x00010101u };
        for (std::uint32_t bits{ 0u }; bits < 0x7F800000u; bits += wide_stride)
        {
            const float value{ std::bit_cast<float>(bits) };
            measure_atan(value);
            measure_atan(-value);
        }

        float previous_fast_atan{ chlm::atan(-1024.f) };
        float previous_precise_atan{ chlm::atan_precise(-1024.f) };
        bool fast_atan_monotonic{ true };
        bool precise_atan_monotonic{ true };
        for (int sample{ -1048576 }; sample <= 1048576; ++sample)
        {
            const float value{ static_cast<float>(sample) / 1024.f };
            const float fast{ chlm::atan(value) };
            const float precise{ chlm::atan_precise(value) };
            if (fast < previous_fast_atan)
                fast_atan_monotonic = false;
            if (precise < previous_precise_atan)
                precise_atan_monotonic = false;
            previous_fast_atan = fast;
            previous_precise_atan = precise;
        }

        const auto measure_atan2 = [&](const float y, const float x)
        {
            if (y == 0.f || x == 0.f)
                return;

            const float oracle{ std::atan2(y, x) };
            const float fast_error{ std::abs(chlm::atan2(y, x) - oracle) };
            const float precise_error{ std::abs(chlm::atan2_precise(y, x) - oracle) };

            if (fast_error > worst_fast_atan2_error)
            {
                worst_fast_atan2_error = fast_error;
                worst_fast_atan2_y = y;
                worst_fast_atan2_x = x;
            }
            if (precise_error > worst_precise_atan2_error)
            {
                worst_precise_atan2_error = precise_error;
                worst_precise_atan2_y = y;
                worst_precise_atan2_x = x;
            }
        };

        std::uint32_t y_bits{ 0xA7A21E5Du };
        std::uint32_t x_bits{ 0x4F1BBCDCu };
        for (int sample{ 0 }; sample < 500000; ++sample)
        {
            y_bits = y_bits * 1664525u + 1013904223u;
            x_bits = x_bits * 22695477u + 1u;
            const float y{ std::bit_cast<float>(y_bits) };
            const float x{ std::bit_cast<float>(x_bits) };
            if (std::isfinite(y) && std::isfinite(x))
                measure_atan2(y, x);
        }

        for (int sample{ -4096 }; sample <= 4096; ++sample)
        {
            const float value{ static_cast<float>(sample) / 1024.f };
            measure_atan2(value, 1.f);
            measure_atan2(value, -1.f);
            measure_atan2(1.f, value);
            measure_atan2(-1.f, value);
        }

        test_println("  asin worst fast absolute error: {} at {} (bits {})",
                     worst_fast_asin_error, worst_fast_asin_input,
                     std::bit_cast<std::uint32_t>(worst_fast_asin_input));
        test_println("  asin worst precise absolute error: {} at {} (bits {})",
                     worst_precise_asin_error, worst_precise_asin_input,
                     std::bit_cast<std::uint32_t>(worst_precise_asin_input));
        test_println("  atan worst fast absolute error: {} at {} (bits {})",
                     worst_fast_atan_error, worst_fast_atan_input,
                     std::bit_cast<std::uint32_t>(worst_fast_atan_input));
        test_println("  atan worst precise absolute error: {} at {} (bits {})",
                     worst_precise_atan_error, worst_precise_atan_input,
                     std::bit_cast<std::uint32_t>(worst_precise_atan_input));
        test_println("  atan2 worst fast absolute error: {} at ({}, {})",
                     worst_fast_atan2_error, worst_fast_atan2_y, worst_fast_atan2_x);
        test_println("  atan2 worst precise absolute error: {} at ({}, {})",
                     worst_precise_atan2_error, worst_precise_atan2_y, worst_precise_atan2_x);

        ctx.expect(worst_fast_asin_error <= fast_error_limit, "asin fast absolute-error sweep");
        ctx.expect(worst_precise_asin_error <= precise_error_limit,
                   "asin_precise absolute-error sweep");
        ctx.expect(worst_fast_atan_error <= fast_atan_error_limit,
                   "atan fast absolute-error sweep");
        ctx.expect(worst_precise_atan_error <= precise_error_limit,
                   "atan_precise absolute-error sweep");
        ctx.expect(worst_fast_atan2_error <= fast_atan_error_limit,
                   "atan2 fast absolute-error sweep");
        ctx.expect(worst_precise_atan2_error <= precise_error_limit,
                   "atan2_precise absolute-error sweep");
        ctx.expect(worst_precise_asin_error < worst_fast_asin_error,
                   "asin_precise worst-case improvement");
        ctx.expect(worst_precise_atan_error < worst_fast_atan_error,
                   "atan_precise worst-case improvement");
        ctx.expect(worst_precise_atan2_error < worst_fast_atan2_error,
                   "atan2_precise worst-case improvement");
        ctx.expect(fast_asin_monotonic, "asin monotonicity sweep");
        ctx.expect(precise_asin_monotonic, "asin_precise monotonicity sweep");
        ctx.expect(fast_atan_monotonic, "atan monotonicity sweep");
        ctx.expect(precise_atan_monotonic, "atan_precise monotonicity sweep");
    }
}

void run_scalar_math_exhaustive_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Scalar Math Exhaustive Tests");

    run_classification_checks(ctx);
    run_rounding_oracle_checks(ctx);
    run_base2_oracle_checks(ctx);
    run_natural_exponential_oracle_checks(ctx);
    run_power_oracle_checks(ctx);
    run_trig_oracle_checks(ctx);
    run_tan_oracle_checks(ctx);
    run_acos_oracle_checks(ctx);
    run_inverse_trig_oracle_checks(ctx);
    run_sqrt_oracle_checks(ctx);
    run_rsqrt_oracle_checks(ctx);
    run_sin_cos_oracle_checks(ctx);
}
