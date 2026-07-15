//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Pcg32.h"

#include <cstdint>

namespace chlm::detail::random {
    [[nodiscard]] constexpr std::uint64_t next_u64(pcg32_t& engine) noexcept
    {
        const std::uint64_t high{ engine.next_u32() };
        const std::uint64_t low{ engine.next_u32() };

        return high << 32u | low;
    }

    [[nodiscard]] constexpr std::uint32_t bounded_u32(pcg32_t& engine, const std::uint64_t count) noexcept
    {
        constexpr std::uint64_t full_u32_range{ std::uint64_t{ 1u } << 32u };

        if (count == full_u32_range) return engine.next_u32();

        const std::uint32_t bound{ static_cast<std::uint32_t>(count) };
        const std::uint32_t threshold{ (0u - bound) % bound };

        while (true)
        {
            const std::uint32_t value{ engine.next_u32() };

            if (value >= threshold) return value % bound;
        }
    }

    [[nodiscard]] constexpr float unit_float(pcg32_t& engine) noexcept
    {
        constexpr float inverse_24_bit_range{ 1.f / 16777216.f };

        return static_cast<float>(engine.next_u32() >> 8u) * inverse_24_bit_range;
    }
} // namespace chlm::detail::random
