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

        /**
         * @brief Accesses a column of the matrix for reading or writing.
         *
         * @param i Column index (0 = X/right, 1 = Y/up, 2 = Z/forward).
         * @return Reference to the specified column vector.
         *
         * @note Index must be 0, 1, or 2. Behavior is undefined otherwise
         *       (assertion fires in debug builds).
         */
        float3& operator[](const int i)
        {
            assert(i >= 0 && i < 3);
            return columns[i];
        }

        /**
         * @brief Accesses a column of the matrix for reading (const version).
         *
         * @param i Column index (0 = X/right, 1 = Y/up, 2 = Z/forward).
         * @return Const reference to the specified column vector.
         */
        const float3& operator[](const int i) const
        {
            assert(i >= 0 && i < 3);
            return columns[i];
        }

        /**
         * @brief Returns the identity rotation matrix.
         *
         * @return 3x3 identity matrix.
         */
        static constexpr float3x3 identity() noexcept { return { }; }

        /**
         * @brief Converts a unit quaternion to a 3x3 rotation matrix.
         *
         * Internal helper — public version is free function quat_to_float3x3().
         * Assumes quaternion is normalized.
         *
         * @param q Unit quaternion.
         * @return Equivalent rotation matrix.
         */
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

        /**
         * @brief Creates a rotation matrix around the X axis.
         *
         * @param rad Angle in radians.
         * @return Rotation matrix (right-handed, positive angle = counterclockwise).
         */
        static float3x3 rotate_x(float rad) noexcept;

        /**
         * @brief Creates a rotation matrix around the Y axis.
         *
         * @param rad Angle in radians.
         * @return Rotation matrix (right-handed, positive angle = counterclockwise).
         */
        static float3x3 rotate_y(float rad) noexcept;

        /**
         * @brief Creates a rotation matrix around the Z axis.
         *
         * @param rad Angle in radians.
         * @return Rotation matrix (right-handed, positive angle = counterclockwise).
         */
        static float3x3 rotate_z(float rad) noexcept;

        /**
         * @brief Creates a rotation matrix from an axis and angle.
         *
         * Axis must be normalized for correct results.
         *
         * @param axis Normalized rotation axis.
         * @param rad  Angle in radians (right-handed).
         * @return Rotation matrix.
         */
        static float3x3 rotate_axis_angle(float3 axis, float rad) noexcept;
    };

    // ========================================
    // Multiplication
    // ========================================

    /**
     * @brief Multiplies a 3x3 matrix by a 3D vector.
     *
     * Performs rotation/orientation of the vector.
     *
     * @param m Matrix (column-major).
     * @param v Vector to transform.
     * @return Transformed vector.
     */
    inline float3 mul(const float3x3& m, const float3& v) noexcept
    {
        return v.x * m.columns[0] +
               v.y * m.columns[1] +
               v.z * m.columns[2];
    }

    /**
     * @brief Multiplies two 3x3 matrices.
     *
     * Result represents b applied after a (i.e., a then b).
     *
     * @param a First matrix (applied first).
     * @param b Second matrix (applied after).
     * @return Composed matrix.
     */
    inline float3x3 mul(const float3x3& a, const float3x3& b) noexcept
    {
        float3x3 result;
        result.columns[0] = mul(a, b.columns[0]);
        result.columns[1] = mul(a, b.columns[1]);
        result.columns[2] = mul(a, b.columns[2]);
        return result;
    }

    /**
     * @brief Matrix-vector multiplication operator.
     */
    inline float3 operator*(const float3x3& m, const float3& v) noexcept { return mul(m, v); }

    /**
     * @brief Matrix-matrix multiplication operator.
     */
    inline float3x3 operator*(const float3x3& a, const float3x3& b) noexcept { return mul(a, b); }

    // ========================================
    // Inverse (for rotation matrices: transpose = inverse)
    // Fast path — assumes orthonormal (pure rotation)
    // ========================================

    /**
     * @brief Computes the transpose of a 3x3 matrix.
     *
     * For pure rotation matrices, transpose equals inverse.
     *
     * @param m Input matrix.
     * @return Transposed matrix.
     */
    inline float3x3 transpose(const float3x3& m) noexcept
    {
        return float3x3{
            float3{ m[0].x, m[1].x, m[2].x },
            float3{ m[0].y, m[1].y, m[2].y },
            float3{ m[0].z, m[1].z, m[2].z }
        };
    }

    /**
     * @brief Computes the inverse of a rotation matrix.
     *
     * Fast path: returns transpose (valid only for orthonormal matrices with det = 1).
     *
     * @param m Orthonormal rotation matrix.
     * @return Inverse matrix (equivalent to transpose).
     */
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
