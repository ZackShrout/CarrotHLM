//
// Created by Zack Shrout on 7/12/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "PublicSwizzleTypes.h"
#include "GeneratedUint2Swizzles.h"
#include "GeneratedUint3Swizzles.h"
#include "GeneratedUint4Swizzles.h"

namespace chlm {
    struct uint2
    {
        union
        {
            struct
            {
                unsigned int x;
                unsigned int y;
            };

            struct
            {
                unsigned int r;
                unsigned int g;
            };

            struct
            {
                unsigned int s;
                unsigned int t;
            };

            struct
            {
                unsigned int u;
                unsigned int v;
            };

            CHLM_DETAIL_DECLARE_UINT2_SWIZZLES();
        };

        constexpr uint2() noexcept : x(0u), y(0u) {}

        constexpr uint2(const unsigned int x_value, const unsigned int y_value) noexcept : x(x_value), y(y_value) {}

        constexpr uint2(const uint2& other) noexcept : x(other.x), y(other.y) {}

        constexpr uint2& operator=(const uint2& rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;

            return *this;
        }

        [[nodiscard]] friend constexpr uint2 operator+(const uint2& a, const uint2& b) noexcept
        {
            return uint2{ a.x + b.x, a.y + b.y };
        }

        [[nodiscard]] friend constexpr uint2 operator-(const uint2& a, const uint2& b) noexcept
        {
            return uint2{ a.x - b.x, a.y - b.y };
        }

        [[nodiscard]] friend constexpr uint2 operator*(const uint2& a, const uint2& b) noexcept
        {
            return uint2{ a.x * b.x, a.y * b.y };
        }

        [[nodiscard]] friend constexpr uint2 operator/(const uint2& a, const uint2& b) noexcept
        {
            return uint2{ a.x / b.x, a.y / b.y };
        }

        [[nodiscard]] friend constexpr uint2 operator*(const uint2& v, const unsigned int scalar) noexcept
        {
            return uint2{ v.x * scalar, v.y * scalar };
        }

        [[nodiscard]] friend constexpr uint2 operator*(const unsigned int scalar, const uint2& v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] friend constexpr uint2 operator/(const uint2& v, const unsigned int scalar) noexcept
        {
            return uint2{ v.x / scalar, v.y / scalar };
        }

        constexpr uint2& operator+=(const uint2& rhs) noexcept
        {
            *this = *this + rhs;
            return *this;
        }

        constexpr uint2& operator-=(const uint2& rhs) noexcept
        {
            *this = *this - rhs;
            return *this;
        }

        constexpr uint2& operator*=(const unsigned int scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        constexpr uint2& operator/=(const unsigned int scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }
    };

    struct uint3
    {
        union
        {
            struct
            {
                unsigned int x;
                unsigned int y;
                unsigned int z;
            };

            struct
            {
                unsigned int r;
                unsigned int g;
                unsigned int b;
            };

            struct
            {
                unsigned int s;
                unsigned int t;
                unsigned int p;
            };

            CHLM_DETAIL_DECLARE_UINT3_SWIZZLES();
        };

        constexpr uint3() noexcept : x(0u), y(0u), z(0u) {}

        constexpr uint3(const unsigned int x_value, const unsigned int y_value, const unsigned int z_value) noexcept
            : x(x_value), y(y_value), z(z_value) {}

        constexpr uint3(const uint3& other) noexcept : x(other.x), y(other.y), z(other.z) {}

        constexpr uint3& operator=(const uint3& rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;

            return *this;
        }

        [[nodiscard]] friend constexpr uint3 operator+(const uint3& a, const uint3& b) noexcept
        {
            return uint3{ a.x + b.x, a.y + b.y, a.z + b.z };
        }

        [[nodiscard]] friend constexpr uint3 operator-(const uint3& a, const uint3& b) noexcept
        {
            return uint3{ a.x - b.x, a.y - b.y, a.z - b.z };
        }

        [[nodiscard]] friend constexpr uint3 operator*(const uint3& a, const uint3& b) noexcept
        {
            return uint3{ a.x * b.x, a.y * b.y, a.z * b.z };
        }

        [[nodiscard]] friend constexpr uint3 operator/(const uint3& a, const uint3& b) noexcept
        {
            return uint3{ a.x / b.x, a.y / b.y, a.z / b.z };
        }

        [[nodiscard]] friend constexpr uint3 operator*(const uint3& v, const unsigned int scalar) noexcept
        {
            return uint3{ v.x * scalar, v.y * scalar, v.z * scalar };
        }

        [[nodiscard]] friend constexpr uint3 operator*(const unsigned int scalar, const uint3& v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] friend constexpr uint3 operator/(const uint3& v, const unsigned int scalar) noexcept
        {
            return uint3{ v.x / scalar, v.y / scalar, v.z / scalar };
        }

        constexpr uint3& operator+=(const uint3& rhs) noexcept
        {
            *this = *this + rhs;
            return *this;
        }

        constexpr uint3& operator-=(const uint3& rhs) noexcept
        {
            *this = *this - rhs;
            return *this;
        }

        constexpr uint3& operator*=(const unsigned int scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        constexpr uint3& operator/=(const unsigned int scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }
    };

    struct uint4
    {
        union
        {
            struct
            {
                unsigned int x;
                unsigned int y;
                unsigned int z;
                unsigned int w;
            };

            struct
            {
                unsigned int r;
                unsigned int g;
                unsigned int b;
                unsigned int a;
            };

