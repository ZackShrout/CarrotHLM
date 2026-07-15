//
// Random.h - Deterministic random number generation for CarrotHLM
//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "detail/random/Pcg32.h"
#include "detail/random/RandomBits.h"
#include "detail/random/RandomDistributions.h"

#include <cassert>
#include <cstdint>

/**
 * @file Random.h
 * @brief Deterministic random generation and geometric sampling for games and simulation.
 *
 * The module provides the explicit-state PCG32 generator chlm::rng32, scalar
 * distributions with defined interval semantics, and uniform vector and geometry
 * sampling helpers. It does not use hidden global state and is not suitable for
 * cryptographic or security-sensitive purposes.
 */

namespace chlm {
    /**
     * @brief Complete serializable state of an rng32 generator.
     *
     * Preserve both fields exactly when serializing a generator for replay,
     * rollback, save/load, or deterministic debugging. Restoring a state with
     * rng32::set_state reproduces the same future sequence without reseeding.
     * This type is trivially copyable and contains no pointers or external state.
     */
    struct rng32_state
    {
    public:
        /** @brief Current 64-bit state of the PCG linear congruential generator. */
        std::uint64_t state;

        /**
         * @brief Encoded PCG stream selector.
         *
         * A state captured from rng32 always contains an odd value here. The
         * encoded selector must be preserved exactly rather than treated as the
         * original stream seed passed to rng32::seed.
         */
        std::uint64_t stream;

        /**
         * @brief Compares two serialized generator states exactly.
         *
         * @param other State to compare with this state.
         * @return true when both the generator state and encoded stream match.
         */
        [[nodiscard]] constexpr bool operator==(const rng32_state& other) const noexcept = default;
    };

    /**
     * @brief Small deterministic PCG32 generator for games, tools, and simulation.
     *
     * The generator owns one 64-bit PCG state and one encoded 64-bit stream
     * selector. Separate generator objects do not share state and require no
     * synchronization when each object is owned by one thread or subsystem.
     *
     * Raw output and all scalar distributions are implemented by CarrotHLM so
     * that the same generator type, seed or restored state, sequence of calls,
     * and CarrotHLM version produce the same sequence across supported platforms.
     * Integer ranges are inclusive at both ends. Floating-point ranges are
     * half-open and require finite ordered bounds.
     *
     * @note The cross-platform determinism guarantee covers raw rng32 output and
     * CarrotHLM-owned distribution and sampling functions. If the raw output is
     * adapted for use with standard-library distributions or shuffle algorithms,
     * the resulting values or ordering are not guaranteed to match across standard
     * library implementations or versions. Use CarrotHLM-owned operations or a
     * project-owned, versioned mapping when exact replay portability is required.
     *
     * @warning rng32 is a non-cryptographic generator. Do not use it for keys,
     * tokens, passwords, or any security-sensitive purpose.
     */
    struct rng32
    {
    public:
        /**
         * @brief Constructs a generator with the deterministic default seed.
         *
         * This is equivalent to constructing with state seed `0` and the fixed
         * default stream seed. It does not read a clock, operating-system entropy,
         * or hidden global state. Every default-constructed rng32 begins with the
         * same sequence.
         */
        constexpr rng32() noexcept
        {
            seed(0u);
        }

        /**
         * @brief Constructs a generator from one deterministic state seed.
         *
         * The fixed default stream is used, so equal seed values produce equal
         * sequences. Use the two-seed constructor when independent subsystem or
         * thread streams are required.
         *
         * @param seed_value Initial 64-bit state seed.
         */
        explicit constexpr rng32(const std::uint64_t seed_value) noexcept
        {
            seed(seed_value);
        }

        /**
         * @brief Constructs a generator from deterministic state and stream seeds.
         *
         * The stream seed selects an independent PCG sequence. PCG encodes the
         * lower 63 bits of @p stream_seed as an odd increment, so stream seeds
         * that differ only in their highest bit select the same stream.
         *
         * @param state_seed Initial 64-bit state seed.
         * @param stream_seed Stream-selection seed; its lower 63 bits are significant.
         */
        constexpr rng32(const std::uint64_t state_seed, const std::uint64_t stream_seed) noexcept
        {
            seed(state_seed, stream_seed);
        }

        /**
         * @brief Reseeds the generator on the fixed default stream.
         *
         * Reseeding discards the current sequence position. Calling this function
         * with the same value used by the one-seed constructor restores that
         * constructor's initial sequence.
         *
         * @param seed_value New 64-bit state seed.
         */
        constexpr void seed(const std::uint64_t seed_value) noexcept
        {
            seed(seed_value, k_default_stream_seed);
        }

        /**
         * @brief Reseeds the generator and selects a deterministic stream.
         *
         * Reseeding discards the current sequence position. PCG encodes the lower
         * 63 bits of @p stream_seed, so values that differ only in their highest
         * bit select the same stream.
         *
         * @param state_seed New 64-bit state seed.
         * @param stream_seed Stream-selection seed; its lower 63 bits are significant.
         */
        constexpr void seed(const std::uint64_t state_seed, const std::uint64_t stream_seed) noexcept
        {
            _engine.seed(state_seed, stream_seed);
        }

        /**
         * @brief Generates the next raw 32 random bits.
         *
         * This is the canonical PCG-XSH-RR output and advances the generator once.
         *
         * @return Uniformly distributed value over the full uint32_t domain.
         */
        [[nodiscard]] constexpr std::uint32_t next_u32() noexcept
        {
            return _engine.next_u32();
        }

