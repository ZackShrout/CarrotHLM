//
// Created by Zack Shrout on 1/7/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "ScalarMath.h"
#include "Vector.h"

namespace chlm {
    /**
     * @brief Computes the interpolation factor of a value within a scalar range.
     *
     * Computes (value - start) / (end - start). The start and end values map to
     * 0 and 1 respectively. The result is not clamped, so values outside the
     * range produce factors below 0 or above 1.
     *
     * Reversed ranges are supported: @p start still maps to 0 and @p end still
     * maps to 1. If @p start and @p end are equal, the range has no measurable
     * extent and this function returns 0 for every @p value. The finite
     * subtraction is evaluated in double to avoid overflow across the full
     * float range.
     *
     * @param start Value corresponding to an interpolation factor of 0.
     * @param end Value corresponding to an interpolation factor of 1.
     * @param value Value whose interpolation factor will be computed.
     * @return Unclamped interpolation factor, or 0 if the range is zero-width.
     */
    [[nodiscard]] constexpr float inverse_lerp(const float start, const float end, const float value) noexcept
    {
        if (start == end) return 0.f;

        return static_cast<float>((static_cast<double>(value) - static_cast<double>(start)) /
                                  (static_cast<double>(end) - static_cast<double>(start)));
    }

    /**
     * @brief Linearly remaps a value from one scalar range to another.
     *
     * Maps @p input_start to @p output_start and @p input_end to @p output_end.
     * The mapping is not clamped, so values outside the input range extrapolate
     * beyond the output range. Both ranges may be reversed independently.
     *
     * If the input range is zero-width, inverse_lerp() supplies a factor of 0
     * and this function returns @p output_start.
     *
     * @param input_start Input value corresponding to @p output_start.
     * @param input_end Input value corresponding to @p output_end.
     * @param output_start Result produced at @p input_start.
     * @param output_end Result produced at @p input_end.
     * @param value Input value to remap.
     * @return Linearly remapped value, including extrapolation when applicable.
     */
    [[nodiscard]] constexpr float remap(const float input_start, const float input_end,
                                        const float output_start, const float output_end,
                                        const float value) noexcept
    {
        const float factor{ inverse_lerp(input_start, input_end, value) };

        if (factor == 0.f) return output_start;
        if (factor == 1.f) return output_end;

        return static_cast<float>(static_cast<double>(output_start) * (1.0 - static_cast<double>(factor)) +
                                  static_cast<double>(output_end) * static_cast<double>(factor));
    }

    /**
     * @brief Performs a binary threshold test against an edge.
     *
     * This follows shader-style step semantics: values strictly below the edge
     * produce 0, while values equal to or greater than the edge produce 1.
     * A NaN edge or value propagates NaN.
     *
     * @param edge Threshold at which the result changes from 0 to 1.
     * @param value Value to compare with @p edge.
     * @return 0 if @p value is less than @p edge; otherwise 1.
     */
    [[nodiscard]] constexpr float step(const float edge, const float value) noexcept
    {
        if (isnan(edge) || isnan(value)) return edge + value;

        return value < edge ? 0.f : 1.f;
    }

    /**
     * @brief Computes a clamped cubic Hermite transition between two edges.
     *
     * Normalizes @p value between the edges, clamps the factor to [0, 1], and
     * evaluates t^2 * (3 - 2t). The curve returns 0 at @p edge_start, 1 at
     * @p edge_end, .5 at their midpoint, and has zero first derivative at both
     * endpoints. A mirrored polynomial evaluation preserves numerical stability
     * near both ends of the interval.
     *
     * Values outside the edge interval are clamped rather than extrapolated.
     * Reversed edges are supported and produce a descending transition as
     * @p value increases. Equal edges behave as step(edge_start, value).
     *
     * @param edge_start Edge corresponding to a result of 0.
     * @param edge_end Edge corresponding to a result of 1.
     * @param value Value at which to evaluate the transition.
     * @return Smoothed transition value in the inclusive range [0, 1].
     */
    [[nodiscard]] constexpr float smoothstep(const float edge_start, const float edge_end,
                                             const float value) noexcept
    {
        if (edge_start == edge_end) return step(edge_start, value);

        const float t{ saturate(inverse_lerp(edge_start, edge_end, value)) };

        if (t <= .5f) return t * t * (3.f - 2.f * t);

        const float complement{ 1.f - t };

        return 1.f - complement * complement * (3.f - 2.f * complement);
    }

