//
// Created by zshrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Core.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"

#include <future>

namespace chlm {
    /**
     * @brief Converts a unit quaternion to a 4x4 rotation matrix.
     *
     * The resulting matrix contains only rotation (no translation or scale).
     * The bottom row is always {0, 0, 0, 1}.
     *
     * @param q Unit quaternion (assumed normalized).
     * @return 4x4 homogeneous rotation matrix.
     */
    constexpr float4x4 to_float4x4(const quat& q) noexcept
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

        return float4x4{
            float4{ 1.f - 2.f * (yy + zz), 2.f * (xy - wz), 2.f * (xz + wy), 0.f },
            float4{ 2.f * (xy + wz), 1.f - 2.f * (xx + zz), 2.f * (yz - wx), 0.f },
            float4{ 2.f * (xz - wy), 2.f * (yz + wx), 1.f - 2.f * (xx + yy), 0.f },
            float4{ 0.f, 0.f, 0.f, 1.f }
        };
    }

    /**
     * @brief Converts a unit quaternion to a 3x3 rotation matrix.
     *
     * Equivalent to the upper 3x3 part of to_float4x4(q).
     *
     * @param q Unit quaternion (assumed normalized).
     * @return 3x3 rotation matrix.
     */
    constexpr float3x3 to_float3x3(const quat& q) noexcept
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
     * @brief Extracts a quaternion from a 3x3 rotation matrix.
     *
     * Robust conversion that handles all cases (including singular trace).
     * The input matrix must be orthonormal (pure rotation, det = 1).
     * The resulting quaternion is normalized.
     *
     * @param m Orthonormal 3x3 rotation matrix.
     * @return Equivalent unit quaternion.
     */
    constexpr quat quat_from_float3x3(const float3x3& m) noexcept
    {
        quat q;

        if (const float trace{ m[0].x + m[1].y + m[2].z }; trace > 0.0f)
        {
            const float s{ std::sqrt(trace + 1.0f) * 2.0f };
            q.w = .25f * s;
            q.x = (m[2].y - m[1].z) / s;
            q.y = (m[0].z - m[2].x) / s;
            q.z = (m[1].x - m[0].y) / s;
        }
        else if (m[0].x > m[1].y && m[0].x > m[2].z)
        {
            const float s{ std::sqrt(1.f + m[0].x - m[1].y - m[2].z) * 2.f };
            q.w = (m[2].y - m[1].z) / s;
            q.x = .25f * s;
            q.y = (m[0].y + m[1].x) / s;
            q.z = (m[0].z + m[2].x) / s;
        }
        else if (m[1].y > m[2].z)
        {
            const float s{ std::sqrt(1.f + m[1].y - m[0].x - m[2].z) * 2.f };
            q.w = (m[0].z - m[2].x) / s;
            q.x = (m[0].y + m[1].x) / s;
            q.y = .25f * s;
            q.z = (m[1].z + m[2].y) / s;
        }
        else
        {
            const float s{ std::sqrt(1.f + m[2].z - m[0].x - m[1].y) * 2.f };
            q.w = (m[1].x - m[0].y) / s;
            q.x = (m[0].z + m[2].x) / s;
            q.y = (m[1].z + m[2].y) / s;
            q.z = .25f * s;
        }

        return q;
    }

    /**
     * @brief Computes the transpose of a 4x4 matrix.
     *
     * @param m Input matrix.
     * @return Transposed matrix (rows become columns).
     */
    constexpr float4x4 transpose(const float4x4& m) noexcept
    {
        return float4x4{
            float4{ m[0].x, m[1].x, m[2].x, m[3].x },
            float4{ m[0].y, m[1].y, m[2].y, m[3].y },
            float4{ m[0].z, m[1].z, m[2].z, m[3].z },
            float4{ m[0].w, m[1].w, m[2].w, m[3].w }
        };
    }

    /**
     * @brief Fast inverse for affine transformation matrices.
     *
     * Assumes the matrix is composed of rotation (or rotation + uniform scale),
     * translation, and no shear or projection - typical for object/model matrices.
     * Much faster than general inverse.
     *
     * @param m Affine transformation matrix.
     * @return Inverse matrix such that m * affine_inverse(m) ≈ identity.
     */
    inline float4x4 affine_inverse(const float4x4& m) noexcept
    {
        const float3x3 rot{
            m.columns[0].xyz,
            m.columns[1].xyz,
            m.columns[2].xyz
        };

        float3x3 rot_inv{ inverse_orthonormal(rot) };

        const float3 trans{ m.columns[3].xyz };
        float3 inv_trans{ -mul(rot_inv, trans) };

        return float4x4{
            float4{ rot_inv.columns[0].x, rot_inv.columns[0].y, rot_inv.columns[0].z, 0.f },
            float4{ rot_inv.columns[1].x, rot_inv.columns[1].y, rot_inv.columns[1].z, 0.f },
            float4{ rot_inv.columns[2].x, rot_inv.columns[2].y, rot_inv.columns[2].z, 0.f },
            float4{ inv_trans.x, inv_trans.y, inv_trans.z, 1.f }
        };
    }

    /**
     * @brief Computes the normal matrix from a 4x4 transformation.
     *
     * The normal matrix is the inverse transpose of the upper 3x3 part.
     * Used to correctly transform surface normals when non-uniform scaling is present.
     *
     * @param m Transformation matrix (model matrix).
     * @return 3x3 matrix for transforming normals.
     */
    constexpr float3x3 normal_matrix(const float4x4& m) noexcept
    {
        const float3x3 upper{
            m.columns[0].xyz,
            m.columns[1].xyz,
            m.columns[2].xyz
        };

        return inverse_orthonormal(upper);
    }

    /**
     * @brief Computes the general inverse of a 4x4 matrix.
     *
     * Uses Gauss-Jordan elimination with partial pivoting for numerical stability.
     * This is a full inverse that works for any invertible 4x4 matrix (including
     * those with projection, shear, or non-uniform scaling).
     *
     * @note This is significantly slower than affine_inverse() or assuming an
     *       orthonormal basis. Use only when a full inverse is required
     *       (e.g. for view-projection matrices or arbitrary transformations).
     *       For model matrices, prefer affine_inverse().
     *
     * @param m The matrix to invert. Must be invertible.
     * @return The inverse matrix such that m * inverse(m) ≈ identity.
     *         If the matrix is singular (determinant ≈ 0), returns the identity
     *         matrix as a safe fallback.
     */
    constexpr float4x4 inverse(const float4x4& m) noexcept
    {
        // Copy matrix into flat scalar array (column-major -> row-major transpose for row ops)
        float a[4][4];
        a[0][0] = m[0].x;  a[0][1] = m[1].x;  a[0][2] = m[2].x;  a[0][3] = m[3].x;
        a[1][0] = m[0].y;  a[1][1] = m[1].y;  a[1][2] = m[2].y;  a[1][3] = m[3].y;
        a[2][0] = m[0].z;  a[2][1] = m[1].z;  a[2][2] = m[2].z;  a[2][3] = m[3].z;
        a[3][0] = m[0].w;  a[3][1] = m[1].w;  a[3][2] = m[2].w;  a[3][3] = m[3].w;

        // Augmented identity
        float inv[4][4]{
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };

        for (int i{ 0 }; i < 4; ++i)
        {
            // Partial pivoting
            int pivot{ i };
            float max_val{ std::abs(a[i][i]) };
            for (int j = i + 1; j < 4; ++j)
            {
                if (const float val{ std::abs(a[j][i]) }; val > max_val)
                {
                    max_val = val;
                    pivot = j;
                }
            }

            if (max_val < epsilon)
                return float4x4::identity();  // Singular

            // Swap rows
            if (pivot != i)
            {
                for (int k{ 0 }; k < 4; ++k)
                {
                    std::swap(a[i][k], a[pivot][k]);
                    std::swap(inv[i][k], inv[pivot][k]);
                }
            }

            // Normalize pivot row
            const float inv_pivot{ 1.0f / a[i][i] };
            for (int k{ 0 }; k < 4; ++k)
            {
                a[i][k] *= inv_pivot;
                inv[i][k] *= inv_pivot;
            }

            // Eliminate column
            for (int j{ 0 }; j < 4; ++j)
            {
                if (j == i) continue;
                const float factor{ a[j][i] };
                for (int k{ 0 }; k < 4; ++k)
                {
                    a[j][k] -= a[i][k] * factor;
                    inv[j][k] -= inv[i][k] * factor;
                }
            }
        }

        // Transpose inv back into column-major float4x4
        return {
            float4{ inv[0][0], inv[1][0], inv[2][0], inv[3][0] },
            float4{ inv[0][1], inv[1][1], inv[2][1], inv[3][1] },
            float4{ inv[0][2], inv[1][2], inv[2][2], inv[3][2] },
            float4{ inv[0][3], inv[1][3], inv[2][3], inv[3][3] }
        };
    }
} // namespace chlm
