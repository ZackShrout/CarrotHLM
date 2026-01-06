//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Core.h"

namespace chlm {
    // ========================================
    // float4x4 - Column-major, 4 float4 columns
    // ========================================
    struct float4x4
    {
        float4 columns[4]{
            float4{ 1.f, 0.f, 0.f, 0.f },
            float4{ 0.f, 1.f, 0.f, 0.f },
            float4{ 0.f, 0.f, 1.f, 0.f },
            float4{ 0.f, 0.f, 0.f, 1.f }
        }; // default is identity

        // Access column
        float4& operator[](const int i)
        {
            assert(i >= 0 && i < 4);
            return columns[i];
        }

        const float4& operator[](const int i) const
        {
            assert(i >= 0 && i < 4);
            return columns[i];
        }

        // Static builders
        static constexpr float4x4 identity() noexcept { return { }; }

        static float4x4 translate(float3 t) noexcept;
        static float4x4 scale(float3 s) noexcept;
        static float4x4 rotate_x(float rad) noexcept;
        static float4x4 rotate_y(float rad) noexcept;
        static float4x4 rotate_z(float rad) noexcept;
        static float4x4 rotate_axis_angle(float3 axis, float rad) noexcept;

        static float4x4 look_at_lh(float3 eye, float3 target, float3 up) noexcept;
        static float4x4 look_at_rh(float3 eye, float3 target, float3 up) noexcept;

        static float4x4 perspective_lh(float fov_y_rad, float aspect, float z_near, float z_far) noexcept;
        static float4x4 perspective_rh(float fov_y_rad, float aspect, float z_near, float z_far) noexcept;

        static float4x4 ortho_lh(float width, float height, float z_near, float z_far) noexcept;
        static float4x4 ortho_rh(float width, float height, float z_near, float z_far) noexcept;
    };

    // ========================================
    // Multiplication (HLSL order: mul(M, v) = matrix * vector)
    // ========================================
    inline float4 mul(const float4x4& m, const float4& v) noexcept
    {
        return v.x * m.columns[0] +
               v.y * m.columns[1] +
               v.z * m.columns[2] +
               v.w * m.columns[3];
    }

    inline float4x4 mul(const float4x4& a, const float4x4& b) noexcept
    {
        float4x4 result;
        result.columns[0] = mul(a, b.columns[0]);
        result.columns[1] = mul(a, b.columns[1]);
        result.columns[2] = mul(a, b.columns[2]);
        result.columns[3] = mul(a, b.columns[3]);
        return result;
    }

    inline float4 operator*(const float4x4& m, const float4& v) noexcept { return mul(m, v); }
    inline float4x4 operator*(const float4x4& a, const float4x4& b) noexcept { return mul(a, b); }

    // ========================================
    // Transform Implementations
    // ========================================
    inline float4x4 float4x4::translate(float3 t) noexcept
    {
        return float4x4{
            float4{ 1.f, 0.f, 0.f, 0.f },
            float4{ 0.f, 1.f, 0.f, 0.f },
            float4{ 0.f, 0.f, 1.f, 0.f },
            float4{ t.x, t.y, t.z, 1.f }
        };
    }

    inline float4x4 float4x4::scale(float3 s) noexcept
    {
        return float4x4{
            float4{ s.x, 0.f, 0.f, 0.f },
            float4{ 0.f, s.y, 0.f, 0.f },
            float4{ 0.f, 0.f, s.z, 0.f },
            float4{ 0.f, 0.f, 0.f, 1.f }
        };
    }

    inline float4x4 float4x4::rotate_x(const float rad) noexcept
    {
        const float c{ std::cos(rad) };
        const float s{ std::sin(rad) };

        return float4x4{
            float4{ 1.f, 0.f, 0.f, 0.f },
            float4{ 0.f, c, s, 0.f },
            float4{ 0.f, -s, c, 0.f },
            float4{ 0.f, 0.f, 0.f, 1.f }
        };
    }

    inline float4x4 float4x4::rotate_y(const float rad) noexcept
    {
        const float c{ std::cos(rad) };
        const float s{ std::sin(rad) };

        return float4x4{
            float4{ c, 0.f, -s, 0.f },
            float4{ 0.f, 1.f, 0.f, 0.f },
            float4{ s, 0.f, c, 0.f },
            float4{ 0.f, 0.f, 0.f, 1.f }
        };
    }

