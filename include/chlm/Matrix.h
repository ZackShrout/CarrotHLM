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

        /**
         * @brief Accesses a column of the matrix for reading or writing.
         *
         * @param i Column index (0=X/right, 1=Y/up, 2=Z/forward, 3=translation).
         * @return Reference to the specified column.
         *
         * @note Index must be 0–3. Asserts in debug builds on out-of-bounds access.
         */
        float4& operator[](const int i)
        {
            assert(i >= 0 && i < 4);
            return columns[i];
        }

        /**
         * @brief Accesses a column of the matrix for reading (const version).
         *
         * @param i Column index (0–3).
         * @return Const reference to the specified column.
         */
        const float4& operator[](const int i) const
        {
            assert(i >= 0 && i < 4);
            return columns[i];
        }

        /**
         * @brief Returns the identity matrix.
         *
         * @return 4x4 identity matrix.
         */
        static constexpr float4x4 identity() noexcept { return { }; }

        /**
         * @brief Creates a translation matrix.
         *
         * @param t Translation vector.
         * @return Translation matrix.
         */
        static float4x4 translate(float3 t) noexcept;

        /**
         * @brief Creates a scale matrix.
         *
         * @param s Scale vector (per-axis).
         * @return Scale matrix.
         */
        static float4x4 scale(float3 s) noexcept;

        /**
         * @brief Creates a rotation matrix around the X axis.
         *
         * @param rad Angle in radians (right-handed).
         * @return Rotation matrix.
         */
        static float4x4 rotate_x(float rad) noexcept;

        /**
         * @brief Creates a rotation matrix around the Y axis.
         *
         * @param rad Angle in radians (right-handed).
         * @return Rotation matrix.
         */
        static float4x4 rotate_y(float rad) noexcept;

        /**
         * @brief Creates a rotation matrix around the Z axis.
         *
         * @param rad Angle in radians (right-handed).
         * @return Rotation matrix.
         */
        static float4x4 rotate_z(float rad) noexcept;

        /**
         * @brief Creates a rotation matrix from axis and angle.
         *
         * @param axis Normalized rotation axis.
         * @param rad Angle in radians (right-handed).
         * @return Rotation matrix.
         */
        static float4x4 rotate_axis_angle(float3 axis, float rad) noexcept;

        /**
         * @brief Creates a left-handed look-at view matrix.
         *
         * @param eye    Camera position.
         * @param target Look target position.
         * @param up     Up vector (usually {0,1,0}).
         * @return View matrix (+Z forward).
         */
        static float4x4 look_at_lh(float3 eye, float3 target, float3 up) noexcept;

        /**
         * @brief Creates a right-handed look-at view matrix.
         *
         * @param eye    Camera position.
         * @param target Look target position.
         * @param up     Up vector (usually {0,1,0}).
         * @return View matrix (-Z forward).
         */
        static float4x4 look_at_rh(float3 eye, float3 target, float3 up) noexcept;

        /**
         * @brief Creates a left-handed perspective projection matrix.
         *
         * @param fov_y_rad Vertical field of view in radians.
         * @param aspect    Aspect ratio (width / height).
         * @param z_near    Near clip plane distance (>0).
         * @param z_far     Far clip plane distance (> z_near).
         * @return Projection matrix (+Z forward, [0,1] depth).
         */
        static float4x4 perspective_lh(float fov_y_rad, float aspect, float z_near, float z_far) noexcept;

        /**
         * @brief Creates a right-handed perspective projection matrix.
         *
         * @param fov_y_rad Vertical field of view in radians.
         * @param aspect    Aspect ratio (width / height).
         * @param z_near    Near clip plane distance (>0).
         * @param z_far     Far clip plane distance (> z_near).
         * @return Projection matrix (-Z forward, [0,1] depth).
         */
        static float4x4 perspective_rh(float fov_y_rad, float aspect, float z_near, float z_far) noexcept;

        /**
         * @brief Creates a left-handed orthographic projection matrix.
         *
         * @param width  View width.
         * @param height View height.
         * @param z_near Near clip plane.
         * @param z_far  Far clip plane.
         * @return Orthographic matrix centered at origin.
         */
        static float4x4 ortho_lh(float width, float height, float z_near, float z_far) noexcept;

        /**
         * @brief Creates a right-handed orthographic projection matrix.
         *
         * @param width  View width.
         * @param height View height.
         * @param z_near Near clip plane.
         * @param z_far  Far clip plane.
         * @return Orthographic matrix centered at origin.
         */
        static float4x4 ortho_rh(float width, float height, float z_near, float z_far) noexcept;
    };

    // ========================================
    // Multiplication (HLSL order: mul(M, v) = matrix * vector)
    // ========================================

    /**
     * @brief Multiplies a 4x4 matrix by a 4D vector.
     *
     * @param m Matrix.
     * @param v Vector (treated as column vector).
     * @return Transformed vector.
     */
    inline float4 mul(const float4x4& m, const float4& v) noexcept
    {
        return v.x * m.columns[0] +
               v.y * m.columns[1] +
               v.z * m.columns[2] +
               v.w * m.columns[3];
    }

    /**
     * @brief Multiplies two 4x4 matrices.
     *
     * @param a First matrix (applied first).
     * @param b Second matrix (applied after a).
     * @return Composed matrix (a then b).
     */
    inline float4x4 mul(const float4x4& a, const float4x4& b) noexcept
    {
        float4x4 result;
        result.columns[0] = mul(a, b.columns[0]);
        result.columns[1] = mul(a, b.columns[1]);
        result.columns[2] = mul(a, b.columns[2]);
        result.columns[3] = mul(a, b.columns[3]);
        return result;
    }

    /**
     * @brief Matrix-vector multiplication operator.
     */
    inline float4 operator*(const float4x4& m, const float4& v) noexcept { return mul(m, v); }

    /**
     * @brief Matrix-matrix multiplication operator.
     */
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
