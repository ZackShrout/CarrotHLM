//
// Created by Zack Shrout on 7/15/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "../../Vector.h"

namespace chlm {
    /**
     * @brief Generates a 2D vector from independent half-open component ranges.
     *
     * The x and y components are sampled independently using rng32::next_float.
     * The generator advances once per component, for two advances in total.
     *
     * @param rng Generator to sample and advance.
     * @param min_inclusive Finite lower bounds for the x and y components.
     * @param max_exclusive Finite upper bounds excluded from each component range.
     * @return Vector whose components are in the corresponding intervals
     * [`min_inclusive`, `max_exclusive`).
     * @pre Each component of @p min_inclusive must be less than or equal to the
     * corresponding component of @p max_exclusive, and all bounds must be finite.
     * Invalid bounds trigger an assertion in debug builds.
     */
    [[nodiscard]] inline float2 random_float2(
        rng32& rng,
        const float2 min_inclusive,
        const float2 max_exclusive) noexcept
    {
        return {
            rng.next_float(min_inclusive.x, max_exclusive.x),
            rng.next_float(min_inclusive.y, max_exclusive.y)
        };
    }

    /**
     * @brief Generates a 3D vector from independent half-open component ranges.
     *
     * The x, y, and z components are sampled independently using
     * rng32::next_float. The generator advances once per component, for three
     * advances in total.
     *
     * @param rng Generator to sample and advance.
     * @param min_inclusive Finite lower bounds for the x, y, and z components.
     * @param max_exclusive Finite upper bounds excluded from each component range.
     * @return Vector whose components are in the corresponding intervals
     * [`min_inclusive`, `max_exclusive`).
     * @pre Each component of @p min_inclusive must be less than or equal to the
     * corresponding component of @p max_exclusive, and all bounds must be finite.
     * Invalid bounds trigger an assertion in debug builds.
     */
    [[nodiscard]] inline float3 random_float3(
        rng32& rng,
        const float3 min_inclusive,
        const float3 max_exclusive) noexcept
    {
        return {
            rng.next_float(min_inclusive.x, max_exclusive.x),
            rng.next_float(min_inclusive.y, max_exclusive.y),
            rng.next_float(min_inclusive.z, max_exclusive.z)
        };
    }

    /**
     * @brief Generates a 4D vector from independent half-open component ranges.
     *
     * The x, y, z, and w components are sampled independently using
     * rng32::next_float. The generator advances once per component, for four
     * advances in total.
     *
     * @param rng Generator to sample and advance.
     * @param min_inclusive Finite lower bounds for all four components.
     * @param max_exclusive Finite upper bounds excluded from each component range.
     * @return Vector whose components are in the corresponding intervals
     * [`min_inclusive`, `max_exclusive`).
     * @pre Each component of @p min_inclusive must be less than or equal to the
     * corresponding component of @p max_exclusive, and all bounds must be finite.
     * Invalid bounds trigger an assertion in debug builds.
     */
    [[nodiscard]] inline float4 random_float4(
        rng32& rng,
        const float4 min_inclusive,
        const float4 max_exclusive) noexcept
    {
        return {
            rng.next_float(min_inclusive.x, max_exclusive.x),
            rng.next_float(min_inclusive.y, max_exclusive.y),
            rng.next_float(min_inclusive.z, max_exclusive.z),
            rng.next_float(min_inclusive.w, max_exclusive.w)
        };
    }

    /**
     * @brief Samples a uniformly distributed direction on the unit circle.
     *
     * A uniform angle in [0, 2*pi) is mapped to `(cos(angle), sin(angle))`.
     * Every orientation in the XY plane therefore has equal angular probability.
     * Floating-point trigonometric approximation may make the squared length
     * differ slightly from exactly `1`. The generator advances once.
     *
     * @param rng Generator to sample and advance.
     * @return Approximately unit-length 2D direction on the unit circle.
     */
    [[nodiscard]] inline float2 random_unit_vector2(rng32& rng) noexcept
    {
        const float angle{ rng.next_float(0.f, two_pi) };
        const sin_cos_result direction{ sin_cos(angle) };

        return { direction.cosine, direction.sine };
    }

