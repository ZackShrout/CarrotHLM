//
// Created by Zack Shrout on 7/12/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

namespace {
    template<int OwnerComponents, int... Indices>
    void validate_uint_swizzle_case(bool& ok, int& read_cases, int& write_cases)
    {
        constexpr int length{ sizeof...(Indices) };
        constexpr int indices[]{ Indices... };
        constexpr bool writable{ chlm::detail::swizzle_indices_unique_v<Indices...> };

        unsigned int seed[4]{ 10u, 20u, 30u, 40u };

        if constexpr (length == 2)
        {
            using swizzle_type = chlm::detail::uvector_swizzle2<OwnerComponents, Indices...>;
            static_assert(requires(swizzle_type value) { static_cast<chlm::uint2>(value); });
            static_assert((requires(swizzle_type& lhs, const chlm::uint2& rhs) { lhs = rhs; }) == writable);

            swizzle_type swizzle{};
            for (int lane = 0; lane < OwnerComponents; ++lane) swizzle.lanes[lane] = seed[lane];
            const chlm::uint2 read_value{ static_cast<chlm::uint2>(swizzle) };
            ok = ok &&
                 read_value.x == seed[indices[0]] &&
                 read_value.y == seed[indices[1]];
            ++read_cases;

            if constexpr (writable)
            {
                swizzle = chlm::uint2{ 101u, 102u };
                ok = ok &&
                     swizzle.lanes[indices[0]] == 101u &&
                     swizzle.lanes[indices[1]] == 102u;
                ++write_cases;
            }
        }
        else if constexpr (length == 3)
        {
            using swizzle_type = chlm::detail::uvector_swizzle3<OwnerComponents, Indices...>;
            static_assert(requires(swizzle_type value) { static_cast<chlm::uint3>(value); });
            static_assert((requires(swizzle_type& lhs, const chlm::uint3& rhs) { lhs = rhs; }) == writable);

            swizzle_type swizzle{};
            for (int lane = 0; lane < OwnerComponents; ++lane) swizzle.lanes[lane] = seed[lane];
            const chlm::uint3 read_value{ static_cast<chlm::uint3>(swizzle) };
            ok = ok &&
                 read_value.x == seed[indices[0]] &&
                 read_value.y == seed[indices[1]] &&
                 read_value.z == seed[indices[2]];
            ++read_cases;

            if constexpr (writable)
            {
                swizzle = chlm::uint3{ 101u, 102u, 103u };
                ok = ok &&
                     swizzle.lanes[indices[0]] == 101u &&
                     swizzle.lanes[indices[1]] == 102u &&
                     swizzle.lanes[indices[2]] == 103u;
                ++write_cases;
            }
        }
        else if constexpr (length == 4)
        {
            using swizzle_type = chlm::detail::uvector_swizzle4<OwnerComponents, Indices...>;
            static_assert(requires(swizzle_type value) { static_cast<chlm::uint4>(value); });
            static_assert((requires(swizzle_type& lhs, const chlm::uint4& rhs) { lhs = rhs; }) == writable);

            swizzle_type swizzle{};
            for (int lane = 0; lane < OwnerComponents; ++lane) swizzle.lanes[lane] = seed[lane];
            const chlm::uint4 read_value{ static_cast<chlm::uint4>(swizzle) };
            ok = ok &&
                 read_value.x == seed[indices[0]] &&
                 read_value.y == seed[indices[1]] &&
                 read_value.z == seed[indices[2]] &&
                 read_value.w == seed[indices[3]];
            ++read_cases;

            if constexpr (writable)
            {
                swizzle = chlm::uint4{ 101u, 102u, 103u, 104u };
                ok = ok &&
                     swizzle.lanes[indices[0]] == 101u &&
                     swizzle.lanes[indices[1]] == 102u &&
                     swizzle.lanes[indices[2]] == 103u &&
                     swizzle.lanes[indices[3]] == 104u;
                ++write_cases;
            }
        }
    }

    template<int OwnerComponents, int Length, int... Prefix>
    void enumerate_uint_swizzle_cases(bool& ok, int& read_cases, int& write_cases)
    {
        if constexpr (sizeof...(Prefix) == Length)
        {
            validate_uint_swizzle_case<OwnerComponents, Prefix...>(ok, read_cases, write_cases);
        }
        else
        {
            enumerate_uint_swizzle_cases<OwnerComponents, Length, Prefix..., 0>(ok, read_cases, write_cases);
            if constexpr (OwnerComponents > 1) enumerate_uint_swizzle_cases<OwnerComponents, Length, Prefix..., 1>(ok, read_cases, write_cases);
            if constexpr (OwnerComponents > 2) enumerate_uint_swizzle_cases<OwnerComponents, Length, Prefix..., 2>(ok, read_cases, write_cases);
            if constexpr (OwnerComponents > 3) enumerate_uint_swizzle_cases<OwnerComponents, Length, Prefix..., 3>(ok, read_cases, write_cases);
        }
    }
}

