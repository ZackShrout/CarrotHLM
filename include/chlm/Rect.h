//
// Created by Zack Shrout on 3/31/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include "Core.h"

namespace chlm {
    /**
     * @brief Unsigned integer axis-aligned rectangle.
     *
     * Represents a 2D rectangle using a minimum corner (`position`) and an extent (`size`).
     *
     * This type is especially useful for pixel-space regions such as sprite sheet frames,
     * texture subregions, atlas packing, and image bounds.
     *
     * The rectangle uses an **exclusive max corner** convention:
     * - Minimum corner is inclusive
     * - Maximum corner is exclusive
     *
     * For example, a rectangle with:
     * - `position = { 10, 20 }`
     * - `size = { 32, 16 }`
     *
     * covers X in `[10, 42)` and Y in `[20, 36)`.
     */
    struct uint_rect
    {
        uint2 position{};
        uint2 size{};
    };

    /**
     * @brief Floating-point axis-aligned rectangle.
     *
     * Represents a 2D rectangle using a minimum corner (`position`) and an extent (`size`).
     *
     * This type is useful for normalized UV regions, UI layout bounds, world-space 2D bounds,
     * and other continuous-space rectangle operations.
     *
     * The rectangle uses an **exclusive max corner** convention:
     * - Minimum corner is inclusive
     * - Maximum corner is exclusive
     *
     * For example, a rectangle with:
     * - `position = { 0.25f, 0.5f }`
     * - `size = { 0.5f, 0.25f }`
     *
     * covers X in `[0.25, 0.75)` and Y in `[0.5, 0.75)`.
     */
    struct float_rect
    {
        float2 position{};
        float2 size{};
    };

    /**
     * @brief Returns the minimum corner of an unsigned integer rectangle.
     *
     * The minimum corner is the rectangle's origin / top-left-like position,
     * depending on the coordinate system in use.
     *
     * @param rect Rectangle to query.
     * @return The inclusive minimum corner of the rectangle.
     */
    [[nodiscard]] constexpr uint2 rect_min(const uint_rect& rect) noexcept
    {
        return rect.position;
    }

    /**
     * @brief Returns the exclusive maximum corner of an unsigned integer rectangle.
     *
     * The maximum corner is computed as:
     *
     * `position + size`
     *
     * This value is exclusive, meaning points exactly equal to this corner
     * are considered outside the rectangle.
     *
     * @param rect Rectangle to query.
     * @return The exclusive maximum corner of the rectangle.
     */
    [[nodiscard]] constexpr uint2 rect_max(const uint_rect& rect) noexcept
    {
        return rect.position + rect.size;
    }

    /**
     * @brief Returns whether an unsigned integer point lies inside a rectangle.
     *
     * Uses the rectangle's inclusive minimum corner and exclusive maximum corner:
     *
     * - `point >= rect_min(rect)`
     * - `point < rect_max(rect)`
     *
     * This convention is especially useful for pixel-space bounds and subrect iteration.
     *
     * @param rect Rectangle to test against.
     * @param point Point to test.
     * @return true if the point lies inside the rectangle, otherwise false.
     */
    [[nodiscard]] constexpr bool contains(const uint_rect& rect, const uint2 point) noexcept
    {
        const uint2 min{ rect_min(rect) };
        const uint2 max{ rect_max(rect) };

        return point.x >= min.x && point.y >= min.y &&
               point.x < max.x && point.y < max.y;
    }

    /**
     * @brief Returns whether an unsigned integer rectangle has zero area.
     *
     * A rectangle is considered empty if either dimension is zero.
     *
     * @param rect Rectangle to test.
     * @return true if the rectangle has zero width or zero height, otherwise false.
     */
    [[nodiscard]] constexpr bool empty(const uint_rect& rect) noexcept
    {
        return rect.size.x == 0u || rect.size.y == 0u;
    }

    /**
     * @brief Returns the minimum corner of a floating-point rectangle.
     *
     * The minimum corner is the rectangle's origin / minimum bound.
     *
     * @param rect Rectangle to query.
     * @return The inclusive minimum corner of the rectangle.
     */
    [[nodiscard]] constexpr float2 rect_min(const float_rect& rect) noexcept
    {
        return rect.position;
    }

    /**
     * @brief Returns the exclusive maximum corner of a floating-point rectangle.
     *
     * The maximum corner is computed as:
     *
     * `position + size`
     *
     * This value is exclusive, meaning points exactly equal to this corner
     * are considered outside the rectangle.
     *
     * @param rect Rectangle to query.
     * @return The exclusive maximum corner of the rectangle.
     */
    [[nodiscard]] constexpr float2 rect_max(const float_rect& rect) noexcept
    {
        return rect.position + rect.size;
    }

    /**
     * @brief Returns whether a floating-point point lies inside a rectangle.
     *
     * Uses the rectangle's inclusive minimum corner and exclusive maximum corner:
     *
     * - `point >= rect_min(rect)`
     * - `point < rect_max(rect)`
     *
     * This is useful for normalized UV bounds, layout rectangles, and general
     * 2D continuous-space containment tests.
     *
     * @param rect Rectangle to test against.
     * @param point Point to test.
     * @return true if the point lies inside the rectangle, otherwise false.
     */
    [[nodiscard]] constexpr bool contains(const float_rect& rect, const float2 point) noexcept
    {
        const float2 min{ rect_min(rect) };
        const float2 max{ rect_max(rect) };

        return point.x >= min.x && point.y >= min.y &&
               point.x < max.x && point.y < max.y;
    }

    /**
     * @brief Returns whether a floating-point rectangle has zero or negative area.
     *
     * A rectangle is considered empty if its width or height is less than or equal to zero.
     *
     * @param rect Rectangle to test.
     * @return true if the rectangle has non-positive width or height, otherwise false.
     */
    [[nodiscard]] constexpr bool empty(const float_rect& rect) noexcept
    {
        return rect.size.x <= 0.0f || rect.size.y <= 0.0f;
    }
} // namespace chlm
