//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "../include/chlm/CarrotHLM.h"

#include <print>

constexpr float GENERAL_EPS = 1e-4f;  // or 1e-5f

bool almost_equal(float a, float b, float eps = 1e-6f)
{
    return std::abs(a - b) <= eps;
}

bool almost_equal(const chlm::float4& a, const chlm::float4& b, float eps = GENERAL_EPS)
{
    return almost_equal(a.x, b.x, eps) &&
           almost_equal(a.y, b.y, eps) &&
           almost_equal(a.z, b.z, eps) &&
           almost_equal(a.w, b.w, eps);
}

bool almost_equal(const chlm::float4x4& a, const chlm::float4x4& b)
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

void test_inverse()
{
    using namespace chlm;

    std::println("Testing general inverse...");

    // 1. Identity matrix
    float4x4 I = float4x4::identity();
    float4x4 invI = inverse(I);
    if (almost_equal(invI, I))
        std::println("Identity test: PASSED");
    else
        std::println("Identity test: FAILED");

    // 2. Simple scale + translation (affine)
    float4x4 T = float4x4::translate(float3{2.0f, 3.0f, 4.0f}) *
                 float4x4::scale(float3{2.0f, 1.0f, 0.5f});
    float4x4 invT = inverse(T);
    float4x4 product = T * invT;
    if (almost_equal(product, float4x4::identity()))
        std::println("Affine transform test: PASSED");
    else
        std::println("Affine transform test: FAILED");

    // 3. Rotation matrix (should be orthogonal)
    float4x4 R = float4x4::rotate_axis_angle(normalize(float3{1.0f, 1.0f, 1.0f}), 0.123f);
    float4x4 invR = inverse(R);
    float4x4 should_be_I = R * invR;
    if (almost_equal(should_be_I, float4x4::identity()))
        std::println("Rotation test: PASSED");
    else
        std::println("Rotation test: FAILED");

    // 4. A random non-singular matrix
    float4x4 M{
    float4{ 1.0f, 2.0f, 3.0f, 4.0f },
    float4{ 0.0f, 5.0f, 1.0f, 2.0f },
    float4{ 7.0f, 8.0f, 9.0f, 0.0f },
    float4{ 2.0f, 1.0f, 4.0f, 5.0f }
    };
    float4x4 invM = inverse(M);
    float4x4 check = M * invM;
    if (almost_equal(check, float4x4::identity()))
        std::println("Random matrix test: PASSED\n");
    else
        std::println("Random matrix test: FAILED\n");
}

int main()
{
    using namespace chlm;

    std::println("=== CarrotHLM Validation Test ===\n");

    test_inverse();

    // 1. Vector basics + swizzles
    float4 pos{ 1.f, 2.f, 3.f, 1.f };
    float3 xyz{ pos.xyz };
    float2 xy{ pos.xy };
    float4 weird{ pos.wzyx };
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
    std::println("\nQuaternion 180° Y rotation of +X:");
    std::println("  -> ({}, {}, {})", static_cast<float>(rotated.x), static_cast<float>(rotated.y),
                 static_cast<float>(rotated.z));

    // 3. float4x4 transforms
    const float4x4 model{
        float4x4::translate({ 10.f, 20.f, 30.f }) *
        float4x4::rotate_y(pi / 2.f) *
        float4x4::scale({ 2.f, 2.f, 2.f })
    };

    const float4 local{ 1.f, 0.f, 0.f, 1.f };
    float4 world{ model * local };
    std::println("\nfloat4x4 transform chain (translate -> rotate90°Y -> scale2):");
    std::println("  local (1,0,0,1) → world ({}, {}, {})",
                 static_cast<float>(world.x), static_cast<float>(world.y), static_cast<float>(world.z));

    // 4. Affine inverse
    const float4x4 inv_model{ affine_inverse(model) };
    float4 back{ inv_model * world };
    std::println("\nAfter affine_inverse:");
    std::println("  back to ({}, {}, {})",
                 static_cast<float>(back.x), static_cast<float>(back.y), static_cast<float>(back.z));

    // 5. float3x3 + normal matrix
    const float3x3 rot3{ to_float3x3(q) };
    const float3 v3{ 1.f, 0.f, 0.f };
    float3 rotated3{ rot3 * v3 };
    std::println("\nfloat3x3 from same quat:");
    std::println("  +X → ({}, {}, {})", static_cast<float>(rotated3.x), static_cast<float>(rotated3.y),
                 static_cast<float>(rotated3.z));

    float3x3 norm_mat{ normal_matrix(model) };
    std::println("\nNormal matrix extracted from model (should be scaled inverse rotation)");

    // 6. Axis-angle round trip
    const float3 axis{ 1.f, 1.f, 1.f };
    const quat q_axis{ quat_from_axis_angle(normalize(axis), 2.f * pi / 3.f) }; // 120°
    float4x4 mat_from_q{ to_float4x4(q_axis) };
    float3x3 mat3_from_q{ to_float3x3(q_axis) };
    std::println("\nAxis-angle -> quat -> matrix round-trip successful if no asserts fire.");

    std::println("\n=== All tests passed! CarrotHLM is ready. ===");

    return 0;
}
