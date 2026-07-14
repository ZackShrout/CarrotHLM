//
// Created by Zack Shrout on 7/12/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

namespace {
    template<int OwnerComponents, int... Indices>
    void validate_int_swizzle_case(bool& ok, int& read_cases, int& write_cases)
    {
        constexpr int length{ sizeof...(Indices) };
        constexpr int indices[]{ Indices... };
        constexpr bool writable{ chlm::detail::swizzle_indices_unique_v<Indices...> };

        int seed[4]{ 10, -20, 30, -40 };

        if constexpr (length == 2)
        {
            using swizzle_type = chlm::detail::ivector_swizzle2<OwnerComponents, Indices...>;
            swizzle_type swizzle{};
            for (int lane = 0; lane < OwnerComponents; ++lane) swizzle.lanes[lane] = seed[lane];
            const chlm::int2 read_value{ static_cast<chlm::int2>(swizzle) };
            ok = ok && read_value.x == seed[indices[0]] && read_value.y == seed[indices[1]];
            ++read_cases;
            if constexpr (writable)
            {
                swizzle = chlm::int2{ 101, -102 };
                ok = ok && swizzle.lanes[indices[0]] == 101 && swizzle.lanes[indices[1]] == -102;
                ++write_cases;
            }
        }
        else if constexpr (length == 3)
        {
            using swizzle_type = chlm::detail::ivector_swizzle3<OwnerComponents, Indices...>;
            swizzle_type swizzle{};
            for (int lane = 0; lane < OwnerComponents; ++lane) swizzle.lanes[lane] = seed[lane];
            const chlm::int3 read_value{ static_cast<chlm::int3>(swizzle) };
            ok = ok && read_value.x == seed[indices[0]] && read_value.y == seed[indices[1]] && read_value.z == seed[indices[2]];
            ++read_cases;
            if constexpr (writable)
            {
                swizzle = chlm::int3{ 101, -102, 103 };
                ok = ok &&
                     swizzle.lanes[indices[0]] == 101 &&
                     swizzle.lanes[indices[1]] == -102 &&
                     swizzle.lanes[indices[2]] == 103;
                ++write_cases;
            }
        }
        else if constexpr (length == 4)
        {
            using swizzle_type = chlm::detail::ivector_swizzle4<OwnerComponents, Indices...>;
            swizzle_type swizzle{};
            for (int lane = 0; lane < OwnerComponents; ++lane) swizzle.lanes[lane] = seed[lane];
            const chlm::int4 read_value{ static_cast<chlm::int4>(swizzle) };
            ok = ok &&
                 read_value.x == seed[indices[0]] &&
                 read_value.y == seed[indices[1]] &&
                 read_value.z == seed[indices[2]] &&
                 read_value.w == seed[indices[3]];
            ++read_cases;
            if constexpr (writable)
            {
                swizzle = chlm::int4{ 101, -102, 103, -104 };
                ok = ok &&
                     swizzle.lanes[indices[0]] == 101 &&
                     swizzle.lanes[indices[1]] == -102 &&
                     swizzle.lanes[indices[2]] == 103 &&
                     swizzle.lanes[indices[3]] == -104;
                ++write_cases;
            }
        }
    }

    template<int OwnerComponents, int Length, int... Prefix>
    void enumerate_int_swizzle_cases(bool& ok, int& read_cases, int& write_cases)
    {
        if constexpr (sizeof...(Prefix) == Length)
        {
            validate_int_swizzle_case<OwnerComponents, Prefix...>(ok, read_cases, write_cases);
        }
        else
        {
            enumerate_int_swizzle_cases<OwnerComponents, Length, Prefix..., 0>(ok, read_cases, write_cases);
            if constexpr (OwnerComponents > 1) enumerate_int_swizzle_cases<OwnerComponents, Length, Prefix..., 1>(ok, read_cases, write_cases);
            if constexpr (OwnerComponents > 2) enumerate_int_swizzle_cases<OwnerComponents, Length, Prefix..., 2>(ok, read_cases, write_cases);
            if constexpr (OwnerComponents > 3) enumerate_int_swizzle_cases<OwnerComponents, Length, Prefix..., 3>(ok, read_cases, write_cases);
        }
    }
}

