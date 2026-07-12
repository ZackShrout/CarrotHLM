//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include <array>

namespace chlm::detail {
    template<int... Indices>
    consteval bool swizzle_indices_unique()
    {
        constexpr int indices[]{ Indices... };
        for (int i{ 0 }; i < static_cast<int>(sizeof...(Indices)); ++i)
        {
            for (int j{ i + 1 }; j < static_cast<int>(sizeof...(Indices)); ++j)
            {
                if (indices[i] == indices[j])
                {
                    return false;
                }
            }
        }

        return true;
    }

    template<int... Indices>
    inline constexpr bool swizzle_indices_unique_v{ swizzle_indices_unique<Indices...>() };
} // namespace chlm::detail