    /**
     * @brief Computes a clamped quintic Hermite transition between two edges.
     *
     * Normalizes @p value between the edges, clamps the factor to [0, 1], and
     * evaluates t^3 * (t * (6t - 15) + 10). The curve returns 0 at
     * @p edge_start, 1 at @p edge_end, .5 at their midpoint, and has zero first
     * and second derivatives at both endpoints. Compared with smoothstep(), this
     * produces gentler acceleration into and out of the transition. A mirrored
     * polynomial evaluation preserves numerical stability near both endpoints.
     *
     * Values outside the edge interval are clamped rather than extrapolated.
     * Reversed edges are supported and produce a descending transition as
     * @p value increases. Equal edges behave as step(edge_start, value).
     *
     * @param edge_start Edge corresponding to a result of 0.
     * @param edge_end Edge corresponding to a result of 1.
     * @param value Value at which to evaluate the transition.
     * @return Smoothed transition value in the inclusive range [0, 1].
     */
    [[nodiscard]] constexpr float smootherstep(const float edge_start, const float edge_end,
                                               const float value) noexcept
    {
        if (edge_start == edge_end) return step(edge_start, value);

        const float t{ saturate(inverse_lerp(edge_start, edge_end, value)) };

        if (t <= .5f) return t * t * t * (t * (t * 6.f - 15.f) + 10.f);

        const float complement{ 1.f - t };

        return 1.f - complement * complement * complement *
                     (complement * (complement * 6.f - 15.f) + 10.f);
    }

    /**
     * @brief Moves a scalar value toward a target by a bounded amount.
     *
     * Advances @p current toward @p target by at most @p max_delta. If the
     * remaining distance is less than or equal to @p max_delta, the function
     * returns @p target exactly rather than stepping past it. Movement works in
     * both increasing and decreasing directions.
     *
     * A zero or negative @p max_delta disables movement and returns @p current.
     * Equal current and target values also return @p current, preserving its
     * floating-point representation. For positive deltas, NaN inputs propagate.
     *
     * @param current Value from which movement begins.
     * @param target Value toward which to move.
     * @param max_delta Maximum non-negative distance to move in this call.
     * @return The advanced value, clamped to @p target without overshoot.
     */
    [[nodiscard]] constexpr float move_towards(const float current, const float target,
                                               const float max_delta) noexcept
    {
        if (max_delta <= 0.f || current == target) return current;
        if (isnan(current) || isnan(target) || isnan(max_delta)) return current + target + max_delta;

        if (target > current)
            return target - current <= max_delta ? target : current + max_delta;

        return current - target <= max_delta ? target : current - max_delta;
    }

    /**
     * @brief Wraps a value into a zero-based repeating interval.
     *
     * For a finite positive @p length, the result lies in the half-open interval
     * [0, length). Positive and negative inputs wrap in the same direction, so
     * `repeat(-1, 5)` returns 4. Exact positive or negative multiples of
     * @p length return positive zero.
     *
     * A zero or negative @p length defines a degenerate interval and returns 0.
     * NaN inputs propagate when the length is positive. An infinite value or
     * positive infinite length returns NaN.
     *
     * @param value Value to wrap.
     * @param length Exclusive upper bound and period of the repeating interval.
     * @return Wrapped value in [0, length), or 0 for a non-positive length.
     */
    [[nodiscard]] constexpr float repeat(const float value, const float length) noexcept
    {
        if (length <= 0.f) return 0.f;
        if (!isfinite(length)) return isnan(length) ? length : length - length; // length - length here produces NaN

        const float remainder{ fmod(value, length) };

        if (isnan(remainder)) return remainder;
        if (remainder == 0.f) return 0.f;
        if (remainder > 0.f) return remainder;

        const float wrapped{ remainder + length };

        return wrapped == length ? 0.f : wrapped;
    }