void run_int_vector_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Int Vector Tests");

    static_assert(sizeof(int2) == 8);
    static_assert(sizeof(int3) == 12);
    static_assert(sizeof(int4) == 16);

    int2 uv{ 1, -2 };
    uv.r = 3;
    uv.v = -4;

    int3 coord{ 5, -6, 7 };
    coord.rgb = int3{ 8, -9, 10 };

    int4 mask{ 1, -2, 3, -4 };
    mask.stpq = int4{ 5, -6, 7, -8 };

    ctx.expect(
        uv.x == 3 && uv.y == -4 &&
        coord.x == 8 && coord.y == -9 && coord.z == 10 &&
        mask.x == 5 && mask.y == -6 && mask.z == 7 && mask.w == -8,
        "Int alias family test"
    );

    const int2 sum2{ int2{ 8, -4 } + int2{ 2, 1 } };
    const int3 sum3{ int3{ 8, -4, 2 } + int3{ 2, 1, -1 } };
    const int4 sum4{ int4{ 8, -4, 2, -1 } + int4{ 2, 1, -1, 1 } };

    int2 texcoord{ 10, -20 };
    const int3 uvu{ texcoord.uvu };
    texcoord.yx = int2{ 30, -40 };

    int3 normal{ 1, -2, 3 };
    const int4 xyzx{ normal.xyzx };
    normal.zyx = int3{ 6, -5, 4 };

    ctx.expect(
        test_equal(sum2, int2{ 10, -3 }) &&
        test_equal(sum3, int3{ 10, -3, 1 }) &&
        test_equal(sum4, int4{ 10, -3, 1, 0 }) &&
        test_equal(int2{ 8, -4 } - int2{ 2, 1 }, int2{ 6, -5 }) &&
        test_equal(int3{ 8, -4, 2 } * int3{ 2, 3, -4 }, int3{ 16, -12, -8 }) &&
        test_equal(int4{ 8, -4, 2, -8 } / int4{ 2, 2, -2, 4 }, int4{ 4, -2, -1, -2 }) &&
        test_equal(-int2{ 8, -4 }, int2{ -8, 4 }) &&
        test_equal(uvu, int3{ 10, -20, 10 }) &&
        test_equal(texcoord, int2{ -40, 30 }) &&
        test_equal(xyzx, int4{ 1, -2, 3, 1 }) &&
        test_equal(normal, int3{ 4, -5, 6 }),
        "Int arithmetic and swizzle test"
    );

    bool ok{ true };
    int read_cases{ 0 };
    int write_cases{ 0 };
    enumerate_int_swizzle_cases<2, 2>(ok, read_cases, write_cases);
    enumerate_int_swizzle_cases<2, 3>(ok, read_cases, write_cases);
    enumerate_int_swizzle_cases<2, 4>(ok, read_cases, write_cases);
    ctx.expect(ok, "Exhaustive int2-owner swizzle semantics");
    test_println("  int2-owner swizzles: {} read cases, {} write cases", read_cases, write_cases);

    ok = true; read_cases = 0; write_cases = 0;
    enumerate_int_swizzle_cases<3, 2>(ok, read_cases, write_cases);
    enumerate_int_swizzle_cases<3, 3>(ok, read_cases, write_cases);
    enumerate_int_swizzle_cases<3, 4>(ok, read_cases, write_cases);
    ctx.expect(ok, "Exhaustive int3-owner swizzle semantics");
    test_println("  int3-owner swizzles: {} read cases, {} write cases", read_cases, write_cases);

    ok = true; read_cases = 0; write_cases = 0;
    enumerate_int_swizzle_cases<4, 2>(ok, read_cases, write_cases);
    enumerate_int_swizzle_cases<4, 3>(ok, read_cases, write_cases);
    enumerate_int_swizzle_cases<4, 4>(ok, read_cases, write_cases);
    ctx.expect(ok, "Exhaustive int4-owner swizzle semantics");
    test_println("  int4-owner swizzles: {} read cases, {} write cases", read_cases, write_cases);
}
