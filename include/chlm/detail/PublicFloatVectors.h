//
// Created by Zack Shrout on 7/12/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "PublicSwizzleTypes.h"
#include "GeneratedFloat2Swizzles.h"
#include "GeneratedFloat3Swizzles.h"
#include "GeneratedFloat4Swizzles.h"
#include "VectorStorage.h"

namespace chlm {
    struct float2
    {
        union
        {
            struct
            {
                float x;
                float y;
            };

            struct
            {
                float r;
                float g;
            };

            struct
            {
                float s;
                float t;
            };

            struct
            {
                float u;
                float v;
            };

            CHLM_DETAIL_DECLARE_FLOAT2_SWIZZLES();
        };

        constexpr float2() noexcept : x(0.f), y(0.f) {}

        constexpr float2(const float x_value, const float y_value) noexcept : x(x_value), y(y_value) {}

        constexpr float2(const float2& other) noexcept : x(other.x), y(other.y) {}

        explicit float2(const detail::float_storage& storage_value) noexcept
        {
            const auto lanes{ storage_value.to_array() };
            x = lanes[0];
            y = lanes[1];
        }

        float2& operator=(const float2& rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;

            return *this;
        }

        [[nodiscard]] detail::float_storage to_storage() const noexcept
        {
            return detail::float_storage{ x, y, 0.f, 0.f };
        }

        [[nodiscard]] friend float2 operator+(const float2& a, const float2& b) noexcept
        {
            return float2{ a.to_storage() + b.to_storage() };
        }

        [[nodiscard]] friend float2 operator-(const float2& a, const float2& b) noexcept
        {
            return float2{ a.to_storage() - b.to_storage() };
        }

        [[nodiscard]] friend float2 operator*(const float2& a, const float2& b) noexcept
        {
            return float2{ a.to_storage() * b.to_storage() };
        }

        [[nodiscard]] friend float2 operator/(const float2& a, const float2& b) noexcept
        {
            return float2{ a.to_storage() / b.to_storage() };
        }

        [[nodiscard]] friend float2 operator*(const float2& v, const float scalar) noexcept
        {
            return float2{ v.to_storage() * detail::float_storage::splat(scalar) };
        }

        [[nodiscard]] friend float2 operator*(const float scalar, const float2& v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] friend float2 operator/(const float2& v, const float scalar) noexcept
        {
            return float2{ v.to_storage() / detail::float_storage::splat(scalar) };
        }

        [[nodiscard]] friend float2 operator-(const float2& v) noexcept
        {
            return float2{ detail::float_storage::splat(0.f) - v.to_storage() };
        }

        float2& operator+=(const float2& rhs) noexcept
        {
            *this = *this + rhs;
            return *this;
        }

        float2& operator-=(const float2& rhs) noexcept
        {
            *this = *this - rhs;
            return *this;
        }

        float2& operator*=(const float scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        float2& operator/=(const float scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }
    };

    struct float3
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
            };

            struct
            {
                float r;
                float g;
                float b;
            };

            struct
            {
                float s;
                float t;
                float p;
            };

            CHLM_DETAIL_DECLARE_FLOAT3_SWIZZLES();
        };

        constexpr float3() noexcept : x(0.f), y(0.f), z(0.f) {}

        constexpr float3(const float x_value, const float y_value, const float z_value) noexcept
            : x(x_value), y(y_value), z(z_value) {}

        constexpr float3(const float3& other) noexcept : x(other.x), y(other.y), z(other.z) {}

        explicit float3(const detail::float_storage& storage_value) noexcept
        {
            const auto lanes{ storage_value.to_array() };
            x = lanes[0];
            y = lanes[1];
            z = lanes[2];
        }

        float3& operator=(const float3& rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;

            return *this;
        }

        [[nodiscard]] detail::float_storage to_storage() const noexcept
        {
            return detail::float_storage{ x, y, z, 0.f };
        }

        [[nodiscard]] friend float3 operator+(const float3& a, const float3& b) noexcept
        {
            return float3{ a.to_storage() + b.to_storage() };
        }

        [[nodiscard]] friend float3 operator-(const float3& a, const float3& b) noexcept
        {
            return float3{ a.to_storage() - b.to_storage() };
        }

        [[nodiscard]] friend float3 operator*(const float3& a, const float3& b) noexcept
        {
            return float3{ a.to_storage() * b.to_storage() };
        }

        [[nodiscard]] friend float3 operator/(const float3& a, const float3& b) noexcept
        {
            return float3{ a.to_storage() / b.to_storage() };
        }

        [[nodiscard]] friend float3 operator*(const float3& v, const float scalar) noexcept
        {
            return float3{ v.to_storage() * detail::float_storage::splat(scalar) };
        }

        [[nodiscard]] friend float3 operator*(const float scalar, const float3& v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] friend float3 operator/(const float3& v, const float scalar) noexcept
        {
            return float3{ v.to_storage() / detail::float_storage::splat(scalar) };
        }

        [[nodiscard]] friend float3 operator-(const float3& v) noexcept
        {
            return float3{ detail::float_storage::splat(0.f) - v.to_storage() };
        }

        float3& operator+=(const float3& rhs) noexcept
        {
            *this = *this + rhs;
            return *this;
        }

        float3& operator-=(const float3& rhs) noexcept
        {
            *this = *this - rhs;
            return *this;
        }

        float3& operator*=(const float scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        float3& operator/=(const float scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }
    };

    struct float4
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };

            struct
            {
                float r;
                float g;
                float b;
                float a;
            };

            struct
            {
                float s;
                float t;
                float p;
                float q;
            };

            CHLM_DETAIL_DECLARE_FLOAT4_SWIZZLES();
        };

        constexpr float4() noexcept : x(0.f), y(0.f), z(0.f), w(0.f) {}

        constexpr float4(const float x_value, const float y_value, const float z_value, const float w_value) noexcept
            : x(x_value), y(y_value), z(z_value), w(w_value) {}

        constexpr float4(const float4& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {}

        explicit float4(const detail::float_storage& storage_value) noexcept
        {
            const auto lanes{ storage_value.to_array() };
            x = lanes[0];
            y = lanes[1];
            z = lanes[2];
            w = lanes[3];
        }

        float4& operator=(const float4& rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;

            return *this;
        }

        [[nodiscard]] detail::float_storage to_storage() const noexcept
        {
            return detail::float_storage{ x, y, z, w };
        }

        [[nodiscard]] friend float4 operator+(const float4& a, const float4& b) noexcept
        {
            return float4{ a.to_storage() + b.to_storage() };
        }

        [[nodiscard]] friend float4 operator-(const float4& a, const float4& b) noexcept
        {
            return float4{ a.to_storage() - b.to_storage() };
        }

        [[nodiscard]] friend float4 operator*(const float4& a, const float4& b) noexcept
        {
            return float4{ a.to_storage() * b.to_storage() };
        }

        [[nodiscard]] friend float4 operator/(const float4& a, const float4& b) noexcept
        {
            return float4{ a.to_storage() / b.to_storage() };
        }

        [[nodiscard]] friend float4 operator*(const float4& v, const float scalar) noexcept
        {
            return float4{ v.to_storage() * detail::float_storage::splat(scalar) };
        }

        [[nodiscard]] friend float4 operator*(const float scalar, const float4& v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] friend float4 operator/(const float4& v, const float scalar) noexcept
        {
            return float4{ v.to_storage() / detail::float_storage::splat(scalar) };
        }

        [[nodiscard]] friend float4 operator-(const float4& v) noexcept
        {
            return float4{ detail::float_storage::splat(0.f) - v.to_storage() };
        }

        float4& operator+=(const float4& rhs) noexcept
        {
            *this = *this + rhs;
            return *this;
        }

        float4& operator-=(const float4& rhs) noexcept
        {
            *this = *this - rhs;
            return *this;
        }
    };
} // namespace chlm

