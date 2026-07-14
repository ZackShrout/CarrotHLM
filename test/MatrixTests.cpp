//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

namespace {
    void run_inverse_tests(test_context& ctx)
    {
        using namespace chlm;

        ctx.section("Matrix Inverse Tests");

        const float4x4 identity = float4x4::identity();
        const float4x4 inv_identity = inverse(identity);
        ctx.expect(test_almost_equal(inv_identity, identity), "Identity inverse test");

        const float4x4 affine = float4x4::translate(float3{ 2.0f, 3.0f, 4.0f }) *
                                float4x4::scale(float3{ 2.0f, 1.0f, 0.5f });
        const float4x4 inv_affine = inverse(affine);
        const float4x4 affine_product = affine * inv_affine;
        ctx.expect(test_almost_equal(affine_product, float4x4::identity()), "Affine inverse test");

        const float4x4 rotation = float4x4::rotate_axis_angle(normalize(float3{ 1.0f, 1.0f, 1.0f }), 0.123f);
        const float4x4 inv_rotation = inverse(rotation);
        const float4x4 should_be_identity = rotation * inv_rotation;
        ctx.expect(test_almost_equal(should_be_identity, float4x4::identity()), "Rotation inverse test");

        const float4x4 random_matrix{
            float4{ 1.0f, 2.0f, 3.0f, 4.0f },
            float4{ 0.0f, 5.0f, 1.0f, 2.0f },
            float4{ 7.0f, 8.0f, 9.0f, 0.0f },
            float4{ 2.0f, 1.0f, 4.0f, 5.0f }
        };
        const float4x4 inv_random = inverse(random_matrix);
        const float4x4 random_product = random_matrix * inv_random;
        ctx.expect(test_almost_equal(random_product, float4x4::identity()), "Random matrix inverse test");
    }
}

void run_matrix_tests(test_context& ctx)
{
    using namespace chlm;

    run_inverse_tests(ctx);

    ctx.section("Matrix Transform Tests");

    const float4x4 model{
        float4x4::translate({ 10.f, 20.f, 30.f }) *
        float4x4::rotate_y(pi / 2.f)
    };

    const float4 local{ 1.f, 0.f, 0.f, 1.f };
    const float4 world{ model * local };

    test_println("  local (1,0,0,1) -> world ({}, {}, {})",
                 static_cast<float>(world.x),
                 static_cast<float>(world.y),
                 static_cast<float>(world.z));

    const float4x4 inv_model{ affine_inverse(model) };
    const float4 back{ inv_model * world };
    ctx.expect(
        test_almost_equal(back.x, local.x, GENERAL_EPS) &&
        test_almost_equal(back.y, local.y, GENERAL_EPS) &&
        test_almost_equal(back.z, local.z, GENERAL_EPS) &&
        test_almost_equal(back.w, local.w, GENERAL_EPS),
        "Affine inverse round-trip test"
    );

    const quat q{ quat_from_axis_angle(normalize(float3{ 0.f, 1.f, 0.f }), pi) };
    const float3x3 rot3{ to_float3x3(q) };
    const float3 v3{ 1.f, 0.f, 0.f };
    const float3 rotated3{ rot3 * v3 };
    ctx.expect(
        test_almost_equal(rotated3.x, -1.f, GENERAL_EPS) &&
        test_almost_equal(rotated3.y, 0.f, GENERAL_EPS) &&
        test_almost_equal(rotated3.z, 0.f, GENERAL_EPS),
        "float3x3 rotation test"
    );

    const float3x3 norm_mat{ normal_matrix(model) };
    (void)norm_mat;
    ctx.expect(true, "Normal matrix smoke test");
}
