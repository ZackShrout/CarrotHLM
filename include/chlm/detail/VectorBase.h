//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "VectorStorage.h"

namespace chlm::detail {
    template<int Components>
    struct float_vector_base
    {
        static_assert(Components >= 2 && Components <= 4);

        static constexpr int component_count{ Components };
        using storage_type = float_storage;

        storage_type storage{};

        float_vector_base() = default;

        float_vector_base(const float x, const float y)
            : storage(x, y, 0.0f, 0.0f)
        {
            static_assert(Components == 2);
        }

        float_vector_base(const float x, const float y, const float z)
            : storage(x, y, z, 0.0f)
        {
            static_assert(Components == 3);
        }

        float_vector_base(const float x, const float y, const float z, const float w)
            : storage(x, y, z, w)
        {
            static_assert(Components == 4);
        }

        explicit float_vector_base(const storage_type& new_storage)
            : storage(new_storage)
        {
        }

        template<int Lane>
        [[nodiscard]] float lane() const
        {
            static_assert(Lane >= 0 && Lane < Components);
            return storage.template lane<Lane>();
        }

        template<int Lane>
        void set_lane(const float lane_value)
        {
            static_assert(Lane >= 0 && Lane < Components);
            storage = storage.template with_lane<Lane>(lane_value);
        }

        [[nodiscard]] friend float_vector_base operator+(const float_vector_base& a, const float_vector_base& b)
        {
            return float_vector_base{ a.storage + b.storage };
        }

        [[nodiscard]] friend float_vector_base operator-(const float_vector_base& a, const float_vector_base& b)
        {
            return float_vector_base{ a.storage - b.storage };
        }
    };

    using float2_base = float_vector_base<2>;
    using float3_base = float_vector_base<3>;
    using float4_base = float_vector_base<4>;
} // namespace chlm::detail
