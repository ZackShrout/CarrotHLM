//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "simd/SimdSelect.h"

#include <array>

namespace chlm::detail {
    template<typename Backend>
    struct basic_float_storage
    {
        using backend_type = Backend;
        using register_type = typename Backend::float32x4;

        register_type value;

        basic_float_storage()
            : value(Backend::splat(0.0f))
        {
        }

        basic_float_storage(const float x, const float y, const float z, const float w)
            : value(Backend::set(x, y, z, w))
        {
        }

        explicit basic_float_storage(const register_type native_value)
            : value(native_value)
        {
        }

        [[nodiscard]] static basic_float_storage splat(const float lane_value)
        {
            return basic_float_storage{ Backend::splat(lane_value) };
        }

        [[nodiscard]] static basic_float_storage load(const float* lanes)
        {
            return basic_float_storage{ Backend::load(lanes) };
        }

        [[nodiscard]] static basic_float_storage bit_and(const basic_float_storage& a, const basic_float_storage& b)
        {
            return basic_float_storage{ Backend::bit_and(a.value, b.value) };
        }

        [[nodiscard]] static basic_float_storage bit_or(const basic_float_storage& a, const basic_float_storage& b)
        {
            return basic_float_storage{ Backend::bit_or(a.value, b.value) };
        }

        [[nodiscard]] static basic_float_storage bit_xor(const basic_float_storage& a, const basic_float_storage& b)
        {
            return basic_float_storage{ Backend::bit_xor(a.value, b.value) };
        }

        [[nodiscard]] static basic_float_storage min(const basic_float_storage& a, const basic_float_storage& b)
        {
            return basic_float_storage{ Backend::min(a.value, b.value) };
        }

        [[nodiscard]] static basic_float_storage max(const basic_float_storage& a, const basic_float_storage& b)
        {
            return basic_float_storage{ Backend::max(a.value, b.value) };
        }

        void store(float* out) const
        {
            Backend::store(out, value);
        }

        [[nodiscard]] std::array<float, 4> to_array() const
        {
            std::array<float, 4> lanes{};
            store(lanes.data());

            return lanes;
        }

        template<int Lane>
        [[nodiscard]] float lane() const
        {
            return Backend::template extract_lane<Lane>(value);
        }

        template<int Lane>
        [[nodiscard]] basic_float_storage with_lane(const float lane_value) const
        {
            return basic_float_storage{ Backend::template replace_lane<Lane>(value, lane_value) };
        }

        [[nodiscard]] static register_type cmp_eq(const basic_float_storage& a, const basic_float_storage& b)
        {
            return Backend::cmp_eq(a.value, b.value);
        }

        [[nodiscard]] static register_type cmp_neq(const basic_float_storage& a, const basic_float_storage& b)
        {
            return Backend::cmp_neq(a.value, b.value);
        }

        [[nodiscard]] static register_type cmp_lt(const basic_float_storage& a, const basic_float_storage& b)
        {
            return Backend::cmp_lt(a.value, b.value);
        }

        [[nodiscard]] static register_type cmp_lte(const basic_float_storage& a, const basic_float_storage& b)
        {
            return Backend::cmp_lte(a.value, b.value);
        }

        [[nodiscard]] static register_type cmp_gt(const basic_float_storage& a, const basic_float_storage& b)
        {
            return Backend::cmp_gt(a.value, b.value);
        }

        [[nodiscard]] static register_type cmp_gte(const basic_float_storage& a, const basic_float_storage& b)
        {
            return Backend::cmp_gte(a.value, b.value);
        }

        [[nodiscard]] static basic_float_storage select(const register_type mask, const basic_float_storage& if_true, const basic_float_storage& if_false)
        {
            return basic_float_storage{ Backend::select(mask, if_true.value, if_false.value) };
        }

        template<int I0, int I1, int I2, int I3>
        [[nodiscard]] static basic_float_storage shuffle(const basic_float_storage& value)
        {
            return basic_float_storage{ Backend::template shuffle<I0, I1, I2, I3>(value.value) };
        }

        [[nodiscard]] friend basic_float_storage operator+(const basic_float_storage& a, const basic_float_storage& b)
        {
            return basic_float_storage{ Backend::add(a.value, b.value) };
        }

        [[nodiscard]] friend basic_float_storage operator-(const basic_float_storage& a, const basic_float_storage& b)
        {
            return basic_float_storage{ Backend::sub(a.value, b.value) };
        }

        [[nodiscard]] friend basic_float_storage operator*(const basic_float_storage& a, const basic_float_storage& b)
        {
            return basic_float_storage{ Backend::mul(a.value, b.value) };
        }

        [[nodiscard]] friend basic_float_storage operator/(const basic_float_storage& a, const basic_float_storage& b)
        {
            return basic_float_storage{ Backend::div(a.value, b.value) };
        }
    };

    using float_storage = basic_float_storage<active_simd>;
    using scalar_float_storage = basic_float_storage<scalar_simd>;
} // namespace chlm::detail
