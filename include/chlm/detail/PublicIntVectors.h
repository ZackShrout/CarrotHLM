//
// Created by Zack Shrout on 7/12/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "PublicSwizzleTypes.h"
#include "GeneratedInt2Swizzles.h"
#include "GeneratedInt3Swizzles.h"
#include "GeneratedInt4Swizzles.h"

namespace chlm {
    struct int2
    {
        union
        {
            struct
            {
                int x;
                int y;
            };

            struct
            {
                int r;
                int g;
            };

            struct
            {
                int s;
                int t;
            };

            struct
            {
                int u;
                int v;
            };

            CHLM_DETAIL_DECLARE_INT2_SWIZZLES();
        };

        constexpr int2() noexcept : x(0), y(0) {}

        constexpr int2(const int x_value, const int y_value) noexcept : x(x_value), y(y_value) {}

        constexpr int2(const int2& other) noexcept : x(other.x), y(other.y) {}

        constexpr int2& operator=(const int2& rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;

            return *this;
        }

        [[nodiscard]] friend constexpr int2 operator+(const int2& a, const int2& b) noexcept
        {
            return int2{ a.x + b.x, a.y + b.y };
        }

        [[nodiscard]] friend constexpr int2 operator-(const int2& a, const int2& b) noexcept
        {
            return int2{ a.x - b.x, a.y - b.y };
        }

        [[nodiscard]] friend constexpr int2 operator*(const int2& a, const int2& b) noexcept
        {
            return int2{ a.x * b.x, a.y * b.y };
        }

        [[nodiscard]] friend constexpr int2 operator/(const int2& a, const int2& b) noexcept
        {
            return int2{ a.x / b.x, a.y / b.y };
        }

        [[nodiscard]] friend constexpr int2 operator*(const int2& v, const int scalar) noexcept
        {
            return int2{ v.x * scalar, v.y * scalar };
        }

        [[nodiscard]] friend constexpr int2 operator*(const int scalar, const int2& v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] friend constexpr int2 operator/(const int2& v, const int scalar) noexcept
        {
            return int2{ v.x / scalar, v.y / scalar };
        }

        [[nodiscard]] friend constexpr int2 operator-(const int2& v) noexcept
        {
            return int2{ -v.x, -v.y };
        }

        constexpr int2& operator+=(const int2& rhs) noexcept
        {
            *this = *this + rhs;
            return *this;
        }

        constexpr int2& operator-=(const int2& rhs) noexcept
        {
            *this = *this - rhs;
            return *this;
        }

        constexpr int2& operator*=(const int scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        constexpr int2& operator/=(const int scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }
    };

    struct int3
    {
        union
        {
            struct
            {
                int x;
                int y;
                int z;
            };

            struct
            {
                int r;
                int g;
                int b;
            };

            struct
            {
                int s;
                int t;
                int p;
            };

            CHLM_DETAIL_DECLARE_INT3_SWIZZLES();
        };

        constexpr int3() noexcept : x(0), y(0), z(0) {}

        constexpr int3(const int x_value, const int y_value, const int z_value) noexcept : x(x_value), y(y_value),
            z(z_value) {}

        constexpr int3(const int3& other) noexcept : x(other.x), y(other.y), z(other.z) {}

        constexpr int3& operator=(const int3& rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;

            return *this;
        }

        [[nodiscard]] friend constexpr int3 operator+(const int3& a, const int3& b) noexcept
        {
            return int3{ a.x + b.x, a.y + b.y, a.z + b.z };
        }

        [[nodiscard]] friend constexpr int3 operator-(const int3& a, const int3& b) noexcept
        {
            return int3{ a.x - b.x, a.y - b.y, a.z - b.z };
        }

        [[nodiscard]] friend constexpr int3 operator*(const int3& a, const int3& b) noexcept
        {
            return int3{ a.x * b.x, a.y * b.y, a.z * b.z };
        }

        [[nodiscard]] friend constexpr int3 operator/(const int3& a, const int3& b) noexcept
        {
            return int3{ a.x / b.x, a.y / b.y, a.z / b.z };
        }

        [[nodiscard]] friend constexpr int3 operator*(const int3& v, const int scalar) noexcept
        {
            return int3{ v.x * scalar, v.y * scalar, v.z * scalar };
        }

        [[nodiscard]] friend constexpr int3 operator*(const int scalar, const int3& v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] friend constexpr int3 operator/(const int3& v, const int scalar) noexcept
        {
            return int3{ v.x / scalar, v.y / scalar, v.z / scalar };
        }

        [[nodiscard]] friend constexpr int3 operator-(const int3& v) noexcept
        {
            return int3{ -v.x, -v.y, -v.z };
        }

        constexpr int3& operator+=(const int3& rhs) noexcept
        {
            *this = *this + rhs;
            return *this;
        }

        constexpr int3& operator-=(const int3& rhs) noexcept
        {
            *this = *this - rhs;
            return *this;
        }

        constexpr int3& operator*=(const int scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        constexpr int3& operator/=(const int scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }
    };

    struct int4
    {
        union
        {
            struct
            {
                int x;
                int y;
                int z;
                int w;
            };

            struct
            {
                int r;
                int g;
                int b;
                int a;
            };

            struct
            {
                int s;
                int t;
                int p;
                int q;
            };