namespace chlm::detail {
    template<int OwnerComponents, int I0, int I1>
    [[nodiscard]] inline vector_swizzle2<OwnerComponents, I0, I1>::operator float2() const noexcept
    {
        return float2{ lanes[I0], lanes[I1] };
    }

    template<int OwnerComponents, int I0, int I1>
    inline vector_swizzle2<OwnerComponents, I0, I1>&
    vector_swizzle2<OwnerComponents, I0, I1>::operator=(const float2& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1>)
    {
        const float tmp0{ rhs.x };
        const float tmp1{ rhs.y };
        lanes[I0] = tmp0;
        lanes[I1] = tmp1;

        return *this;
    }

    template<int OwnerComponents, int I0, int I1>
    inline vector_swizzle2<OwnerComponents, I0, I1>&
    vector_swizzle2<OwnerComponents, I0, I1>::operator=(const vector_swizzle2& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1>)
    {
        return *this = static_cast<float2>(rhs);
    }

    template<int OwnerComponents, int I0, int I1, int I2>
    [[nodiscard]] inline vector_swizzle3<OwnerComponents, I0, I1, I2>::operator float3() const noexcept
    {
        return float3{ lanes[I0], lanes[I1], lanes[I2] };
    }

    template<int OwnerComponents, int I0, int I1, int I2>
    inline vector_swizzle3<OwnerComponents, I0, I1, I2>&
    vector_swizzle3<OwnerComponents, I0, I1, I2>::operator=(const float3& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2>)
    {
        const float tmp0{ rhs.x };
        const float tmp1{ rhs.y };
        const float tmp2{ rhs.z };
        lanes[I0] = tmp0;
        lanes[I1] = tmp1;
        lanes[I2] = tmp2;

        return *this;
    }

    template<int OwnerComponents, int I0, int I1, int I2>
    inline vector_swizzle3<OwnerComponents, I0, I1, I2>&
    vector_swizzle3<OwnerComponents, I0, I1, I2>::operator=(const vector_swizzle3& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2>)
    {
        return *this = static_cast<float3>(rhs);
    }

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    [[nodiscard]] inline vector_swizzle4<OwnerComponents, I0, I1, I2, I3>::operator float4() const noexcept
    {
        return float4{ lanes[I0], lanes[I1], lanes[I2], lanes[I3] };
    }

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    inline vector_swizzle4<OwnerComponents, I0, I1, I2, I3>&
    vector_swizzle4<OwnerComponents, I0, I1, I2, I3>::operator=(const float4& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2, I3>)
    {
        const float tmp0{ rhs.x };
        const float tmp1{ rhs.y };
        const float tmp2{ rhs.z };
        const float tmp3{ rhs.w };
        lanes[I0] = tmp0;
        lanes[I1] = tmp1;
        lanes[I2] = tmp2;
        lanes[I3] = tmp3;

        return *this;
    }

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    inline vector_swizzle4<OwnerComponents, I0, I1, I2, I3>&
    vector_swizzle4<OwnerComponents, I0, I1, I2, I3>::operator=(const vector_swizzle4& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2, I3>)
    {
        return *this = static_cast<float4>(rhs);
    }
} // namespace chlm::detail
