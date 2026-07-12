//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <chlm/detail/SwizzleProxy.h>

namespace {
    using chlm::detail::float2_base;
    using chlm::detail::float4_base;
    using chlm::detail::make_swizzle;
    using chlm::detail::swizzle_proxy;

    static_assert(swizzle_proxy<float4_base, 0, 1>::writable);
    static_assert(swizzle_proxy<float4_base, 1, 0>::writable);
    static_assert(!swizzle_proxy<float4_base, 0, 0>::writable);
    static_assert(!swizzle_proxy<float4_base, 3, 3, 3, 3>::writable);
}

void run_swizzle_scaffolding_tests(test_context& ctx)
{
    using namespace chlm::detail;

    ctx.section("Swizzle Scaffolding Tests");

    float4_base a{ 10.f, 20.f, 30.f, 40.f };
    const auto xyz = static_cast<float3_base>(make_swizzle<0, 1, 2>(a));
    ctx.expect(
        test_almost_equal(xyz.lane<0>(), 10.f) &&
        test_almost_equal(xyz.lane<1>(), 20.f) &&
        test_almost_equal(xyz.lane<2>(), 30.f),
        "Read swizzle xyz test"
    );

    const auto yxwz = static_cast<float4_base>(make_swizzle<1, 0, 3, 2>(a));
    ctx.expect(
        test_almost_equal(yxwz.lane<0>(), 20.f) &&
        test_almost_equal(yxwz.lane<1>(), 10.f) &&
        test_almost_equal(yxwz.lane<2>(), 40.f) &&
        test_almost_equal(yxwz.lane<3>(), 30.f),
        "Read swizzle yxwz test"
    );

    make_swizzle<0, 1>(a) = float2_base{ 1.f, 2.f };
    ctx.expect(
        test_almost_equal(a.lane<0>(), 1.f) &&
        test_almost_equal(a.lane<1>(), 2.f) &&
        test_almost_equal(a.lane<2>(), 30.f) &&
        test_almost_equal(a.lane<3>(), 40.f),
        "Write swizzle xy test"
    );

    make_swizzle<1, 0>(a) = float2_base{ 7.f, 8.f };
    ctx.expect(
        test_almost_equal(a.lane<0>(), 8.f) &&
        test_almost_equal(a.lane<1>(), 7.f) &&
        test_almost_equal(a.lane<2>(), 30.f) &&
        test_almost_equal(a.lane<3>(), 40.f),
        "Write swizzle yx test"
    );

    float4_base b{ 1.f, 2.f, 3.f, 4.f };
    make_swizzle<2, 3>(a) = make_swizzle<0, 1>(b);
    ctx.expect(
        test_almost_equal(a.lane<0>(), 8.f) &&
        test_almost_equal(a.lane<1>(), 7.f) &&
        test_almost_equal(a.lane<2>(), 1.f) &&
        test_almost_equal(a.lane<3>(), 2.f),
        "Proxy to proxy assignment test"
    );

    const auto xxxx = static_cast<float4_base>(make_swizzle<0, 0, 0, 0>(a));
    ctx.expect(
        test_almost_equal(xxxx.lane<0>(), 8.f) &&
        test_almost_equal(xxxx.lane<1>(), 8.f) &&
        test_almost_equal(xxxx.lane<2>(), 8.f) &&
        test_almost_equal(xxxx.lane<3>(), 8.f),
        "Repeated-lane read swizzle test"
    );
}
