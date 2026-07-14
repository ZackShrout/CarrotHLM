//
// Created by Zack Shrout on 7/12/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <array>

namespace {
    template<int OwnerComponents, int... Indices>
    void validate_swizzle_case(bool& ok, int& read_cases, int& write_cases)
    {
        constexpr int length{ sizeof...(Indices) };
        constexpr int indices[]{ Indices... };
        constexpr bool writable{ chlm::detail::swizzle_indices_unique_v<Indices...> };

        float seed[4]{ 10.f, 20.f, 30.f, 40.f };

        if constexpr (length == 2)
        {
            using swizzle_type = chlm::detail::vector_swizzle2<OwnerComponents, Indices...>;
            static_assert(requires(swizzle_type value) { static_cast<chlm::float2>(value); });
            static_assert((requires(swizzle_type& lhs, const chlm::float2& rhs) { lhs = rhs; }) == writable);
            static_assert((requires(swizzle_type& lhs, const swizzle_type& rhs) { lhs = rhs; }) == writable);

            swizzle_type swizzle{};
            for (int lane = 0; lane < OwnerComponents; ++lane) swizzle.lanes[lane] = seed[lane];

            const chlm::float2 read_value{ static_cast<chlm::float2>(swizzle) };
            ok = ok &&
                 test_almost_equal(read_value.x, seed[indices[0]]) &&
                 test_almost_equal(read_value.y, seed[indices[1]]);
            ++read_cases;

            if constexpr (writable)
            {
                swizzle = chlm::float2{ 101.f, 102.f };
                ok = ok &&
                     test_almost_equal(swizzle.lanes[indices[0]], 101.f) &&
                     test_almost_equal(swizzle.lanes[indices[1]], 102.f);

                swizzle_type rhs{};
                for (int lane = 0; lane < OwnerComponents; ++lane) rhs.lanes[lane] = seed[lane] + 50.f;
                swizzle = rhs;
                ok = ok &&
                     test_almost_equal(swizzle.lanes[indices[0]], rhs.lanes[indices[0]]) &&
                     test_almost_equal(swizzle.lanes[indices[1]], rhs.lanes[indices[1]]);
                ++write_cases;
            }
        }
        else if constexpr (length == 3)
        {
            using swizzle_type = chlm::detail::vector_swizzle3<OwnerComponents, Indices...>;
            static_assert(requires(swizzle_type value) { static_cast<chlm::float3>(value); });
            static_assert((requires(swizzle_type& lhs, const chlm::float3& rhs) { lhs = rhs; }) == writable);
            static_assert((requires(swizzle_type& lhs, const swizzle_type& rhs) { lhs = rhs; }) == writable);

            swizzle_type swizzle{};
            for (int lane = 0; lane < OwnerComponents; ++lane) swizzle.lanes[lane] = seed[lane];

            const chlm::float3 read_value{ static_cast<chlm::float3>(swizzle) };
            ok = ok &&
                 test_almost_equal(read_value.x, seed[indices[0]]) &&
                 test_almost_equal(read_value.y, seed[indices[1]]) &&
                 test_almost_equal(read_value.z, seed[indices[2]]);
            ++read_cases;

            if constexpr (writable)
            {
                swizzle = chlm::float3{ 101.f, 102.f, 103.f };
                ok = ok &&
                     test_almost_equal(swizzle.lanes[indices[0]], 101.f) &&
                     test_almost_equal(swizzle.lanes[indices[1]], 102.f) &&
                     test_almost_equal(swizzle.lanes[indices[2]], 103.f);

                swizzle_type rhs{};
                for (int lane = 0; lane < OwnerComponents; ++lane) rhs.lanes[lane] = seed[lane] + 50.f;
                swizzle = rhs;
                ok = ok &&
                     test_almost_equal(swizzle.lanes[indices[0]], rhs.lanes[indices[0]]) &&
                     test_almost_equal(swizzle.lanes[indices[1]], rhs.lanes[indices[1]]) &&
                     test_almost_equal(swizzle.lanes[indices[2]], rhs.lanes[indices[2]]);
                ++write_cases;
            }
        }
        else if constexpr (length == 4)
        {
            using swizzle_type = chlm::detail::vector_swizzle4<OwnerComponents, Indices...>;
            static_assert(requires(swizzle_type value) { static_cast<chlm::float4>(value); });
            static_assert((requires(swizzle_type& lhs, const chlm::float4& rhs) { lhs = rhs; }) == writable);
            static_assert((requires(swizzle_type& lhs, const swizzle_type& rhs) { lhs = rhs; }) == writable);

            swizzle_type swizzle{};
            for (int lane = 0; lane < OwnerComponents; ++lane) swizzle.lanes[lane] = seed[lane];

            const chlm::float4 read_value{ static_cast<chlm::float4>(swizzle) };
            ok = ok &&
                 test_almost_equal(read_value.x, seed[indices[0]]) &&
                 test_almost_equal(read_value.y, seed[indices[1]]) &&
                 test_almost_equal(read_value.z, seed[indices[2]]) &&
                 test_almost_equal(read_value.w, seed[indices[3]]);
            ++read_cases;

            if constexpr (writable)
            {
                swizzle = chlm::float4{ 101.f, 102.f, 103.f, 104.f };
                ok = ok &&
                     test_almost_equal(swizzle.lanes[indices[0]], 101.f) &&
                     test_almost_equal(swizzle.lanes[indices[1]], 102.f) &&
                     test_almost_equal(swizzle.lanes[indices[2]], 103.f) &&
                     test_almost_equal(swizzle.lanes[indices[3]], 104.f);

                swizzle_type rhs{};
                for (int lane = 0; lane < OwnerComponents; ++lane) rhs.lanes[lane] = seed[lane] + 50.f;
                swizzle = rhs;
                ok = ok &&
                     test_almost_equal(swizzle.lanes[indices[0]], rhs.lanes[indices[0]]) &&
                     test_almost_equal(swizzle.lanes[indices[1]], rhs.lanes[indices[1]]) &&
                     test_almost_equal(swizzle.lanes[indices[2]], rhs.lanes[indices[2]]) &&
                     test_almost_equal(swizzle.lanes[indices[3]], rhs.lanes[indices[3]]);
                ++write_cases;
            }
        }
    }

