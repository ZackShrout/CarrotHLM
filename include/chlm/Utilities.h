//
// Created by Zack Shrout on 1/7/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Vector.h"

namespace chlm {
    /**
     * @brief Reflects an incident vector across a surface normal.
     *
     * Computes the reflection direction: i - 2 * dot(i, n) * n.
     * The normal is assumed to be normalized for correct results.
     *
     * @param i Incident direction (typically pointing towards the surface).
     * @param n Surface normal (should be normalized).
     * @return Reflected direction.
     */
    constexpr float3 reflect(const float3 i, const float3 n) noexcept
    {
        return i - 2.f * dot(i, n) * n;
    }

    /**
     * @brief Computes the refraction direction using Snell's law.
     *
     * @param i Incident direction (pointing towards the surface, typically normalized).
     * @param n Surface normal (must be normalized, pointing towards the incident side).
     * @param eta Ratio of indices of refraction (n1 / n2), where n1 is the medium the ray is coming from.
     * @return Refracted direction. Returns float3_zero on total internal reflection.
     */
    constexpr float3 refract(const float3 i, const float3 n, float eta) noexcept
    {
        const float ndoti{ dot(i, n) };
        float k{ 1.f - eta * eta * (1.f - ndoti * ndoti) };
        if (k < 0.f)
            return float3_zero; // total internal reflection
        return eta * i - (eta * dot(i, n) + std::sqrt(k)) * n;
    }

    /**
     * @brief Orients a normal to point towards the same hemisphere as the incident direction.
     *
     * Equivalent to GLSL/HLSL faceforward(). Useful for ensuring consistent normal direction
     * in shading (e.g., when a normal map might flip the geometric normal).
     *
     * @param n      Normal to potentially flip.
     * @param i      Incident direction (typically view direction or light direction).
     * @param nref   Reference normal (usually the geometric or interpolated normal).
     * @return n if dot(nref, i) >= 0, otherwise -n.
     */
    constexpr float3 faceforward(const float3 n, const float3 i, const float3 nref) noexcept
    {
        return dot(nref, i) < 0.f ? n : -n;
    }

    /**
     * @brief Computes the angle in radians between two vectors.
     *
     * Safe against zero-length vectors and numerical issues.
     *
     * @param a First vector.
     * @param b Second vector.
     * @return Angle in radians [0, π]. Returns 0 if either vector has zero length.
     */
    constexpr float angle_between(const float3 a, const float3 b) noexcept
    {
        const float d{ dot(a, b) };
        const float len2{ length_squared(a) * length_squared(b) };
        if (len2 <= 0.f) return 0.f; // or handle gracefully
        return std::acos(clamp(d / std::sqrt(len2), -1.f, 1.f));
    }

    /**
     * @brief Computes the angle in radians between two normalized vectors.
     *
     * Faster variant that assumes both inputs are already unit length.
     * Use this when you know the vectors are normalized.
     *
     * @param a Normalized vector.
     * @param b Normalized vector.
     * @return Angle in radians [0, π].
     */
    constexpr float angle_between_normalized(const float3 a, const float3 b) noexcept
    {
        return std::acos(clamp(dot(a, b), -1.f, 1.f));
    }

    /**
     * @brief Signed distance from a point to a plane defined as dot(p, normal) + d = 0.
     *
     * Positive values are on the side the normal points towards.
     *
     * @param point       Point to test.
     * @param plane_normal Normalized plane normal.
     * @param plane_d     Plane constant (distance offset).
     * @return Signed distance to the plane.
     */
    constexpr float distance_point_plane(const float3 point, const float3 plane_normal, float plane_d) noexcept
    {
        return dot(point, plane_normal) + plane_d;
    }

    /**
     * @brief Signed distance from a point to a plane defined by a normal and a point on the plane.
     *
     * Positive values are on the side the normal points towards.
     *
     * @param point           Point to test.
     * @param plane_normal    Normalized plane normal.
     * @param point_on_plane  Any point lying on the plane.
     * @return Signed distance to the plane.
     */
    constexpr float distance_point_plane(const float3 point, const float3 plane_normal, const float3 point_on_plane) noexcept
    {
        return dot(point - point_on_plane, plane_normal);
    }

    /**
     * @brief Projects vector v onto the direction of another vector.
     *
     * @param v    Vector to project.
     * @param onto Direction to project onto (should be non-zero).
     * @return Projection of v onto onto. Returns float3_zero if onto has near-zero length.
     */
    constexpr float3 project_vector(const float3 v, const float3 onto) noexcept
    {
        const float denom{ dot(onto, onto) };
        if (denom <= epsilon) return float3_zero;
        return (dot(v, onto) / denom) * onto;
    }
}