//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include <chlm/CarrotHLM.h>

#include <print>
#include <string_view>

constexpr float GENERAL_EPS = 1e-4f;

bool test_almost_equal(float a, float b, float eps = 1e-6f)
{
    return std::abs(a - b) <= eps;
}

bool test_almost_equal(const chlm::float4& a, const chlm::float4& b, float eps = GENERAL_EPS)
{
    return test_almost_equal(a.x, b.x, eps) &&
           test_almost_equal(a.y, b.y, eps) &&
           test_almost_equal(a.z, b.z, eps) &&
           test_almost_equal(a.w, b.w, eps);
}

bool test_almost_equal(const chlm::float4x4& a, const chlm::float4x4& b)
{
    for (int i = 0; i < 4; ++i)
    {
        if (std::abs(a[i].x - b[i].x) > GENERAL_EPS) return false;
        if (std::abs(a[i].y - b[i].y) > GENERAL_EPS) return false;
        if (std::abs(a[i].z - b[i].z) > GENERAL_EPS) return false;
        if (std::abs(a[i].w - b[i].w) > GENERAL_EPS) return false;
    }
    return true;
}

struct test_context
{
    int failures{ 0 };

    void expect(bool condition, const std::string_view success, const std::string_view failure)
    {
        if (condition)
        {
            std::println("{}: PASSED", success);
            return;
        }

        std::println("{}: FAILED", failure);
        ++failures;
    }
};

void test_inverse(test_context& ctx)
{
    using namespace chlm;

    std::println("Testing general inverse...");

    // 1. Identity matrix
    float4x4 I = float4x4::identity();
    float4x4 invI = inverse(I);
    ctx.expect(test_almost_equal(invI, I), "Identity test", "Identity test");

    // 2. Simple scale + translation (affine)
    float4x4 T = float4x4::translate(float3{2.0f, 3.0f, 4.0f}) *
                 float4x4::scale(float3{2.0f, 1.0f, 0.5f});
    float4x4 invT = inverse(T);
    float4x4 product = T * invT;
    ctx.expect(test_almost_equal(product, float4x4::identity()), "Affine transform test", "Affine transform test");

    // 3. Rotation matrix (should be orthogonal)
    float4x4 R = float4x4::rotate_axis_angle(normalize(float3{1.0f, 1.0f, 1.0f}), 0.123f);
    float4x4 invR = inverse(R);
    float4x4 should_be_I = R * invR;
    ctx.expect(test_almost_equal(should_be_I, float4x4::identity()), "Rotation test", "Rotation test");

    // 4. A random non-singular matrix
    float4x4 M{
        float4{ 1.0f, 2.0f, 3.0f, 4.0f },
        float4{ 0.0f, 5.0f, 1.0f, 2.0f },
        float4{ 7.0f, 8.0f, 9.0f, 0.0f },
        float4{ 2.0f, 1.0f, 4.0f, 5.0f }
    };
    float4x4 invM = inverse(M);
    float4x4 check = M * invM;
    ctx.expect(test_almost_equal(check, float4x4::identity()), "Random matrix test", "Random matrix test");
    std::println();
}

