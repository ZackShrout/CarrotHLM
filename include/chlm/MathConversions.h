//
// Created by zshrout on 1/5/26.
// Copyright (c) 2026 BunnySofty. All rights reserved.
//

#pragma once

#include "Core.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Matrix3x3.h"

namespace chlm {

    // quat → float4x4
    inline float4x4 to_float4x4(const quat& q) noexcept {
        float xx = q.x * q.x;
        float yy = q.y * q.y;
        float zz = q.z * q.z;
        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float yz = q.y * q.z;
        float wx = q.w * q.x;
        float wy = q.w * q.y;
        float wz = q.w * q.z;

        return float4x4{
            float4{1.0f - 2.0f*(yy + zz), 2.0f*(xy - wz),         2.0f*(xz + wy),         0.0f},
            float4{2.0f*(xy + wz),         1.0f - 2.0f*(xx + zz), 2.0f*(yz - wx),         0.0f},
            float4{2.0f*(xz - wy),         2.0f*(yz + wx),         1.0f - 2.0f*(xx + yy), 0.0f},
            float4{0.0f,                   0.0f,                   0.0f,                   1.0f}
        };
    }

    // quat → float3x3
    inline float3x3 to_float3x3(const quat& q) noexcept {
        float xx = q.x * q.x;
        float yy = q.y * q.y;
        float zz = q.z * q.z;
        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float yz = q.y * q.z;
        float wx = q.w * q.x;
        float wy = q.w * q.y;
        float wz = q.w * q.z;

        return float3x3{
            float3{1.0f - 2.0f*(yy + zz), 2.0f*(xy - wz),         2.0f*(xz + wy)},
            float3{2.0f*(xy + wz),         1.0f - 2.0f*(xx + zz), 2.0f*(yz - wx)},
            float3{2.0f*(xz - wy),         2.0f*(yz + wx),         1.0f - 2.0f*(xx + yy)}
        };
    }

    // float3x3 → quat (assumes matrix is pure rotation)
    inline quat quat_from_float3x3(const float3x3& m) noexcept {
        quat q;
        float trace = m[0].x + m[1].y + m[2].z;

        if (trace > 0.0f) {
            float s = std::sqrt(trace + 1.0f) * 2.0f;
            q.w = 0.25f * s;
            q.x = (m[2].y - m[1].z) / s;
            q.y = (m[0].z - m[2].x) / s;
            q.z = (m[1].x - m[0].y) / s;
        } else if (m[0].x > m[1].y && m[0].x > m[2].z) {
            float s = std::sqrt(1.0f + m[0].x - m[1].y - m[2].z) * 2.0f;
            q.w = (m[2].y - m[1].z) / s;
            q.x = 0.25f * s;
            q.y = (m[0].y + m[1].x) / s;
            q.z = (m[0].z + m[2].x) / s;
        } else if (m[1].y > m[2].z) {
            float s = std::sqrt(1.0f + m[1].y - m[0].x - m[2].z) * 2.0f;
            q.w = (m[0].z - m[2].x) / s;
            q.x = (m[0].y + m[1].x) / s;
            q.y = 0.25f * s;
            q.z = (m[1].z + m[2].y) / s;
        } else {
            float s = std::sqrt(1.0f + m[2].z - m[0].x - m[1].y) * 2.0f;
            q.w = (m[1].x - m[0].y) / s;
            q.x = (m[0].z + m[2].x) / s;
            q.y = (m[1].z + m[2].y) / s;
            q.z = 0.25f * s;
        }

        return q;
    }

    // Transpose float4x4
    inline float4x4 transpose(const float4x4& m) noexcept {
        return float4x4{
            float4{m[0].x, m[1].x, m[2].x, m[3].x},
            float4{m[0].y, m[1].y, m[2].y, m[3].y},
            float4{m[0].z, m[1].z, m[2].z, m[3].z},
            float4{m[0].w, m[1].w, m[2].w, m[3].w}
        };
    }

    // Fast affine inverse (rotation + translation + uniform scale)
    // Assumes no shear, no projection — common for model matrices
    inline float4x4 affine_inverse(const float4x4& m) noexcept {
        // Extract 3x3 rotation part
        float3x3 rot{
            m.columns[0].xyz,
            m.columns[1].xyz,
            m.columns[2].xyz
        };

        float3x3 rot_inv = inverse(rot);  // transpose for rotation

        float3 trans = m.columns[3].xyz;
        float3 inv_trans = -mul(rot_inv, trans);  // ← use mul instead of rotate_vector

        return float4x4{
            float4{rot_inv.columns[0].x, rot_inv.columns[0].y, rot_inv.columns[0].z, 0.0f},
            float4{rot_inv.columns[1].x, rot_inv.columns[1].y, rot_inv.columns[1].z, 0.0f},
            float4{rot_inv.columns[2].x, rot_inv.columns[2].y, rot_inv.columns[2].z, 0.0f},
            float4{inv_trans.x,          inv_trans.y,          inv_trans.z,          1.0f}
        };
    }

    // Normal matrix = inverse transpose of 3x3 rotation part
    // Used to transform normals correctly under non-uniform scale
    inline float3x3 normal_matrix(const float4x4& m) noexcept {
        float3x3 upper{
            m.columns[0].xyz,
            m.columns[1].xyz,
            m.columns[2].xyz
        };
        return inverse(upper);
    }

    // Optional: full general inverse (slow, use only when needed)
    // Uses Gauss-Jordan — not recommended for hot paths
    // inline float4x4 general_inverse(const float4x4& m) noexcept { ... }
} // namespace chlm