        /**
         * @brief Generates the next raw 64 random bits.
         *
         * The high and low halves come from two consecutive next_u32 calls, in
         * that order. The generator therefore advances twice.
         *
         * @return Uniformly distributed value over the full uint64_t domain.
         */
        [[nodiscard]] constexpr std::uint64_t next_u64() noexcept
        {
            return detail::random::next_u64(_engine);
        }

        /**
         * @brief Generates an unbiased unsigned integer in an inclusive range.
         *
         * Rejection sampling avoids the modulo bias produced when the range width
         * does not divide the full 32-bit output domain. The full range from `0`
         * through `UINT32_MAX` is supported. A degenerate range returns its sole
         * value and still advances the generator.
         *
         * @param min_inclusive Smallest value that may be returned.
         * @param max_inclusive Largest value that may be returned.
         * @return Uniformly distributed value in [`min_inclusive`, `max_inclusive`].
         * @pre @p min_inclusive must be less than or equal to @p max_inclusive.
         * Invalid bounds trigger an assertion in debug builds.
         */
        [[nodiscard]] constexpr std::uint32_t next_uint(
            const std::uint32_t min_inclusive,
            const std::uint32_t max_inclusive) noexcept
        {
            return detail::random::next_uint(_engine, min_inclusive, max_inclusive);
        }

        /**
         * @brief Generates an unbiased signed integer in an inclusive range.
         *
         * Rejection sampling avoids modulo bias. The full range from `INT_MIN`
         * through `INT_MAX` is supported on the required 32-bit int type. A
         * degenerate range returns its sole value and still advances the generator.
         *
         * @param min_inclusive Smallest value that may be returned.
         * @param max_inclusive Largest value that may be returned.
         * @return Uniformly distributed value in [`min_inclusive`, `max_inclusive`].
         * @pre @p min_inclusive must be less than or equal to @p max_inclusive.
         * Invalid bounds trigger an assertion in debug builds.
         */
        [[nodiscard]] constexpr int next_int(
            const int min_inclusive,
            const int max_inclusive) noexcept
        {
            return detail::random::next_int(_engine, min_inclusive, max_inclusive);
        }

        /**
         * @brief Generates a uniformly distributed unit-interval float.
         *
         * The value is constructed from 24 random bits, matching the precision of
         * the float significand. The generator advances once.
         *
         * @return Uniformly distributed value in the half-open interval [0, 1).
         */
        [[nodiscard]] constexpr float next_float() noexcept
        {
            return detail::random::unit_float(_engine);
        }

        /**
         * @brief Generates a uniformly distributed float in a half-open range.
         *
         * The result includes @p min_inclusive and excludes @p max_exclusive.
         * Rounding is explicitly prevented from producing the upper bound. A
         * degenerate range returns its sole value and still advances the generator.
         * The distribution has at most 2^24 distinct source samples.
         *
         * @param min_inclusive Finite lower bound that may be returned.
         * @param max_exclusive Finite upper bound that is never returned unless
         * the range is degenerate.
         * @return Uniformly distributed value in [`min_inclusive`, `max_exclusive`),
         * or @p min_inclusive when both bounds are equal.
         * @pre Both bounds must be finite, and @p min_inclusive must be less than
         * or equal to @p max_exclusive. Invalid bounds trigger an assertion in
         * debug builds.
         */
        [[nodiscard]] constexpr float next_float(
            const float min_inclusive,
            const float max_exclusive) noexcept
        {
            return detail::random::next_float(_engine, min_inclusive, max_exclusive);
        }

        /**
         * @brief Generates an unbiased random boolean.
         *
         * The result uses one bit from a fresh next_u32 output and advances the
         * generator once.
         *
         * @return true or false with equal probability.
         */
        [[nodiscard]] constexpr bool next_bool() noexcept
        {
            return (next_u32() & 1u) != 0u;
        }

        /**
         * @brief Performs a Bernoulli trial with a specified success probability.
         *
         * The generator advances once for every valid call, including probabilities
         * of `0` and `1`. Therefore chance(0) always returns false and chance(1)
         * always returns true without changing sequence-consumption semantics.
         *
         * @param probability Probability of returning true in the inclusive range [0, 1].
         * @return true with probability @p probability; otherwise false.
         * @pre @p probability must be finite and in [0, 1]. Invalid values trigger
         * an assertion in debug builds.
         */
        [[nodiscard]] constexpr bool chance(const float probability) noexcept
        {
            assert(probability >= 0.f && probability <= 1.f);
            return next_float() < probability;
        }

        /**
         * @brief Captures the complete current generator state.
         *
         * This operation does not advance the generator. Restoring the returned
         * value with set_state reproduces all subsequent outputs exactly when the
         * same sequence of random API calls is made.
         *
         * @return Trivially copyable state containing the sequence position and stream.
         */
        [[nodiscard]] constexpr rng32_state state() const noexcept
        {
            return { _engine.state(), _engine.stream() };
        }

        /**
         * @brief Restores a previously captured generator state.
         *
         * Restoration replaces both the sequence position and stream without
         * advancing or reseeding the generator.
         *
         * @param generator_state State previously returned by rng32::state.
         * @pre @p generator_state must contain an odd encoded stream selector.
         * Invalid state triggers an assertion in debug builds.
         */
        constexpr void set_state(const rng32_state& generator_state) noexcept
        {
            assert((generator_state.stream & 1u) != 0u);
            _engine.set_state(generator_state.state, generator_state.stream);
        }

    private:
        static constexpr std::uint64_t k_default_stream_seed{ 54u };

        detail::random::pcg32_t _engine{};
    };
} // namespace chlm

#include "detail/random/RandomGeometry.h"
