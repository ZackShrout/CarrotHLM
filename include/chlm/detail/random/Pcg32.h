//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include <cstdint>

namespace chlm::detail::random {
    struct pcg32_t
    {
    public:
        constexpr pcg32_t() noexcept = default;

        constexpr pcg32_t(const std::uint64_t state_seed, const std::uint64_t stream_seed) noexcept
        {
            seed(state_seed, stream_seed);
        }

        constexpr void seed(const std::uint64_t state_seed, const std::uint64_t stream_seed) noexcept
        {
            _state = 0u;
            _stream = stream_seed << 1u | 1u;
            static_cast<void>(next_u32());
            _state += state_seed;
            static_cast<void>(next_u32());
        }

        [[nodiscard]] constexpr std::uint32_t next_u32() noexcept
        {
            constexpr std::uint64_t multiplier{ 6364136223846793005u };

            const std::uint64_t previous_state{ _state };
            _state = previous_state * multiplier + _stream;

            const std::uint32_t xorshifted{
                static_cast<std::uint32_t>((previous_state >> 18u ^ previous_state) >> 27u)
            };
            const std::uint32_t rotation{ static_cast<std::uint32_t>(previous_state >> 59u) };

            return xorshifted >> rotation | xorshifted << (0u - rotation & 31u);
        }

        [[nodiscard]] constexpr std::uint64_t state() const noexcept
        {
            return _state;
        }

        [[nodiscard]] constexpr std::uint64_t stream() const noexcept
        {
            return _stream;
        }

        constexpr void set_state(const std::uint64_t state, const std::uint64_t stream) noexcept
        {
            _state = state;
            _stream = stream;
        }

    private:
        std::uint64_t _state{ 0u };
        std::uint64_t _stream{ 1u };
    };
} // namespace chlm::detail::random