    template<int OwnerComponents, int Length, int... Prefix>
    void enumerate_swizzle_cases(bool& ok, int& read_cases, int& write_cases)
    {
        if constexpr (sizeof...(Prefix) == Length)
        {
            validate_swizzle_case<OwnerComponents, Prefix...>(ok, read_cases, write_cases);
        }
        else
        {
            enumerate_swizzle_cases<OwnerComponents, Length, Prefix..., 0>(ok, read_cases, write_cases);
            if constexpr (OwnerComponents > 1)
            {
                enumerate_swizzle_cases<OwnerComponents, Length, Prefix..., 1>(ok, read_cases, write_cases);
            }
            if constexpr (OwnerComponents > 2)
            {
                enumerate_swizzle_cases<OwnerComponents, Length, Prefix..., 2>(ok, read_cases, write_cases);
            }
            if constexpr (OwnerComponents > 3)
            {
                enumerate_swizzle_cases<OwnerComponents, Length, Prefix..., 3>(ok, read_cases, write_cases);
            }
        }
    }

    template<int OwnerComponents>
    void run_exhaustive_swizzle_group(test_context& ctx, const std::string_view label)
    {
        bool ok{ true };
        int read_cases{ 0 };
        int write_cases{ 0 };

        enumerate_swizzle_cases<OwnerComponents, 2>(ok, read_cases, write_cases);
        enumerate_swizzle_cases<OwnerComponents, 3>(ok, read_cases, write_cases);
        enumerate_swizzle_cases<OwnerComponents, 4>(ok, read_cases, write_cases);

        ctx.expect(ok, label);
        test_println("  {}: {} read cases, {} write cases", label, read_cases, write_cases);
    }
}