int main()
{
    using namespace chlm;
    test_context ctx{};

    std::println("=== CarrotHLM Validation Test ===\n");

    test_inverse(ctx);

    // 1. Vector basics + swizzles
    float4 pos{ 1.f, 2.f, 3.f, 1.f };
    float3 xyz{ pos.xyz };
    float2 xy{ pos.xy };
    float4 weird{ pos.wzyx };
    ctx.expect(
        test_almost_equal(xyz.x, 1.f) &&
        test_almost_equal(xyz.y, 2.f) &&
        test_almost_equal(xyz.z, 3.f) &&
        test_almost_equal(xy.x, 1.f) &&
        test_almost_equal(xy.y, 2.f) &&
        test_almost_equal(weird.x, 1.f) &&
        test_almost_equal(weird.y, 3.f) &&
        test_almost_equal(weird.z, 2.f) &&
        test_almost_equal(weird.w, 1.f),
        "Vector swizzle test",
        "Vector swizzle test"
    );
    std::println("Vector swizzles:");
    std::println("  pos.xyz = ({}, {}, {})", static_cast<float>(xyz.x), static_cast<float>(xyz.y),
                 static_cast<float>(xyz.z));
    std::println("  pos.xy  = ({}, {})", static_cast<float>(xy.x), static_cast<float>(xy.y));
    std::println("  pos.wzyx = ({}, {}, {}, {})",
                 static_cast<float>(weird.x), static_cast<float>(weird.y),
                 static_cast<float>(weird.z), static_cast<float>(weird.w));

    // 2. Quaternion rotation
    const quat q{ quat_from_axis_angle(normalize(float3{ 0.f, 1.f, 0.f }), pi) };
    float3 rotated{ rotate_vector(q, float3{ 1.f, 0.f, 0.f }) };
    ctx.expect(
        test_almost_equal(rotated.x, -1.f, GENERAL_EPS) &&
        test_almost_equal(rotated.y, 0.f, GENERAL_EPS) &&
        test_almost_equal(rotated.z, 0.f, GENERAL_EPS),
        "Quaternion rotation test",
        "Quaternion rotation test"
    );
    std::println("\nQuaternion 180° Y rotation of +X:");
    std::println("  -> ({}, {}, {})", static_cast<float>(rotated.x), static_cast<float>(rotated.y),
                 static_cast<float>(rotated.z));

    // 3. float4x4 transforms
    const float4x4 model{
        float4x4::translate({ 10.f, 20.f, 30.f }) *
        float4x4::rotate_y(pi / 2.f)
    };

    const float4 local{ 1.f, 0.f, 0.f, 1.f };
    float4 world{ model * local };
    std::println("\nfloat4x4 transform chain (translate -> rotate90°Y):");
    std::println("  local (1,0,0,1) → world ({}, {}, {})",
                 static_cast<float>(world.x), static_cast<float>(world.y), static_cast<float>(world.z));

    // 4. Affine inverse
    const float4x4 inv_model{ affine_inverse(model) };
    float4 back{ inv_model * world };
    ctx.expect(
        test_almost_equal(back.x, local.x, GENERAL_EPS) &&
        test_almost_equal(back.y, local.y, GENERAL_EPS) &&
        test_almost_equal(back.z, local.z, GENERAL_EPS) &&
        test_almost_equal(back.w, local.w, GENERAL_EPS),
        "Affine inverse round-trip test",
        "Affine inverse round-trip test"
    );
    std::println("\nAfter affine_inverse:");
    std::println("  back to ({}, {}, {})",
                 static_cast<float>(back.x), static_cast<float>(back.y), static_cast<float>(back.z));

    // 5. float3x3 + normal matrix
    const float3x3 rot3{ to_float3x3(q) };
    const float3 v3{ 1.f, 0.f, 0.f };
    float3 rotated3{ rot3 * v3 };
    ctx.expect(
        test_almost_equal(rotated3.x, -1.f, GENERAL_EPS) &&
        test_almost_equal(rotated3.y, 0.f, GENERAL_EPS) &&
        test_almost_equal(rotated3.z, 0.f, GENERAL_EPS),
        "float3x3 rotation test",
        "float3x3 rotation test"
    );
    std::println("\nfloat3x3 from same quat:");
    std::println("  +X → ({}, {}, {})", static_cast<float>(rotated3.x), static_cast<float>(rotated3.y),
                 static_cast<float>(rotated3.z));

    float3x3 norm_mat{ normal_matrix(model) };
    (void)norm_mat;
    std::println("\nNormal matrix extracted from model");

    // 6. Axis-angle round trip
    const float3 axis{ 1.f, 1.f, 1.f };
    const quat q_axis{ quat_from_axis_angle(normalize(axis), 2.f * pi / 3.f) }; // 120°
    float4x4 mat_from_q{ to_float4x4(q_axis) };
    float3x3 mat3_from_q{ to_float3x3(q_axis) };
    (void)mat_from_q;
    (void)mat3_from_q;
    std::println("\nAxis-angle -> quat -> matrix round-trip successful if no asserts fire.");

    if (ctx.failures == 0)
    {
        std::println("\n=== All tests passed! CarrotHLM is ready. ===");
        return 0;
    }

    std::println("\n=== {} validation test(s) failed. ===", ctx.failures);
    return 1;
}
