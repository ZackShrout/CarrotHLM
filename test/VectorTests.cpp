//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

void run_vector_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Vector Tests");

    ctx.expect(sizeof(float2) == 8, "float2 packed size test");
    ctx.expect(sizeof(float3) == 12, "float3 packed size test");
    ctx.expect(sizeof(float4) == 16, "float4 size test");

    const float4 pos{ 1.f, 2.f, 3.f, 1.f };
    const float3 xyz{ pos.xyz };
    const float2 xy{ pos.xy };
    const float4 weird{ pos.yxwz };
    const float4 repeated{ pos.aaaa };
    const float3 rgb{ pos.rgb };
    const float2 st{ pos.st };

    ctx.expect(
        test_almost_equal(xyz.x, 1.f) &&
        test_almost_equal(xyz.y, 2.f) &&
        test_almost_equal(xyz.z, 3.f) &&
        test_almost_equal(xy.x, 1.f) &&
        test_almost_equal(xy.y, 2.f) &&
        test_almost_equal(weird.x, 2.f) &&
        test_almost_equal(weird.y, 1.f) &&
        test_almost_equal(weird.z, 1.f) &&
        test_almost_equal(weird.w, 3.f) &&
        test_almost_equal(repeated.x, 1.f) &&
        test_almost_equal(repeated.y, 1.f) &&
        test_almost_equal(repeated.z, 1.f) &&
        test_almost_equal(repeated.w, 1.f) &&
        test_almost_equal(rgb.x, 1.f) &&
        test_almost_equal(rgb.y, 2.f) &&
        test_almost_equal(rgb.z, 3.f) &&
        test_almost_equal(st.x, 1.f) &&
        test_almost_equal(st.y, 2.f),
        "Vector swizzle test"
    );

    test_println("  pos.xyz = ({}, {}, {})",
                 static_cast<float>(xyz.x),
                 static_cast<float>(xyz.y),
                 static_cast<float>(xyz.z));
    test_println("  pos.xy  = ({}, {})",
                 static_cast<float>(xy.x),
                 static_cast<float>(xy.y));
    test_println("  pos.yxwz = ({}, {}, {}, {})",
                 static_cast<float>(weird.x),
                 static_cast<float>(weird.y),
                 static_cast<float>(weird.z),
                 static_cast<float>(weird.w));
    test_println("  pos.aaaa = ({}, {}, {}, {})",
                 static_cast<float>(repeated.x),
                 static_cast<float>(repeated.y),
                 static_cast<float>(repeated.z),
                 static_cast<float>(repeated.w));
    test_println("  pos.rgb = ({}, {}, {})",
                 static_cast<float>(rgb.x),
                 static_cast<float>(rgb.y),
                 static_cast<float>(rgb.z));
    test_println("  pos.st = ({}, {})",
                 static_cast<float>(st.x),
                 static_cast<float>(st.y));

    const float2 sum2{ float2{ 1.f, 2.f } + float2{ 3.f, 4.f } };
    const float3 sum3{ float3{ 1.f, 2.f, 3.f } + float3{ 4.f, 5.f, 6.f } };
    const float3 cross_result{ cross(float3{ 1.f, 0.f, 0.f }, float3{ 0.f, 1.f, 0.f }) };
    float2 texcoord{ 1.f, 2.f };
    const float2 yx{ texcoord.yx };
    const float3 uvu{ texcoord.uvu };
    const float4 uvuv{ texcoord.uvuv };
    texcoord.yx = float2{ 7.f, 8.f };
    texcoord.uv = float2{ 3.f, 4.f };

    float3 normal{ 1.f, 2.f, 3.f };
    const float2 rg{ normal.rg };
    const float3 stp{ normal.stp };
    const float4 xyzx{ normal.xyzx };
    normal.xy = float2{ 9.f, 8.f };
    normal.zyx = float3{ 6.f, 5.f, 4.f };

    ctx.expect(
        test_almost_equal(sum2, float2{ 4.f, 6.f }) &&
        test_almost_equal(sum3, float3{ 5.f, 7.f, 9.f }) &&
        test_almost_equal(cross_result, float3{ 0.f, 0.f, 1.f }) &&
        test_almost_equal(yx, float2{ 2.f, 1.f }) &&
        test_almost_equal(uvu, float3{ 1.f, 2.f, 1.f }) &&
        test_almost_equal(uvuv, float4{ 1.f, 2.f, 1.f, 2.f }) &&
        test_almost_equal(texcoord, float2{ 3.f, 4.f }) &&
        test_almost_equal(rg, float2{ 1.f, 2.f }) &&
        test_almost_equal(stp, float3{ 1.f, 2.f, 3.f }) &&
        test_almost_equal(xyzx, float4{ 1.f, 2.f, 3.f, 1.f }) &&
        test_almost_equal(normal, float3{ 4.f, 5.f, 6.f }),
        "Portable float2/float3 arithmetic and swizzle test"
    );
}