            struct
            {
                unsigned int s;
                unsigned int t;
                unsigned int p;
                unsigned int q;
            };

            CHLM_DETAIL_DECLARE_UINT4_SWIZZLES();
        };

        constexpr uint4() noexcept : x(0u), y(0u), z(0u), w(0u) {}

        constexpr uint4(const unsigned int x_value, const unsigned int y_value, const unsigned int z_value,
                        const unsigned int w_value) noexcept
            : x(x_value), y(y_value), z(z_value), w(w_value) {}

        constexpr uint4(const uint4& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {}

        constexpr uint4& operator=(const uint4& rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
            return *this;
        }

        [[nodiscard]] friend constexpr uint4 operator+(const uint4& a, const uint4& b) noexcept
        {
            return uint4{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
        }

        [[nodiscard]] friend constexpr uint4 operator-(const uint4& a, const uint4& b) noexcept
        {
            return uint4{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
        }

        [[nodiscard]] friend constexpr uint4 operator*(const uint4& a, const uint4& b) noexcept
        {
            return uint4{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
        }

        [[nodiscard]] friend constexpr uint4 operator/(const uint4& a, const uint4& b) noexcept
        {
            return uint4{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
        }

        [[nodiscard]] friend constexpr uint4 operator*(const uint4& v, const unsigned int scalar) noexcept
        {
            return uint4{ v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar };
        }

        [[nodiscard]] friend constexpr uint4 operator*(const unsigned int scalar, const uint4& v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] friend constexpr uint4 operator/(const uint4& v, const unsigned int scalar) noexcept
        {
            return uint4{ v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar };
        }

        constexpr uint4& operator+=(const uint4& rhs) noexcept
        {
            *this = *this + rhs;
            return *this;
        }

        constexpr uint4& operator-=(const uint4& rhs) noexcept
        {
            *this = *this - rhs;
            return *this;
        }

        constexpr uint4& operator*=(const unsigned int scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        constexpr uint4& operator/=(const unsigned int scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }
    };
} // namespace chlm

namespace chlm::detail {
    template<int OwnerComponents, int I0, int I1>
    [[nodiscard]] inline uvector_swizzle2<OwnerComponents, I0, I1>::operator uint2() const noexcept
    {
        return uint2{ lanes[I0], lanes[I1] };
    }

    template<int OwnerComponents, int I0, int I1>
    inline uvector_swizzle2<OwnerComponents, I0, I1>&
    uvector_swizzle2<OwnerComponents, I0, I1>::operator=(const uint2& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1>)
    {
        const unsigned int tmp0{ rhs.x };
        const unsigned int tmp1{ rhs.y };
        lanes[I0] = tmp0;
        lanes[I1] = tmp1;

        return *this;
    }

    template<int OwnerComponents, int I0, int I1>
    inline uvector_swizzle2<OwnerComponents, I0, I1>&
    uvector_swizzle2<OwnerComponents, I0, I1>::operator=(const uvector_swizzle2& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1>)
    {
        return *this = static_cast<uint2>(rhs);
    }

    template<int OwnerComponents, int I0, int I1, int I2>
    [[nodiscard]] inline uvector_swizzle3<OwnerComponents, I0, I1, I2>::operator uint3() const noexcept
    {
        return uint3{ lanes[I0], lanes[I1], lanes[I2] };
    }

    template<int OwnerComponents, int I0, int I1, int I2>
    inline uvector_swizzle3<OwnerComponents, I0, I1, I2>&
    uvector_swizzle3<OwnerComponents, I0, I1, I2>::operator=(const uint3& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2>)
    {
        const unsigned int tmp0{ rhs.x };
        const unsigned int tmp1{ rhs.y };
        const unsigned int tmp2{ rhs.z };
        lanes[I0] = tmp0;
        lanes[I1] = tmp1;
        lanes[I2] = tmp2;

        return *this;
    }

    template<int OwnerComponents, int I0, int I1, int I2>
    inline uvector_swizzle3<OwnerComponents, I0, I1, I2>&
    uvector_swizzle3<OwnerComponents, I0, I1, I2>::operator=(const uvector_swizzle3& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2>)
    {
        return *this = static_cast<uint3>(rhs);
    }

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    [[nodiscard]] inline uvector_swizzle4<OwnerComponents, I0, I1, I2, I3>::operator uint4() const noexcept
    {
        return uint4{ lanes[I0], lanes[I1], lanes[I2], lanes[I3] };
    }

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    inline uvector_swizzle4<OwnerComponents, I0, I1, I2, I3>&
    uvector_swizzle4<OwnerComponents, I0, I1, I2, I3>::operator=(const uint4& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2, I3>)
    {
        const unsigned int tmp0{ rhs.x };
        const unsigned int tmp1{ rhs.y };
        const unsigned int tmp2{ rhs.z };
        const unsigned int tmp3{ rhs.w };
        lanes[I0] = tmp0;
        lanes[I1] = tmp1;
        lanes[I2] = tmp2;
        lanes[I3] = tmp3;

        return *this;
    }

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    inline uvector_swizzle4<OwnerComponents, I0, I1, I2, I3>&
    uvector_swizzle4<OwnerComponents, I0, I1, I2, I3>::operator=(const uvector_swizzle4& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2, I3>)
    {
        return *this = static_cast<uint4>(rhs);
    }
} // namespace chlm::detail
