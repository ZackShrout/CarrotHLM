//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "RandomBits.h"

#include <bit>
#include <cassert>
#include <cstdint>
#include <limits>

namespace chlm::detail::random {
    [[nodiscard]] constexpr bool is_finite(const float value) noexcept
    {
        return (std::bit_cast<std::uint32_t>(value) & 0x7F800000u) != 0x7F800000u;
    }

    [[nodiscard]] constexpr std::uint32_t next_uint(
        pcg32_t& engine,
        const std::uint32_t min_inclusive,
        const std::uint32_t max_inclusive) noexcept
    {
        assert(min_inclusive <= max_inclusive);

        const std::uint64_t count{ static_cast<std::uint64_t>(max_inclusive) - min_inclusive + 1u };

        return min_inclusive + bounded_u32(engine, count);
    }

    [[nodiscard]] constexpr int next_int(
        pcg32_t& engine,
        const int min_inclusive,
        const int max_inclusive) noexcept
    {
        static_assert(std::numeric_limits<int>::digits == 31, "rng32 requires a 32-bit int type");
        assert(min_inclusive <= max_inclusive);

        const std::uint64_t count{
            static_cast<std::uint64_t>(static_cast<std::int64_t>(max_inclusive) - min_inclusive) + 1u
        };

        const std::int64_t result{ static_cast<std::int64_t>(min_inclusive) + bounded_u32(engine, count) };

        return static_cast<int>(result);
    }

    [[nodiscard]] constexpr float previous_float(const float value) noexcept
    {
        const std::uint32_t bits{ std::bit_cast<std::uint32_t>(value) };

        if ((bits & 0x7FFFFFFFu) == 0u) return -std::numeric_limits<float>::denorm_min();

        const std::uint32_t previous_bits{ (bits & 0x80000000u) == 0u ? bits - 1u : bits + 1u };

        return std::bit_cast<float>(previous_bits);
    }

    [[nodiscard]] constexpr float next_float(
        pcg32_t& engine,
        const float min_inclusive,
        const float max_exclusive) noexcept
    {
        assert(min_inclusive <= max_exclusive);
        assert(is_finite(min_inclusive) && is_finite(max_exclusive));

        const float unit{ unit_float(engine) };

        if (min_inclusive == max_exclusive) return min_inclusive;

        const double range{ static_cast<double>(max_exclusive) - static_cast<double>(min_inclusive) };
        const float result{ static_cast<float>(static_cast<double>(min_inclusive) + range * unit) };

        return result < max_exclusive ? result : previous_float(max_exclusive);
    }
} // namespace chlm::detail::random