void run_float_vector_exhaustive_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Float Vector Exhaustive Tests");

    static_assert(sizeof(float2) == 8);
    static_assert(sizeof(float3) == 12);
    static_assert(sizeof(float4) == 16);
    static_assert(alignof(float2) == alignof(float));
    static_assert(alignof(float3) == alignof(float));
    static_assert(alignof(float4) == alignof(float));

    float2 uv{ 1.f, 2.f };
    uv.r = 5.f;
    uv.t = 6.f;
    ctx.expect(
        test_almost_equal(uv.x, 5.f) &&
        test_almost_equal(uv.y, 6.f) &&
        test_almost_equal(uv.s, 5.f) &&
        test_almost_equal(uv.v, 6.f),
        "float2 aliasing test"
    );

    float3 normal{ 1.f, 2.f, 3.f };
    normal.r = 4.f;
    normal.t = 5.f;
    normal.p = 6.f;
    ctx.expect(
        test_almost_equal(normal.x, 4.f) &&
        test_almost_equal(normal.y, 5.f) &&
        test_almost_equal(normal.z, 6.f) &&
        test_almost_equal(normal.g, 5.f),
        "float3 aliasing test"
    );

    float4 color{ 1.f, 2.f, 3.f, 4.f };
    color.r = 8.f;
    color.t = 9.f;
    color.a = 10.f;
    ctx.expect(
        test_almost_equal(color.x, 8.f) &&
        test_almost_equal(color.y, 9.f) &&
        test_almost_equal(color.w, 10.f) &&
        test_almost_equal(color.s, 8.f),
        "float4 aliasing test"
    );

    {
        float2 a{ 8.f, 4.f };
        float2 b{ 2.f, 1.f };
        float2 accum{ a };
        accum += b;
        accum -= float2{ 1.f, 1.f };
        accum *= 2.f;
        accum /= 2.f;

        ctx.expect(
            test_almost_equal(a + b, float2{ 10.f, 5.f }) &&
            test_almost_equal(a - b, float2{ 6.f, 3.f }) &&
            test_almost_equal(a * b, float2{ 16.f, 4.f }) &&
            test_almost_equal(a / b, float2{ 4.f, 4.f }) &&
            test_almost_equal(-a, float2{ -8.f, -4.f }) &&
            test_almost_equal(a * 0.5f, float2{ 4.f, 2.f }) &&
            test_almost_equal(0.5f * a, float2{ 4.f, 2.f }) &&
            test_almost_equal(accum, float2{ 9.f, 4.f }) &&
            test_almost_equal(dot(a, b), 20.f) &&
            test_almost_equal(length_squared(a), 80.f) &&
            test_almost_equal(length(float2{ 3.f, 4.f }), 5.f) &&
            test_almost_equal(normalize(float2{ 3.f, 4.f }), float2{ 0.6f, 0.8f }, 1e-3f) &&
            test_almost_equal(lerp(float2{ 0.f, 2.f }, float2{ 10.f, 6.f }, 0.25f), float2{ 2.5f, 3.f }),
            "float2 arithmetic test"
        );
    }

    {
        float3 a{ 8.f, 4.f, 2.f };
        float3 b{ 2.f, 1.f, 0.5f };
        float3 accum{ a };
        accum += b;
        accum -= float3{ 1.f, 1.f, 1.f };
        accum *= 2.f;
        accum /= 2.f;

        ctx.expect(
            test_almost_equal(a + b, float3{ 10.f, 5.f, 2.5f }) &&
            test_almost_equal(a - b, float3{ 6.f, 3.f, 1.5f }) &&
            test_almost_equal(a * b, float3{ 16.f, 4.f, 1.f }) &&
            test_almost_equal(a / b, float3{ 4.f, 4.f, 4.f }) &&
            test_almost_equal(-a, float3{ -8.f, -4.f, -2.f }) &&
            test_almost_equal(a * 0.5f, float3{ 4.f, 2.f, 1.f }) &&
            test_almost_equal(0.5f * a, float3{ 4.f, 2.f, 1.f }) &&
            test_almost_equal(accum, float3{ 9.f, 4.f, 1.5f }) &&
            test_almost_equal(dot(a, b), 21.f) &&
            test_almost_equal(length_squared(a), 84.f) &&
            test_almost_equal(length(float3{ 2.f, 3.f, 6.f }), 7.f) &&
            test_almost_equal(normalize(float3{ 0.f, 3.f, 4.f }), float3{ 0.f, 0.6f, 0.8f }, 1e-3f) &&
            test_almost_equal(lerp(float3{ 0.f, 2.f, 4.f }, float3{ 10.f, 6.f, 8.f }, 0.25f), float3{ 2.5f, 3.f, 5.f }) &&
            test_almost_equal(cross(float3{ 1.f, 0.f, 0.f }, float3{ 0.f, 1.f, 0.f }), float3{ 0.f, 0.f, 1.f }),
            "float3 arithmetic test"
        );
    }

    {
        float4 a{ 8.f, 4.f, 2.f, 1.f };
        float4 b{ 2.f, 1.f, 0.5f, 0.25f };
        float4 accum{ a };
        accum += b;
        accum -= float4{ 1.f, 1.f, 1.f, 1.f };

        ctx.expect(
            test_almost_equal(a + b, float4{ 10.f, 5.f, 2.5f, 1.25f }) &&
            test_almost_equal(a - b, float4{ 6.f, 3.f, 1.5f, 0.75f }) &&
            test_almost_equal(a * b, float4{ 16.f, 4.f, 1.f, 0.25f }) &&
            test_almost_equal(a / b, float4{ 4.f, 4.f, 4.f, 4.f }) &&
            test_almost_equal(-a, float4{ -8.f, -4.f, -2.f, -1.f }) &&
            test_almost_equal(a * 0.5f, float4{ 4.f, 2.f, 1.f, 0.5f }) &&
            test_almost_equal(0.5f * a, float4{ 4.f, 2.f, 1.f, 0.5f }) &&
            test_almost_equal(accum, float4{ 9.f, 4.f, 1.5f, 0.25f }) &&
            test_almost_equal(dot(a, b), 21.25f) &&
            test_almost_equal(length_squared(a), 85.f) &&
            test_almost_equal(length(float4{ 2.f, 3.f, 6.f, 6.f }), std::sqrt(85.f), GENERAL_EPS) &&
            test_almost_equal(normalize(float4{ 0.f, 3.f, 4.f, 0.f }), float4{ 0.f, 0.6f, 0.8f, 0.f }, 1e-3f) &&
            test_almost_equal(lerp(float4{ 0.f, 2.f, 4.f, 8.f }, float4{ 10.f, 6.f, 8.f, 12.f }, 0.25f), float4{ 2.5f, 3.f, 5.f, 9.f }),
            "float4 arithmetic test"
        );
    }

    {
        float2 texcoord{ 1.f, 2.f };
        float3 tangent{ 3.f, 4.f, 5.f };
        float4 position{ 6.f, 7.f, 8.f, 9.f };

        texcoord.rg = float2{ 11.f, 12.f };
        tangent.stp = float3{ 13.f, 14.f, 15.f };
        position.stpq = float4{ 16.f, 17.f, 18.f, 19.f };

        ctx.expect(
            test_almost_equal(texcoord.xy, float2{ 11.f, 12.f }) &&
            test_almost_equal(texcoord.uv, float2{ 11.f, 12.f }) &&
            test_almost_equal(tangent.xyz, float3{ 13.f, 14.f, 15.f }) &&
            test_almost_equal(tangent.rgb, float3{ 13.f, 14.f, 15.f }) &&
            test_almost_equal(position.xyzw, float4{ 16.f, 17.f, 18.f, 19.f }) &&
            test_almost_equal(position.rgba, float4{ 16.f, 17.f, 18.f, 19.f }),
            "public family alias swizzle test"
        );
    }

    run_exhaustive_swizzle_group<2>(ctx, "Exhaustive float2-owner swizzle semantics");
    run_exhaustive_swizzle_group<3>(ctx, "Exhaustive float3-owner swizzle semantics");
    run_exhaustive_swizzle_group<4>(ctx, "Exhaustive float4-owner swizzle semantics");
}