    /**
     * @brief Samples a uniformly distributed direction on the unit sphere.
     *
     * Uses the Marsaglia method to sample the sphere surface without polar
     * concentration. Every equal-area region of the surface has equal probability.
     * Floating-point square-root rounding may make the squared length differ
     * slightly from exactly `1`. Rejection sampling consumes a variable number
     * of generator outputs.
     *
     * @param rng Generator to sample and advance.
     * @return Approximately unit-length 3D direction on the unit sphere.
     */
    [[nodiscard]] inline float3 random_unit_vector3(rng32& rng) noexcept
    {
        while (true)
        {
            const float x{ rng.next_float(-1.f, 1.f) };
            const float y{ rng.next_float(-1.f, 1.f) };
            const float radius_squared{ x * x + y * y };

            if (radius_squared <= 0.f || radius_squared >= 1.f) continue;

            const float scale{ 2.f * sqrt(1.f - radius_squared) };

            return { x * scale, y * scale, 1.f - 2.f * radius_squared };
        }
    }

    /**
     * @brief Samples a point uniformly by area inside the unit circle.
     *
     * Rejection sampling from the square [-1, 1) x [-1, 1) gives every equal-area
     * region of the open unit disk equal probability. Returned points satisfy
     * `length_squared(result) < 1`; the circular boundary is excluded and the
     * origin may be returned. Sampling consumes a variable number of generator
     * outputs.
     *
     * @param rng Generator to sample and advance.
     * @return 2D point uniformly distributed over the open unit disk.
     */
    [[nodiscard]] inline float2 random_point_in_unit_circle(rng32& rng) noexcept
    {
        while (true)
        {
            const float2 point{
                rng.next_float(-1.f, 1.f),
                rng.next_float(-1.f, 1.f)
            };

            if (length_squared(point) < 1.f) return point;
        }
    }

    /**
     * @brief Samples a point uniformly on the unit circle.
     *
     * This is the positional equivalent of random_unit_vector2 and has the same
     * uniform angular distribution, floating-point length tolerance, and single
     * generator advance.
     *
     * @param rng Generator to sample and advance.
     * @return Approximately unit-length 2D point on the unit circle.
     */
    [[nodiscard]] inline float2 random_point_on_unit_circle(rng32& rng) noexcept
    {
        return random_unit_vector2(rng);
    }

    /**
     * @brief Samples a point uniformly by volume inside the unit sphere.
     *
     * Rejection sampling from the cube [-1, 1)^3 gives every equal-volume region
     * of the open unit ball equal probability. Returned points satisfy
     * `length_squared(result) < 1`; the spherical boundary is excluded and the
     * origin may be returned. Sampling consumes a variable number of generator
     * outputs.
     *
     * @param rng Generator to sample and advance.
     * @return 3D point uniformly distributed throughout the open unit ball.
     */
    [[nodiscard]] inline float3 random_point_in_unit_sphere(rng32& rng) noexcept
    {
        while (true)
        {
            const float3 point{
                rng.next_float(-1.f, 1.f),
                rng.next_float(-1.f, 1.f),
                rng.next_float(-1.f, 1.f)
            };

            if (length_squared(point) < 1.f) return point;
        }
    }

    /**
     * @brief Samples a point uniformly on the unit sphere.
     *
     * This is the positional equivalent of random_unit_vector3 and has the same
     * equal-area surface distribution, floating-point length tolerance, and
     * variable generator consumption.
     *
     * @param rng Generator to sample and advance.
     * @return Approximately unit-length 3D point on the unit sphere.
     */
    [[nodiscard]] inline float3 random_point_on_unit_sphere(rng32& rng) noexcept
    {
        return random_unit_vector3(rng);
    }
} // namespace chlm
