//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

void run_quaternion_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Quaternion Tests");

    const quat q{ quat_from_axis_angle(normalize(float3{ 0.f, 1.f, 0.f }), pi) };
    const float3 rotated{ rotate_vector(q, float3{ 1.f, 0.f, 0.f }) };

    ctx.expect(
        test_almost_equal(rotated.x, -1.f, GENERAL_EPS) &&
        test_almost_equal(rotated.y, 0.f, GENERAL_EPS) &&
        test_almost_equal(rotated.z, 0.f, GENERAL_EPS),
        "Quaternion rotation test"
    );

    test_println("  180 degree Y rotation of +X -> ({}, {}, {})",
                 static_cast<float>(rotated.x),
                 static_cast<float>(rotated.y),
                 static_cast<float>(rotated.z));

    const float3 axis{ 1.f, 1.f, 1.f };
    const quat q_axis{ quat_from_axis_angle(normalize(axis), 2.f * pi / 3.f) };
    const float4x4 mat_from_q{ to_float4x4(q_axis) };
    const float3x3 mat3_from_q{ to_float3x3(q_axis) };
    (void)mat_from_q;
    (void)mat3_from_q;

    ctx.expect(true, "Axis-angle to matrix conversion smoke test");
}
