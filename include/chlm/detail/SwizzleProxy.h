//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "SwizzleCommon.h"
#include "VectorBase.h"

#include <array>

namespace chlm::detail {
    template<typename Owner, int... Indices>
    struct swizzle_proxy
    {
        static_assert(sizeof...(Indices) >= 2 && sizeof...(Indices) <= 4);

        using owner_type = Owner;
        static constexpr int component_count{ static_cast<int>(sizeof...(Indices)) };
        static constexpr bool writable{ swizzle_indices_unique_v<Indices...> };
        using value_type = float_vector_base<component_count>;

        Owner* owner{ };

        explicit swizzle_proxy(Owner& target) noexcept
            : owner(&target) {}

        [[nodiscard]] value_type read() const
        {
            const std::array<float, component_count> lanes{ owner->template lane<Indices>()... };

            if constexpr (component_count == 2)
            {
                return value_type{ lanes[0], lanes[1] };
            }
            else if constexpr (component_count == 3)
            {
                return value_type{ lanes[0], lanes[1], lanes[2] };
            }
            else
            {
                return value_type{ lanes[0], lanes[1], lanes[2], lanes[3] };
            }
        }

        [[nodiscard]] operator value_type() const
        {
            return read();
        }

        template<typename RhsOwner, int... RhsIndices>
        swizzle_proxy& operator=(const swizzle_proxy<RhsOwner, RhsIndices...>& rhs)
            requires(component_count == swizzle_proxy<RhsOwner, RhsIndices...>::component_count && writable)
        {
            return (*this = static_cast<value_type>(rhs));
        }

        swizzle_proxy& operator=(const value_type& rhs)
            requires(writable)
        {
            const auto rhs_lanes{ snapshot_lanes(rhs, std::make_index_sequence<component_count>{ }) };
            assign_from_lanes(rhs_lanes, std::make_index_sequence<component_count>{ });
            return *this;
        }

    private:
        template<std::size_t... LaneOffsets>
        [[nodiscard]] static std::array<float, component_count> snapshot_lanes(const value_type& rhs,
                                                                               std::index_sequence<LaneOffsets...>)
        {
            return { rhs.template lane<static_cast<int>(LaneOffsets)>()... };
        }

        template<std::size_t... LaneOffsets>
        void assign_from_lanes(const std::array<float, component_count>& rhs_lanes, std::index_sequence<LaneOffsets...>)
        {
            ((owner->template set_lane<lane_index_at<LaneOffsets>()>(rhs_lanes[LaneOffsets])), ...);
        }

        template<std::size_t Offset>
        [[nodiscard]] static consteval int lane_index_at()
        {
            constexpr int indices[]{ Indices... };
            return indices[Offset];
        }
    };

    template<int... Indices, typename Owner>
    [[nodiscard]] swizzle_proxy<Owner, Indices...> make_swizzle(Owner& owner) noexcept
    {
        return swizzle_proxy<Owner, Indices...>{ owner };
    }
} // namespace chlm::detail
