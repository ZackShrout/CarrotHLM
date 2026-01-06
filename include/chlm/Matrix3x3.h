//
// Created by zshrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Core.h"
#include "Quaternion.h"

namespace chlm {
    // ========================================
    // float3x3 - Column-major, 3 float3 columns (pure rotation/orientation)
    // ========================================
    struct float3x3
    {
        float3 columns[3]{
            float3{ 1.0f, 0.0f, 0.0f },
            float3{ 0.0f, 1.0f, 0.0f },
            float3{ 0.0f, 0.0f, 1.0f }
        }; // default is identity

        // Access column
        float3& operator[](int i)
        {
            assert(i >= 0 && i < 3);
            return columns[i];
        }

        const float3& operator[](int i) const
        {
            assert(i >= 0 && i < 3);
            return columns[i];
        }

        // Static builders
        static constexpr float3x3 identity() noexcept { return { }; }

        static float3x3 quat_to_float3x3_internal(const quat& q) noexcept
        {
            const float xx{ q.x * q.x };
            const float yy{ q.y * q.y };
            const float zz{ q.z * q.z };
            const float xy{ q.x * q.y };
            const float xz{ q.x * q.z };
            const float yz{ q.y * q.z };
            const float wx{ q.w * q.x };
            const float wy{ q.w * q.y };
            const float wz{ q.w * q.z };

            return float3x3{
                float3{ 1.f - 2.f * (yy + zz), 2.f * (xy - wz), 2.f * (xz + wy) },
                float3{ 2.f * (xy + wz), 1.f - 2.f * (xx + zz), 2.f * (yz - wx) },
                float3{ 2.f * (xz - wy), 2.f * (yz + wx), 1.f - 2.f * (xx + yy) }
            };
        }

        static float3x3 rotate_x(float rad) noexcept;
        static float3x3 rotate_y(float rad) noexcept;
        static float3x3 rotate_z(float rad) noexcept;
        static float3x3 rotate_axis_angle(float3 axis, float rad) noexcept;
    };

    // ========================================
    // Multiplication
    // ========================================
    inline float3 mul(const float3x3& m, const float3& v) noexcept
    {
        return v.x * m.columns[0] +
               v.y * m.columns[1] +
               v.z * m.columns[2];
    }

    inline float3x3 mul(const float3x3& a, const float3x3& b) noexcept
    {
        float3x3 result;
        result.columns[0] = mul(a, b.columns[0]);
        result.columns[1] = mul(a, b.columns[1]);
        result.columns[2] = mul(a, b.columns[2]);
        return result;
    }

    inline float3 operator*(const float3x3& m, const float3& v) noexcept { return mul(m, v); }
    inline float3x3 operator*(const float3x3& a, const float3x3& b) noexcept { return mul(a, b); }

    // ========================================
    // Inverse (for rotation matrices: transpose = inverse)
    // Fast path — assumes orthonormal (pure rotation)
    // ========================================
    inline float3x3 transpose(const float3x3& m) noexcept
    {
        return float3x3{
            float3{ m[0].x, m[1].x, m[2].x },
            float3{ m[0].y, m[1].y, m[2].y },
            float3{ m[0].z, m[1].z, m[2].z }
        };
    }

    inline float3x3 inverse(const float3x3& m) noexcept
    {
        // Note: for pure rotation matrices, inverse = transpose
        //       (determinant = 1, orthonormal columns)
        return transpose(m);
    }

    // ========================================
    // Builders
    // ========================================
    inline float3x3 float3x3::rotate_x(const float rad) noexcept
    {
        const float c{ std::cos(rad) };
        const float s{ std::sin(rad) };

        return float3x3{
            float3{ 1.f, 0.f, 0.f },
            float3{ 0.f, c, s },
            float3{ 0.f, -s, c }
        };
    }

    inline float3x3 float3x3::rotate_y(const float rad) noexcept
    {
        const float c{ std::cos(rad) };
        const float s{ std::sin(rad) };

        return float3x3{
            float3{ c, 0.f, -s },
            float3{ 0.f, 1.f, 0.f },
            float3{ s, 0.f, c }
        };
    }

    inline float3x3 float3x3::rotate_z(const float rad) noexcept
    {
        const float c{ std::cos(rad) };
        const float s{ std::sin(rad) };

        return float3x3{
            float3{ c, s, 0.f },
            float3{ -s, c, 0.f },
            float3{ 0.f, 0.f, 1.f }
        };
    }

    inline float3x3 float3x3::rotate_axis_angle(const float3 axis, const float rad) noexcept
    {
        return quat_to_float3x3_internal(quat_from_axis_angle(axis, rad));
    }
} // namespace chlm