    /**
     * @brief Wraps a value into an explicitly bounded interval.
     *
     * The result lies in the ascending half-open interval formed by the two
     * bounds: [min(bound_a, bound_b), max(bound_a, bound_b)). Supplying bounds
     * in reverse order therefore produces the same result as supplying them in
     * ascending order. Values already inside the interval are returned unchanged.
     *
     * Equal bounds define a zero-width interval and return @p bound_a for every
     * value. NaN values or bounds propagate unless equal bounds have already
     * resolved the result.
     *
     * @param value Value to wrap.
     * @param bound_a First interval bound.
     * @param bound_b Second interval bound.
     * @return Wrapped value in the ascending half-open interval, or @p bound_a
     *         if the interval is zero-width.
     */
    [[nodiscard]] constexpr float wrap(const float value, const float bound_a, const float bound_b) noexcept
    {
        if (bound_a == bound_b) return bound_a;
        if (isnan(value) || isnan(bound_a) || isnan(bound_b)) return value + bound_a + bound_b;

        const float lower{ min(bound_a, bound_b) };
        const float upper{ max(bound_a, bound_b) };

        if (value >= lower && value < upper) return value;

        const float width{ upper - lower };
        const float value_phase{ repeat(value, width) };
        const float lower_phase{ repeat(lower, width) };

        if (isnan(value_phase) || isnan(lower_phase)) return value_phase + lower_phase;

        const float offset{
            value_phase >= lower_phase
                ? value_phase - lower_phase
                : width - (lower_phase - value_phase)
        };
        if (offset == width) return lower;

        const float result{ lower + offset };
        return result == upper ? lower : result;
    }

    /**
     * @brief Computes the shortest signed angular displacement in radians.
     *
     * Returns the signed rotation from @p current to @p target, reduced to the
     * canonical interval (-pi, pi]. Positive results rotate toward increasing
     * angles and negative results rotate toward decreasing angles. Because a
     * half-turn has two equally short directions, an exact half-turn is always
     * represented as positive pi.
     *
     * Inputs may use any number of revolutions and need not already be wrapped.
     * NaN or infinite inputs return NaN.
     *
     * @param current Starting angle in radians.
     * @param target Target angle in radians.
     * @return Shortest signed angular displacement in (-pi, pi].
     */
    [[nodiscard]] constexpr float delta_angle(const float current, const float target) noexcept
    {
        const float current_phase{ repeat(current, two_pi) };
        const float target_phase{ repeat(target, two_pi) };
        float delta{ repeat(target_phase - current_phase, two_pi) };

        if (delta > pi) delta -= two_pi;

        return delta;
    }

    /**
     * @brief Linearly interpolates along the shortest angular arc in radians.
     *
     * Computes `current + delta_angle(current, target) * t`. The interpolation
     * factor is not clamped: 0 returns @p current, values between 0 and 1 follow
     * the shortest arc, and values outside that interval extrapolate along the
     * same signed arc. The result is not wrapped, preserving continuity when
     * @p current contains accumulated revolutions.
     *
     * For an exact half-turn, interpolation follows the positive direction in
     * accordance with delta_angle(). NaN or infinite angle inputs return NaN.
     *
     * @param current Starting angle in radians.
     * @param target Target angle in radians.
     * @param t Unclamped interpolation factor.
     * @return Angle reached along the shortest signed arc.
     */
    [[nodiscard]] constexpr float lerp_angle(const float current, const float target, const float t) noexcept
    {
        return current + delta_angle(current, target) * t;
    }

    /**
     * @brief Produces a value that moves back and forth over a scalar interval.
     *
     * Maps @p value onto a triangle wave over [0, length]. The result rises from
     * 0 to @p length, then falls back to 0, repeating with a period of
     * `2 * length`. Negative inputs mirror positive inputs, so
     * `ping_pong(-value, length) == ping_pong(value, length)` for finite values.
     *
     * A zero or negative @p length defines a degenerate interval and returns 0.
     * NaN inputs propagate when the length is positive. An infinite value or
     * positive infinite length returns NaN.
     *
     * @param value Position along the repeating triangle wave.
     * @param length Inclusive upper bound and half-period of the wave.
     * @return Mirrored value in the inclusive range [0, length].
     */
    [[nodiscard]] constexpr float ping_pong(const float value, const float length) noexcept
    {
        if (length <= 0.f) return 0.f;

        const float magnitude{ abs(value) };
        if (magnitude <= length) return magnitude;

        const float period{ length + length };

        if (isfinite(period))
        {
            const float phase{ repeat(magnitude, period) };

            if (isnan(phase)) return phase;

            return phase <= length ? phase : period - phase;
        }

        const float phase{ repeat(magnitude * .5f, length) };

        if (isnan(phase)) return phase;

        return phase <= length * .5f ? phase * 2.f : (length - phase) * 2.f;
    }

