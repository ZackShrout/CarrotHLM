//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "SimdTraits.h"

#include <array>
#include <bit>
#include <cstdint>

namespace chlm::detail {
    struct simd_scalar
    {
        using float32x4 = std::array<float, 4>;

        static constexpr simd_backend_kind kind{ simd_backend_kind::scalar };
        static constexpr int lane_count{ 4 };

        [[nodiscard]] static constexpr float32x4 set(const float x, const float y, const float z, const float w) noexcept
        {
            return { x, y, z, w };
        }

        [[nodiscard]] static constexpr float32x4 splat(const float value) noexcept
        {
            return { value, value, value, value };
        }

        [[nodiscard]] static constexpr float32x4 load(const float* values) noexcept
        {
            return { values[0], values[1], values[2], values[3] };
        }

        static constexpr void store(float* out, const float32x4& value) noexcept
        {
            out[0] = value[0];
            out[1] = value[1];
            out[2] = value[2];
            out[3] = value[3];
        }

        [[nodiscard]] static constexpr float32x4 add(const float32x4& a, const float32x4& b) noexcept
        {
            return { a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3] };
        }

        [[nodiscard]] static constexpr float32x4 sub(const float32x4& a, const float32x4& b) noexcept
        {
            return { a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3] };
        }

        [[nodiscard]] static constexpr float32x4 mul(const float32x4& a, const float32x4& b) noexcept
        {
            return { a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3] };
        }

        [[nodiscard]] static constexpr float32x4 div(const float32x4& a, const float32x4& b) noexcept
        {
            return { a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3] };
        }

        [[nodiscard]] static constexpr float32x4 bit_and(const float32x4& a, const float32x4& b) noexcept
        {
            return {
                bitwise_and(a[0], b[0]),
                bitwise_and(a[1], b[1]),
                bitwise_and(a[2], b[2]),
                bitwise_and(a[3], b[3])
            };
        }

        [[nodiscard]] static constexpr float32x4 bit_or(const float32x4& a, const float32x4& b) noexcept
        {
            return {
                bitwise_or(a[0], b[0]),
                bitwise_or(a[1], b[1]),
                bitwise_or(a[2], b[2]),
                bitwise_or(a[3], b[3])
            };
        }

        [[nodiscard]] static constexpr float32x4 bit_xor(const float32x4& a, const float32x4& b) noexcept
        {
            return {
                bitwise_xor(a[0], b[0]),
                bitwise_xor(a[1], b[1]),
                bitwise_xor(a[2], b[2]),
                bitwise_xor(a[3], b[3])
            };
        }

        [[nodiscard]] static constexpr float32x4 min(const float32x4& a, const float32x4& b) noexcept
        {
            return {
                a[0] < b[0] ? a[0] : b[0],
                a[1] < b[1] ? a[1] : b[1],
                a[2] < b[2] ? a[2] : b[2],
                a[3] < b[3] ? a[3] : b[3]
            };
        }

        [[nodiscard]] static constexpr float32x4 max(const float32x4& a, const float32x4& b) noexcept
        {
            return {
                a[0] > b[0] ? a[0] : b[0],
                a[1] > b[1] ? a[1] : b[1],
                a[2] > b[2] ? a[2] : b[2],
                a[3] > b[3] ? a[3] : b[3]
            };
        }

        [[nodiscard]] static constexpr float32x4 cmp_eq(const float32x4& a, const float32x4& b) noexcept
        {
            return compare(a, b, [](const float lhs, const float rhs) constexpr { return lhs == rhs; });
        }

        [[nodiscard]] static constexpr float32x4 cmp_neq(const float32x4& a, const float32x4& b) noexcept
        {
            return compare(a, b, [](const float lhs, const float rhs) constexpr { return lhs != rhs; });
        }

        [[nodiscard]] static constexpr float32x4 cmp_lt(const float32x4& a, const float32x4& b) noexcept
        {
            return compare(a, b, [](const float lhs, const float rhs) constexpr { return lhs < rhs; });
        }

        [[nodiscard]] static constexpr float32x4 cmp_lte(const float32x4& a, const float32x4& b) noexcept
        {
            return compare(a, b, [](const float lhs, const float rhs) constexpr { return lhs <= rhs; });
        }

        [[nodiscard]] static constexpr float32x4 cmp_gt(const float32x4& a, const float32x4& b) noexcept
        {
            return compare(a, b, [](const float lhs, const float rhs) constexpr { return lhs > rhs; });
        }

        [[nodiscard]] static constexpr float32x4 cmp_gte(const float32x4& a, const float32x4& b) noexcept
        {
            return compare(a, b, [](const float lhs, const float rhs) constexpr { return lhs >= rhs; });
        }

        [[nodiscard]] static constexpr float32x4 select(const float32x4& mask, const float32x4& if_true, const float32x4& if_false) noexcept
        {
            return {
                mask_allows(mask[0]) ? if_true[0] : if_false[0],
                mask_allows(mask[1]) ? if_true[1] : if_false[1],
                mask_allows(mask[2]) ? if_true[2] : if_false[2],
                mask_allows(mask[3]) ? if_true[3] : if_false[3]
            };
        }

        template<int I0, int I1, int I2, int I3>
        [[nodiscard]] static constexpr float32x4 shuffle(const float32x4& value) noexcept
        {
            static_assert(valid_shuffle_lane<I0>());
            static_assert(valid_shuffle_lane<I1>());
            static_assert(valid_shuffle_lane<I2>());
            static_assert(valid_shuffle_lane<I3>());

            return { value[I0], value[I1], value[I2], value[I3] };
        }

        template<int Lane>
        [[nodiscard]] static constexpr float extract_lane(const float32x4& value) noexcept
        {
            static_assert(Lane >= 0 && Lane < lane_count);
            return value[Lane];
        }

        template<int Lane>
        [[nodiscard]] static constexpr float32x4 replace_lane(float32x4 value, const float lane_value) noexcept
        {
            static_assert(Lane >= 0 && Lane < lane_count);
            value[Lane] = lane_value;
            return value;
        }

    private:
        template<typename Predicate>
        [[nodiscard]] static constexpr float32x4 compare(const float32x4& a, const float32x4& b, Predicate predicate) noexcept
        {
            return {
                predicate(a[0], b[0]) ? mask_true() : mask_false(),
                predicate(a[1], b[1]) ? mask_true() : mask_false(),
                predicate(a[2], b[2]) ? mask_true() : mask_false(),
                predicate(a[3], b[3]) ? mask_true() : mask_false()
            };
        }

        template<int Lane>
        [[nodiscard]] static consteval bool valid_shuffle_lane() noexcept
        {
            return Lane >= 0 && Lane < lane_count;
        }

        [[nodiscard]] static constexpr float mask_true() noexcept
        {
            return std::bit_cast<float>(std::uint32_t{ 0xFFFFFFFFu });
        }

        [[nodiscard]] static constexpr float mask_false() noexcept
        {
            return std::bit_cast<float>(std::uint32_t{ 0u });
        }

        [[nodiscard]] static constexpr bool mask_allows(const float mask_lane) noexcept
        {
            return std::bit_cast<std::uint32_t>(mask_lane) == 0xFFFFFFFFu;
        }

        [[nodiscard]] static constexpr float bitwise_and(const float a, const float b) noexcept
        {
            return std::bit_cast<float>(std::bit_cast<std::uint32_t>(a) & std::bit_cast<std::uint32_t>(b));
        }

        [[nodiscard]] static constexpr float bitwise_or(const float a, const float b) noexcept
        {
            return std::bit_cast<float>(std::bit_cast<std::uint32_t>(a) | std::bit_cast<std::uint32_t>(b));
        }

        [[nodiscard]] static constexpr float bitwise_xor(const float a, const float b) noexcept
        {
            return std::bit_cast<float>(std::bit_cast<std::uint32_t>(a) ^ std::bit_cast<std::uint32_t>(b));
        }
    };
} // namespace chlm::detail