            CHLM_DETAIL_DECLARE_INT4_SWIZZLES();
        };

        constexpr int4() noexcept : x(0), y(0), z(0), w(0) {}

        constexpr int4(const int x_value, const int y_value, const int z_value, const int w_value) noexcept
            : x(x_value), y(y_value), z(z_value), w(w_value) {}

        constexpr int4(const int4& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {}

        constexpr int4& operator=(const int4& rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;

            return *this;
        }

        [[nodiscard]] friend constexpr int4 operator+(const int4& a, const int4& b) noexcept
        {
            return int4{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
        }

        [[nodiscard]] friend constexpr int4 operator-(const int4& a, const int4& b) noexcept
        {
            return int4{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
        }

        [[nodiscard]] friend constexpr int4 operator*(const int4& a, const int4& b) noexcept
        {
            return int4{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
        }

        [[nodiscard]] friend constexpr int4 operator/(const int4& a, const int4& b) noexcept
        {
            return int4{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
        }

        [[nodiscard]] friend constexpr int4 operator*(const int4& v, const int scalar) noexcept
        {
            return int4{ v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar };
        }

        [[nodiscard]] friend constexpr int4 operator*(const int scalar, const int4& v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] friend constexpr int4 operator/(const int4& v, const int scalar) noexcept
        {
            return int4{ v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar };
        }

        [[nodiscard]] friend constexpr int4 operator-(const int4& v) noexcept
        {
            return int4{ -v.x, -v.y, -v.z, -v.w };
        }

        constexpr int4& operator+=(const int4& rhs) noexcept
        {
            *this = *this + rhs;
            return *this;
        }

        constexpr int4& operator-=(const int4& rhs) noexcept
        {
            *this = *this - rhs;
            return *this;
        }

        constexpr int4& operator*=(const int scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        constexpr int4& operator/=(const int scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }
    };
} // namespace chlm

namespace chlm::detail {
    template<int OwnerComponents, int I0, int I1>
    [[nodiscard]] inline ivector_swizzle2<OwnerComponents, I0, I1>::operator int2() const noexcept
    {
        return int2{ lanes[I0], lanes[I1] };
    }

    template<int OwnerComponents, int I0, int I1>
    inline ivector_swizzle2<OwnerComponents, I0, I1>&
    ivector_swizzle2<OwnerComponents, I0, I1>::operator=(const int2& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1>)
    {
        const int tmp0{ rhs.x };
        const int tmp1{ rhs.y };
        lanes[I0] = tmp0;
        lanes[I1] = tmp1;

        return *this;
    }

    template<int OwnerComponents, int I0, int I1>
    inline ivector_swizzle2<OwnerComponents, I0, I1>&
    ivector_swizzle2<OwnerComponents, I0, I1>::operator=(const ivector_swizzle2& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1>)
    {
        return *this = static_cast<int2>(rhs);
    }

    template<int OwnerComponents, int I0, int I1, int I2>
    [[nodiscard]] inline ivector_swizzle3<OwnerComponents, I0, I1, I2>::operator int3() const noexcept
    {
        return int3{ lanes[I0], lanes[I1], lanes[I2] };
    }

    template<int OwnerComponents, int I0, int I1, int I2>
    inline ivector_swizzle3<OwnerComponents, I0, I1, I2>&
    ivector_swizzle3<OwnerComponents, I0, I1, I2>::operator=(const int3& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2>)
    {
        const int tmp0{ rhs.x };
        const int tmp1{ rhs.y };
        const int tmp2{ rhs.z };
        lanes[I0] = tmp0;
        lanes[I1] = tmp1;
        lanes[I2] = tmp2;

        return *this;
    }

    template<int OwnerComponents, int I0, int I1, int I2>
    inline ivector_swizzle3<OwnerComponents, I0, I1, I2>&
    ivector_swizzle3<OwnerComponents, I0, I1, I2>::operator=(const ivector_swizzle3& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2>)
    {
        return *this = static_cast<int3>(rhs);
    }

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    [[nodiscard]] inline ivector_swizzle4<OwnerComponents, I0, I1, I2, I3>::operator int4() const noexcept
    {
        return int4{ lanes[I0], lanes[I1], lanes[I2], lanes[I3] };
    }

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    inline ivector_swizzle4<OwnerComponents, I0, I1, I2, I3>&
    ivector_swizzle4<OwnerComponents, I0, I1, I2, I3>::operator=(const int4& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2, I3>)
    {
        const int tmp0{ rhs.x };
        const int tmp1{ rhs.y };
        const int tmp2{ rhs.z };
        const int tmp3{ rhs.w };
        lanes[I0] = tmp0;
        lanes[I1] = tmp1;
        lanes[I2] = tmp2;
        lanes[I3] = tmp3;

        return *this;
    }

    template<int OwnerComponents, int I0, int I1, int I2, int I3>
    inline ivector_swizzle4<OwnerComponents, I0, I1, I2, I3>&
    ivector_swizzle4<OwnerComponents, I0, I1, I2, I3>::operator=(const ivector_swizzle4& rhs) noexcept
        requires(swizzle_indices_unique_v<I0, I1, I2, I3>)
    {
        return *this = static_cast<int4>(rhs);
    }
} // namespace chlm::detail
