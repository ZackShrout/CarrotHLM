//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "SimdTraits.h"

#include <xmmintrin.h>
#include <emmintrin.h>

namespace chlm::detail {
    struct simd_sse2
    {
        using float32x4 = __m128;

        static constexpr simd_backend_kind kind{ simd_backend_kind::sse2 };
        static constexpr int lane_count{ 4 };

        [[nodiscard]] static inline float32x4 set(const float x, const float y, const float z, const float w) noexcept
        {
            return _mm_setr_ps(x, y, z, w);
        }

        [[nodiscard]] static inline float32x4 splat(const float value) noexcept
        {
            return _mm_set1_ps(value);
        }

        [[nodiscard]] static inline float32x4 load(const float* values) noexcept
        {
            return _mm_loadu_ps(values);
        }

        static inline void store(float* out, const float32x4 value) noexcept
        {
            _mm_storeu_ps(out, value);
        }

        [[nodiscard]] static inline float32x4 add(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_add_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 sub(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_sub_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 mul(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_mul_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 div(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_div_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 bit_and(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_and_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 bit_or(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_or_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 bit_xor(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_xor_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 min(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_min_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 max(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_max_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 cmp_eq(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_cmpeq_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 cmp_neq(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_cmpneq_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 cmp_lt(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_cmplt_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 cmp_lte(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_cmple_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 cmp_gt(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_cmpgt_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 cmp_gte(const float32x4 a, const float32x4 b) noexcept
        {
            return _mm_cmpge_ps(a, b);
        }

        [[nodiscard]] static inline float32x4 select(const float32x4 mask, const float32x4 if_true, const float32x4 if_false) noexcept
        {
            return _mm_or_ps(_mm_and_ps(mask, if_true), _mm_andnot_ps(mask, if_false));
        }

        template<int I0, int I1, int I2, int I3>
        [[nodiscard]] static inline float32x4 shuffle(const float32x4 value) noexcept
        {
            static_assert(I0 >= 0 && I0 < lane_count);
            static_assert(I1 >= 0 && I1 < lane_count);
            static_assert(I2 >= 0 && I2 < lane_count);
            static_assert(I3 >= 0 && I3 < lane_count);

            return _mm_shuffle_ps(value, value, _MM_SHUFFLE(I3, I2, I1, I0));
        }

        template<int Lane>
        [[nodiscard]] static inline float extract_lane(const float32x4 value) noexcept
        {
            static_assert(Lane >= 0 && Lane < lane_count);
            alignas(16) float lanes[4];
            _mm_store_ps(lanes, value);

            return lanes[Lane];
        }

        template<int Lane>
        [[nodiscard]] static inline float32x4 replace_lane(const float32x4 value, const float lane_value) noexcept
        {
            static_assert(Lane >= 0 && Lane < lane_count);
            alignas(16) float lanes[4];
            _mm_store_ps(lanes, value);
            lanes[Lane] = lane_value;

            return _mm_load_ps(lanes);
        }
    };
} // namespace chlm::detail
