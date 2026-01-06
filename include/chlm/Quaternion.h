//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySofty. All rights reserved.
//

#pragma once

#include "Core.h"

namespace chlm {
    // ========================================
    // Quaternion - uses float4 (x,y,z,w)
    // Vector part: xyz, Scalar part: w
    // ========================================
    using quat = float4;

    // Constructors / constants
    constexpr quat quat_identity() noexcept { return quat{ 0.0f, 0.0f, 0.0f, 1.0f }; }

    // Create from axis-angle (axis must be normalized)
    inline quat quat_from_axis_angle(float3 axis, float rad) noexcept
    {
        float half = rad * 0.5f;
        float s = std::sin(half);
        return quat{ axis.x * s, axis.y * s, axis.z * s, std::cos(half) };
    }

    // Create from Euler angles (YPR: yaw Z, pitch X, roll Y — DirectX order)
    inline quat quat_from_euler(float yaw_z, float pitch_x, float roll_y) noexcept
    {
        float cy = std::cos(yaw_z * 0.5f);
        float sy = std::sin(yaw_z * 0.5f);
        float cp = std::cos(pitch_x * 0.5f);
        float sp = std::sin(pitch_x * 0.5f);
        float cr = std::cos(roll_y * 0.5f);
        float sr = std::sin(roll_y * 0.5f);

        return quat{
            sr * cp * cy - cr * sp * sy, // x
            cr * sp * cy + sr * cp * sy, // y
            cr * cp * sy - sr * sp * cy, // z
            cr * cp * cy + sr * sp * sy // w
        };
    }

    // Quaternion multiplication (Hamilton product)
    inline quat mul(const quat& a, const quat& b) noexcept
    {
        return quat{
            a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
            a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
            a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
            a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
        };
    }

    // Conjugate (for unit quaternions, inverse = conjugate)
    inline quat conjugate(const quat& q) noexcept
    {
        return quat{ -q.x, -q.y, -q.z, q.w };
    }

    // Inverse (safe for non-unit — but assume normalized in hot paths)
    inline quat inverse(const quat& q) noexcept
    {
        float len_sq = dot(q, q);
        if (almost_equal(len_sq, 0.0f)) return quat_identity();
        quat conj = conjugate(q);
        return conj * (1.0f / len_sq);
    }

    // Normalized linear interpolation (nlerp) — fast but less accurate
    inline quat nlerp(const quat& a, const quat& b, float t) noexcept
    {
        return normalize(a + (b - a) * t);
    }

    // Spherical linear interpolation (slerp) — constant angular velocity
    inline quat slerp(const quat& a, const quat& b, float t) noexcept
    {
        float d = dot(a, b);
        quat b_adj = b;

        // If dot is negative, slerp won't take shortest path — flip one
        if (d < 0.0f)
        {
            b_adj = -b;
            d = -d;
        }

        // If quats are very close, fall back to nlerp
        if (d > 0.9995f)
        {
            return nlerp(a, b_adj, t);
        }

        float theta = std::acos(clamp(d, -1.0f, 1.0f));
        float sine = std::sin(theta);

        float wa = std::sin((1.0f - t) * theta) / sine;
        float wb = std::sin(t * theta) / sine;

        return a * wa + b_adj * wb;
    }

    // Rotate a vector by quaternion (q * v * q^{-1})
    inline float3 rotate_vector(const quat& q, float3 v) noexcept
    {
        quat vq{ v.x, v.y, v.z, 0.0f };
        quat q_inv = conjugate(q); // assumes q is normalized
        quat result = mul(mul(q, vq), q_inv);
        return result.xyz;
    }
} // namespace chlm
