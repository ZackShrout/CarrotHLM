//
// Created by Zack Shrout on 7/12/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "SwizzleCommon.h"

namespace chlm {
    struct float2;
    struct float3;
    struct float4;
    struct uint2;
    struct uint3;
    struct uint4;
    struct int2;
    struct int3;
    struct int4;
} // namespace chlm

namespace chlm::detail {
    template<int OwnerComponents, int I0, int I1>
    struct vector_swizzle2
    {
        float lanes[OwnerComponents];

        [[nodiscard]] operator float2() const noexcept;

        vector_swizzle2& operator=(const float2& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1>);

        vector_swizzle2& operator=(const vector_swizzle2& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1>);
    };

    template<int OwnerComponents, int I0, int I1, int I2>
    struct vector_swizzle3
    {
        float lanes[OwnerComponents];

        [[nodiscard]] operator float3() const noexcept;

        vector_swizzle3& operator=(const float3& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2>);

        vector_swizzle3& operator=(const vector_swizzle3& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2>);
    };

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    struct vector_swizzle4
    {
        float lanes[OwnerComponents];

        [[nodiscard]] operator float4() const noexcept;

        vector_swizzle4& operator=(const float4& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2, I3>);

        vector_swizzle4& operator=(const vector_swizzle4& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2, I3>);
    };

    template<int OwnerComponents, int I0, int I1>
    struct uvector_swizzle2
    {
        unsigned int lanes[OwnerComponents];

        [[nodiscard]] operator uint2() const noexcept;

        uvector_swizzle2& operator=(const uint2& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1>);

        uvector_swizzle2& operator=(const uvector_swizzle2& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1>);
    };

    template<int OwnerComponents, int I0, int I1, int I2>
    struct uvector_swizzle3
    {
        unsigned int lanes[OwnerComponents];

        [[nodiscard]] operator uint3() const noexcept;

        uvector_swizzle3& operator=(const uint3& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2>);

        uvector_swizzle3& operator=(const uvector_swizzle3& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2>);
    };

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    struct uvector_swizzle4
    {
        unsigned int lanes[OwnerComponents];

        [[nodiscard]] operator uint4() const noexcept;

        uvector_swizzle4& operator=(const uint4& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2, I3>);

        uvector_swizzle4& operator=(const uvector_swizzle4& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2, I3>);
    };

    template<int OwnerComponents, int I0, int I1>
    struct ivector_swizzle2
    {
        int lanes[OwnerComponents];

        [[nodiscard]] operator int2() const noexcept;

        ivector_swizzle2& operator=(const int2& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1>);

        ivector_swizzle2& operator=(const ivector_swizzle2& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1>);
    };

    template<int OwnerComponents, int I0, int I1, int I2>
    struct ivector_swizzle3
    {
        int lanes[OwnerComponents];

        [[nodiscard]] operator int3() const noexcept;

        ivector_swizzle3& operator=(const int3& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2>);

        ivector_swizzle3& operator=(const ivector_swizzle3& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2>);
    };

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    struct ivector_swizzle4
    {
        int lanes[OwnerComponents];

        [[nodiscard]] operator int4() const noexcept;

        ivector_swizzle4& operator=(const int4& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2, I3>);

        ivector_swizzle4& operator=(const ivector_swizzle4& rhs) noexcept
            requires(swizzle_indices_unique_v<I0, I1, I2, I3>);
    };
} // namespace chlm::detail