void run_uint_vector_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Uint Vector Tests");

    static_assert(sizeof(uint2) == 8);
    static_assert(sizeof(uint3) == 12);
    static_assert(sizeof(uint4) == 16);

    uint2 uv{ 1u, 2u };
    uv.r = 3u;
    uv.v = 4u;

    uint3 coord{ 5u, 6u, 7u };
    coord.rgb = uint3{ 8u, 9u, 10u };

    uint4 mask{ 1u, 2u, 3u, 4u };
    mask.stpq = uint4{ 5u, 6u, 7u, 8u };

    ctx.expect(
        uv.x == 3u && uv.y == 4u &&
        coord.x == 8u && coord.y == 9u && coord.z == 10u &&
        mask.x == 5u && mask.y == 6u && mask.z == 7u && mask.w == 8u,
        "Uint alias family test"
    );

    const uint2 sum2{ uint2{ 8u, 4u } + uint2{ 2u, 1u } };
    const uint3 sum3{ uint3{ 8u, 4u, 2u } + uint3{ 2u, 1u, 1u } };
    const uint4 sum4{ uint4{ 8u, 4u, 2u, 1u } + uint4{ 2u, 1u, 1u, 1u } };

    uint2 texcoord{ 10u, 20u };
    const uint3 uvu{ texcoord.uvu };
    texcoord.yx = uint2{ 30u, 40u };

    uint3 normal{ 1u, 2u, 3u };
    const uint4 xyzx{ normal.xyzx };
    normal.zyx = uint3{ 6u, 5u, 4u };

    ctx.expect(
        test_equal(sum2, uint2{ 10u, 5u }) &&
        test_equal(sum3, uint3{ 10u, 5u, 3u }) &&
        test_equal(sum4, uint4{ 10u, 5u, 3u, 2u }) &&
        test_equal(uint2{ 8u, 4u } - uint2{ 2u, 1u }, uint2{ 6u, 3u }) &&
        test_equal(uint3{ 8u, 4u, 2u } * uint3{ 2u, 3u, 4u }, uint3{ 16u, 12u, 8u }) &&
        test_equal(uint4{ 8u, 4u, 2u, 1u } / uint4{ 2u, 2u, 2u, 1u }, uint4{ 4u, 2u, 1u, 1u }) &&
        test_equal(uvu, uint3{ 10u, 20u, 10u }) &&
        test_equal(texcoord, uint2{ 40u, 30u }) &&
        test_equal(xyzx, uint4{ 1u, 2u, 3u, 1u }) &&
        test_equal(normal, uint3{ 4u, 5u, 6u }),
        "Uint arithmetic and swizzle test"
    );

    bool ok{ true };
    int read_cases{ 0 };
    int write_cases{ 0 };
    enumerate_uint_swizzle_cases<2, 2>(ok, read_cases, write_cases);
    enumerate_uint_swizzle_cases<2, 3>(ok, read_cases, write_cases);
    enumerate_uint_swizzle_cases<2, 4>(ok, read_cases, write_cases);
    ctx.expect(ok, "Exhaustive uint2-owner swizzle semantics");
    test_println("  uint2-owner swizzles: {} read cases, {} write cases", read_cases, write_cases);

    ok = true; read_cases = 0; write_cases = 0;
    enumerate_uint_swizzle_cases<3, 2>(ok, read_cases, write_cases);
    enumerate_uint_swizzle_cases<3, 3>(ok, read_cases, write_cases);
    enumerate_uint_swizzle_cases<3, 4>(ok, read_cases, write_cases);
    ctx.expect(ok, "Exhaustive uint3-owner swizzle semantics");
    test_println("  uint3-owner swizzles: {} read cases, {} write cases", read_cases, write_cases);

    ok = true; read_cases = 0; write_cases = 0;
    enumerate_uint_swizzle_cases<4, 2>(ok, read_cases, write_cases);
    enumerate_uint_swizzle_cases<4, 3>(ok, read_cases, write_cases);
    enumerate_uint_swizzle_cases<4, 4>(ok, read_cases, write_cases);
    ctx.expect(ok, "Exhaustive uint4-owner swizzle semantics");
    test_println("  uint4-owner swizzles: {} read cases, {} write cases", read_cases, write_cases);

    constexpr uint_rect rect{ {10u, 20u}, {30u, 40u} };
    static_assert(rect_max(rect).x == 40u && rect_max(rect).y == 60u);
    static_assert(contains(rect, uint2{ 10u, 20u }));
    static_assert(!contains(rect, uint2{ 40u, 60u }));
    static_assert(!empty(rect));
    ctx.expect(true, "Uint rect constexpr smoke test");
}
