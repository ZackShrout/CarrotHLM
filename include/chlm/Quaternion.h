//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Core.h"

namespace chlm {
    // ========================================
    // Quaternion - uses float4 (x,y,z,w)
    // Vector part: xyz, Scalar part: w
    // ========================================
    using quat = float4;

    /**
     * @brief Returns the identity quaternion.
     *
     * Represents no rotation (equivalent to a zero angle around any axis).
     *
     * @return Quaternion {0, 0, 0, 1}.
     */
    constexpr quat quat_identity() noexcept { return quat{ 0.f, 0.f, 0.f, 1.f }; }

    /**
     * @brief Creates a quaternion from a normalized axis and an angle.
     *
     * The axis must be unit-length for correct results.
     *
     * @param axis Normalized rotation axis.
     * @param rad Rotation angle in radians.
     * @return Quaternion representing the axis-angle rotation.
     */
    inline quat quat_from_axis_angle(float3 axis, const float rad) noexcept
    {
        const float half{ rad * .5f };
        const float s{ std::sin(half) };

        return quat{ axis.x * s, axis.y * s, axis.z * s, std::cos(half) };
    }

    /**
     * @brief Creates a quaternion from Euler angles (yaw, pitch, roll).
     *
     * Uses DirectX/YZX order: yaw around Z, then pitch around X, then roll around Y.
     * Angles are in radians.
     *
     * @param yaw_z   Yaw rotation around Z axis (in radians).
     * @param pitch_x Pitch rotation around X axis (in radians).
     * @param roll_y  Roll rotation around Y axis (in radians).
     * @return Quaternion representing the combined rotation.
     */
    inline quat quat_from_euler(const float yaw_z, const float pitch_x, const float roll_y) noexcept
    {
        const float cy{ std::cos(yaw_z * .5f) };
        const float sy{ std::sin(yaw_z * .5f) };
        const float cp{ std::cos(pitch_x * .5f) };
        const float sp{ std::sin(pitch_x * .5f) };
        const float cr{ std::cos(roll_y * .5f) };
        const float sr{ std::sin(roll_y * .5f) };

        return quat{
            sr * cp * cy - cr * sp * sy, // x
            cr * sp * cy + sr * cp * sy, // y
            cr * cp * sy - sr * sp * cy, // z
            cr * cp * cy + sr * sp * sy // w
        };
    }

    /**
     * @brief Multiplies two quaternions (Hamilton product).
     *
     * Equivalent to combining two rotations: result = a followed by b.
     *
     * @param a First quaternion (applied first in rotation order).
     * @param b Second quaternion (applied after a).
     * @return Quaternion representing the composed rotation.
     */
    inline quat mul(const quat& a, const quat& b) noexcept
    {
        return quat{
            a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
            a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
            a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
            a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
        };
    }

    /**
     * @brief Computes the conjugate of a quaternion.
     *
     * For unit quaternions, the conjugate is equal to the inverse.
     *
     * @param q Input quaternion.
     * @return Conjugate quaternion {-x, -y, -z, w}.
     */
    inline quat conjugate(const quat& q) noexcept
    {
        return quat{ -q.x, -q.y, -q.z, q.w };
    }

    /**
     * @brief Computes the inverse of a quaternion.
     *
     * Safe for non-unit quaternions. Returns identity if quaternion is zero-length.
     *
     * @param q Input quaternion.
     * @return Inverse quaternion such that q * inverse(q) = identity.
     */
    inline quat inverse(const quat& q) noexcept
    {
        const float len_sq{ dot(q, q) };

        if (almost_equal(len_sq, .0f)) return quat_identity();

        const quat conj{ conjugate(q) };
        return conj * (1.f / len_sq);
    }

    /**
     * @brief Normalized linear interpolation between two quaternions.
     *
     * Fast but does not guarantee constant angular velocity.
     * Result is always normalized.
     *
     * @param a Start quaternion.
     * @param b End quaternion.
     * @param t Interpolation factor (0 = a, 1 = b).
     * @return Interpolated and normalized quaternion.
     */
    inline quat nlerp(const quat& a, const quat& b, const float t) noexcept
    {
        return normalize(a + (b - a) * t);
    }

    /**
     * @brief Spherical linear interpolation between two quaternions.
     *
     * Provides constant angular velocity along the shortest arc.
     * Automatically handles the shortest path and falls back to nlerp when quaternions are nearly identical.
     *
     * @param a Start quaternion (should be unit-length).
     * @param b End quaternion (should be unit-length).
     * @param t Interpolation factor (0 = a, 1 = b).
     * @return Interpolated quaternion along the great circle arc.
     */
    inline quat slerp(const quat& a, const quat& b, const float t) noexcept
    {
        float d{ dot(a, b) };
        quat b_adj{ b };

        // If dot is negative, slerp won't take shortest path - flip one
        if (d < 0.f)
        {
            b_adj = -b;
            d = -d;
        }

        // If quats are very close, fall back to nlerp
        if (d > .9995f)
        {
            return nlerp(a, b_adj, t);
        }

        const float theta{ std::acos(clamp(d, -1.f, 1.f)) };
        const float sine{ std::sin(theta) };

        const float wa{ std::sin((1.f - t) * theta) / sine };
        const float wb{ std::sin(t * theta) / sine };

        return a * wa + b_adj * wb;
    }

    /**
     * @brief Rotates a 3D vector by a quaternion.
     *
     * Uses the formula q * v * q⁻¹. Assumes the quaternion is normalized
     * (uses conjugate as inverse for performance).
     *
     * @param q Unit quaternion representing the rotation.
     * @param v Vector to rotate.
     * @return Rotated vector.
     */
    inline float3 rotate_vector(const quat& q, float3 v) noexcept
    {
        const quat vq{ v.x, v.y, v.z, 0.f };
        const quat q_inv{ conjugate(q) }; // assumes q is normalized
        quat result{ mul(mul(q, vq), q_inv) };

        return result.xyz;
    }
} // namespace chlm