    /**
     * @brief Applies frame-rate-independent exponential decay toward a target.
     *
     * The decay is parameterized by @p half_life: after that many time units,
     * half of the previous distance to @p target remains. The retained fraction
     * is `2^(-delta_time / half_life)` and is evaluated with exp2_precise(),
     * whose error target is at most 1 ULP. Splitting a time interval into equal
     * updates preserves the same decay curve within 2e-5 of the original span
     * for up to 240 subdivisions, excluding ordinary result underflow.
     *
     * Finite endpoints with valid timing inputs move monotonically toward
     * @p target without overshoot. A zero or negative @p delta_time performs no
     * update. A non-positive or positive infinite @p half_life also disables decay,
     * treating invalid half-lives conservatively rather than moving away from
     * the target. Positive infinite delta time returns @p target immediately.
     * NaN inputs propagate.
     *
     * @param current Value at the beginning of the update.
     * @param target Asymptotic value approached by the decay.
     * @param half_life Time required to remove half the remaining distance.
     * @param delta_time Elapsed time since the previous update, in the same units
     *                   as @p half_life.
     * @return Decayed value between @p current and @p target.
     */
    [[nodiscard]] inline float exp_decay(const float current, const float target,
                                         const float half_life, const float delta_time) noexcept
    {
        if (delta_time <= 0.f || current == target) return current;

        if (isnan(current) || isnan(target) || isnan(half_life) || isnan(delta_time))
            return current + target + half_life + delta_time;

        if (half_life <= 0.f) return current;
        if (isinf(delta_time)) return target;
        if (isinf(half_life)) return current;

        if (isinf(current) || isinf(target))
        {
            if (isinf(current) && isinf(target)) return current + target;

            return isinf(current) ? current : target;
        }

        const float retained{ exp2_precise(-delta_time / half_life) };

        if (retained == 0.f) return target;
        if (retained == 1.f) return current;

        const float decayed{ current * retained + target * (1.f - retained) };
        return clamp(decayed, min(current, target), max(current, target));
    }

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
    inline float3 reflect(const float3 i, const float3 n) noexcept
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
    inline float3 refract(const float3 i, const float3 n, float eta) noexcept
    {
        const float ndoti{ dot(i, n) };
        float k{ 1.f - eta * eta * (1.f - ndoti * ndoti) };

        if (k < 0.f) return float3_zero; // total internal reflection

        return eta * i - (eta * dot(i, n) + sqrt(k)) * n;
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
    inline float3 faceforward(const float3 n, const float3 i, const float3 nref) noexcept
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
    inline float angle_between(const float3 a, const float3 b) noexcept
    {
        const float d{ dot(a, b) };
        const float len2{ length_squared(a) * length_squared(b) };
        if (len2 <= 0.f) return 0.f; // or handle gracefully

        return acos(clamp(d / sqrt(len2), -1.f, 1.f));
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
    inline float angle_between_normalized(const float3 a, const float3 b) noexcept
    {
        return acos(clamp(dot(a, b), -1.f, 1.f));
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
    inline float distance_point_plane(const float3 point, const float3 plane_normal, float plane_d) noexcept
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
    inline float distance_point_plane(const float3 point, const float3 plane_normal, const float3 point_on_plane) noexcept
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
    inline float3 project_vector(const float3 v, const float3 onto) noexcept
    {
        const float denom{ dot(onto, onto) };
        if (denom <= epsilon) return float3_zero;

        return dot(v, onto) / denom * onto;
    }
}