    inline float4x4 float4x4::rotate_z(const float rad) noexcept
    {
        const float c{ std::cos(rad) };
        const float s{ std::sin(rad) };

        return float4x4{
            float4{ c, s, 0.f, 0.f },
            float4{ -s, c, 0.f, 0.f },
            float4{ 0.f, 0.f, 1.f, 0.f },
            float4{ 0.f, 0.f, 0.f, 1.f }
        };
    }

    inline float4x4 float4x4::rotate_axis_angle(float3 axis, const float rad) noexcept
    {
        axis = normalize(axis);
        const float c{ std::cos(rad) };
        const float s{ std::sin(rad) };
        const float t{ 1.0f - c };

        const float x{ axis.x };
        const float y{ axis.y };
        const float z{ axis.z };

        return float4x4{
            float4{ t * x * x + c, t * x * y + s * z, t * x * z - s * y, 0.f },
            float4{ t * x * y - s * z, t * y * y + c, t * y * z + s * x, 0.f },
            float4{ t * x * z + s * y, t * y * z - s * x, t * z * z + c, 0.f },
            float4{ 0.f, 0.f, 0.f, 1.f }
        };
    }

    inline float4x4 float4x4::look_at_lh(const float3 eye, const float3 target, const float3 up) noexcept
    {
        float3 z{ normalize(target - eye) };
        float3 x{ normalize(cross(up, z)) };
        float3 y{ cross(z, x) };

        return float4x4{
            float4{ x.x, y.x, z.x, 0.f },
            float4{ x.y, y.y, z.y, 0.f },
            float4{ x.z, y.z, z.z, 0.f },
            float4{ -dot(x, eye), -dot(y, eye), -dot(z, eye), 1.f }
        };
    }

    inline float4x4 float4x4::look_at_rh(const float3 eye, const float3 target, const float3 up) noexcept
    {
        float3 z{ normalize(eye - target) }; // reversed direction
        float3 x{ normalize(cross(up, z)) };
        float3 y{ cross(z, x) };

        return float4x4{
            float4{ x.x, y.x, z.x, 0.f },
            float4{ x.y, y.y, z.y, 0.f },
            float4{ x.z, y.z, z.z, 0.f },
            float4{ -dot(x, eye), -dot(y, eye), -dot(z, eye), 1.f }
        };
    }

    inline float4x4 float4x4::perspective_lh(const float fov_y_rad, const float aspect, const float z_near,
                                             const float z_far) noexcept
    {
        const float h{ 1.f / std::tan(fov_y_rad * .5f) };
        const float w{ h / aspect };
        const float d{ z_far / (z_far - z_near) };

        return float4x4{
            float4{ w, 0.f, 0.f, 0.f },
            float4{ 0.f, h, 0.f, 0.f },
            float4{ 0.f, 0.f, d, 1.f },
            float4{ 0.f, 0.f, -d * z_near, 0.f }
        };
    }

    inline float4x4 float4x4::perspective_rh(const float fov_y_rad, const float aspect, const float z_near,
                                             const float z_far) noexcept
    {
        const float h{ 1.f / std::tan(fov_y_rad * .5f) };
        const float w{ h / aspect };
        const float d{ z_far / (z_near - z_far) }; // note sign flip

        return float4x4{
            float4{ w, 0.f, 0.f, 0.f },
            float4{ 0.f, h, 0.f, 0.f },
            float4{ 0.f, 0.f, d, -1.f }, // flipped
            float4{ 0.f, 0.f, d * z_near, 0.f }
        };
    }

    inline float4x4 float4x4::ortho_lh(const float width, const float height, const float z_near,
                                       const float z_far) noexcept
    {
        const float r{ width * .5f };
        const float t{ height * .5f };

        return float4x4{
            float4{ 1.f / r, 0.f, 0.f, 0.f },
            float4{ 0.f, 1.f / t, 0.f, 0.f },
            float4{ 0.f, 0.f, 1.f / (z_far - z_near), 0.f },
            float4{ 0.f, 0.f, z_near / (z_near - z_far), 1.f }
        };
    }

    inline float4x4 float4x4::ortho_rh(const float width, const float height, const float z_near,
                                       const float z_far) noexcept
    {
        const float r{ width * .5f };
        const float t{ height * .5f };

        return float4x4{
            float4{ 1.f / r, 0.f, 0.f, 0.f },
            float4{ 0.f, 1.f / t, 0.f, 0.f },
            float4{ 0.f, 0.f, -2.f / (z_far - z_near), 0.f },
            float4{ 0.f, 0.f, (z_far + z_near) / (z_near - z_far), 1.f }
        };
    }
} // namespace chlm
