//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "SimdTraits.h"

#include <arm_neon.h>
#include <cstdint>

namespace chlm::detail {
    struct simd_neon
    {
        using float32x4 = float32x4_t;

        static constexpr simd_backend_kind kind{ simd_backend_kind::neon };
        static constexpr int lane_count{ 4 };

        [[nodiscard]] static inline float32x4 set(const float x, const float y, const float z, const float w) noexcept
        {
            const float lanes[4]{ x, y, z, w };
            return vld1q_f32(lanes);
        }

        [[nodiscard]] static inline float32x4 splat(const float value) noexcept
        {
            return vdupq_n_f32(value);
        }

        [[nodiscard]] static inline float32x4 load(const float* values) noexcept
        {
            return vld1q_f32(values);
        }

        static inline void store(float* out, const float32x4 value) noexcept
        {
            vst1q_f32(out, value);
        }

        [[nodiscard]] static inline float32x4 add(const float32x4 a, const float32x4 b) noexcept
        {
            return vaddq_f32(a, b);
        }

        [[nodiscard]] static inline float32x4 sub(const float32x4 a, const float32x4 b) noexcept
        {
            return vsubq_f32(a, b);
        }

        [[nodiscard]] static inline float32x4 mul(const float32x4 a, const float32x4 b) noexcept
        {
            return vmulq_f32(a, b);
        }

        [[nodiscard]] static inline float32x4 div(const float32x4 a, const float32x4 b) noexcept
        {
            return vdivq_f32(a, b);
        }

        [[nodiscard]] static inline float32x4 bit_and(const float32x4 a, const float32x4 b) noexcept
        {
            return vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(a), vreinterpretq_u32_f32(b)));
        }

        [[nodiscard]] static inline float32x4 bit_or(const float32x4 a, const float32x4 b) noexcept
        {
            return vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(a), vreinterpretq_u32_f32(b)));
        }

        [[nodiscard]] static inline float32x4 bit_xor(const float32x4 a, const float32x4 b) noexcept
        {
            return vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(a), vreinterpretq_u32_f32(b)));
        }

        [[nodiscard]] static inline float32x4 min(const float32x4 a, const float32x4 b) noexcept
        {
            return vminq_f32(a, b);
        }

        [[nodiscard]] static inline float32x4 max(const float32x4 a, const float32x4 b) noexcept
        {
            return vmaxq_f32(a, b);
        }

        [[nodiscard]] static inline float32x4 cmp_eq(const float32x4 a, const float32x4 b) noexcept
        {
            return vreinterpretq_f32_u32(vceqq_f32(a, b));
        }

        [[nodiscard]] static inline float32x4 cmp_neq(const float32x4 a, const float32x4 b) noexcept
        {
            return vreinterpretq_f32_u32(vmvnq_u32(vceqq_f32(a, b)));
        }

        [[nodiscard]] static inline float32x4 cmp_lt(const float32x4 a, const float32x4 b) noexcept
        {
            return vreinterpretq_f32_u32(vcltq_f32(a, b));
        }

        [[nodiscard]] static inline float32x4 cmp_lte(const float32x4 a, const float32x4 b) noexcept
        {
            return vreinterpretq_f32_u32(vcleq_f32(a, b));
        }

        [[nodiscard]] static inline float32x4 cmp_gt(const float32x4 a, const float32x4 b) noexcept
        {
            return vreinterpretq_f32_u32(vcgtq_f32(a, b));
        }

        [[nodiscard]] static inline float32x4 cmp_gte(const float32x4 a, const float32x4 b) noexcept
        {
            return vreinterpretq_f32_u32(vcgeq_f32(a, b));
        }

        [[nodiscard]] static inline float32x4 select(const float32x4 mask, const float32x4 if_true, const float32x4 if_false) noexcept
        {
            return vbslq_f32(vreinterpretq_u32_f32(mask), if_true, if_false);
        }

        template<int I0, int I1, int I2, int I3>
        [[nodiscard]] static inline float32x4 shuffle(const float32x4 value) noexcept
        {
            static_assert(I0 >= 0 && I0 < lane_count);
            static_assert(I1 >= 0 && I1 < lane_count);
            static_assert(I2 >= 0 && I2 < lane_count);
            static_assert(I3 >= 0 && I3 < lane_count);

            const uint8x16_t control{
                static_cast<std::uint8_t>(I0 * 4 + 0), static_cast<std::uint8_t>(I0 * 4 + 1), static_cast<std::uint8_t>(I0 * 4 + 2), static_cast<std::uint8_t>(I0 * 4 + 3),
                static_cast<std::uint8_t>(I1 * 4 + 0), static_cast<std::uint8_t>(I1 * 4 + 1), static_cast<std::uint8_t>(I1 * 4 + 2), static_cast<std::uint8_t>(I1 * 4 + 3),
                static_cast<std::uint8_t>(I2 * 4 + 0), static_cast<std::uint8_t>(I2 * 4 + 1), static_cast<std::uint8_t>(I2 * 4 + 2), static_cast<std::uint8_t>(I2 * 4 + 3),
                static_cast<std::uint8_t>(I3 * 4 + 0), static_cast<std::uint8_t>(I3 * 4 + 1), static_cast<std::uint8_t>(I3 * 4 + 2), static_cast<std::uint8_t>(I3 * 4 + 3)
            };

            return vreinterpretq_f32_u8(vqtbl1q_u8(vreinterpretq_u8_f32(value), control));
        }

        template<int Lane>
        [[nodiscard]] static inline float extract_lane(const float32x4 value) noexcept
        {
            static_assert(Lane >= 0 && Lane < lane_count);
            return vgetq_lane_f32(value, Lane);
        }

        template<int Lane>
        [[nodiscard]] static inline float32x4 replace_lane(const float32x4 value, const float lane_value) noexcept
        {
            static_assert(Lane >= 0 && Lane < lane_count);
            float32x4 result{ value };
            result = vsetq_lane_f32(lane_value, result, Lane);
            return result;
        }
    };
} // namespace chlm::detail
