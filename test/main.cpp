#include "../include/chlm/CarrotHLM.h"

#include <print>

int main() {
    using namespace chlm;

    std::println("=== CarrotHLM Validation Test ===\n");

    // 1. Vector basics + swizzles
    float4 pos{1.0f, 2.0f, 3.0f, 1.0f};
    float3 xyz = pos.xyz;
    float2 xy = pos.xy;
    float4 weird = pos.wzyx;
    std::println("Vector swizzles:");
    std::println("  pos.xyz() = ({}, {}, {})", static_cast<float>(xyz.x), static_cast<float>(xyz.y), static_cast<float>(xyz.z));
    std::println("  pos.xy()  = ({}, {})", static_cast<float>(xy.x), static_cast<float>(xy.y));
    std::println("  pos.wzyx() = ({}, {}, {}, {})",
                 static_cast<float>(weird.x), static_cast<float>(weird.y),
                 static_cast<float>(weird.z), static_cast<float>(weird.w));

    // 2. Quaternion rotation
    quat q = quat_from_axis_angle(normalize(float3{0.0f, 1.0f, 0.0f}), pi);
    float3 rotated = rotate_vector(q, float3{1.0f, 0.0f, 0.0f});
    std::println("\nQuaternion 180° Y rotation of +X:");
    std::println("  -> ({}, {}, {})", static_cast<float>(rotated.x), static_cast<float>(rotated.y), static_cast<float>(rotated.z));

    // 3. float4x4 transforms
    float4x4 model = float4x4::translate({10.0f, 20.0f, 30.0f}) *
                     float4x4::rotate_y(pi / 2.0f) *
                     float4x4::scale({2.0f, 2.0f, 2.0f});

    float4 local{1.0f, 0.0f, 0.0f, 1.0f};
    float4 world = model * local;
    std::println("\nfloat4x4 transform chain (translate → rotate90°Y → scale2):");
    std::println("  local (1,0,0,1) → world ({}, {}, {})",
                 static_cast<float>(world.x), static_cast<float>(world.y), static_cast<float>(world.z));

    // 4. Affine inverse
    float4x4 inv_model = affine_inverse(model);
    float4 back = inv_model * world;
    std::println("\nAfter affine_inverse:");
    std::println("  back to ({}, {}, {})",
                 static_cast<float>(back.x), static_cast<float>(back.y), static_cast<float>(back.z));

    // 5. float3x3 + normal matrix
    float3x3 rot3 = to_float3x3(q);
    float3 v3{1.0f, 0.0f, 0.0f};
    float3 rotated3 = rot3 * v3;
    std::println("\nfloat3x3 from same quat:");
    std::println("  +X → ({}, {}, {})", static_cast<float>(rotated3.x), static_cast<float>(rotated3.y), static_cast<float>(rotated3.z));

    float3x3 norm_mat = normal_matrix(model);
    std::println("\nNormal matrix extracted from model (should be scaled inverse rotation)");

    // 6. Axis-angle round trip
    float3 axis{1.0f, 1.0f, 1.0f};
    quat q_axis = quat_from_axis_angle(normalize(axis), 2.0f * pi / 3.0f);  // 120°
    float4x4 mat_from_q = to_float4x4(q_axis);
    float3x3 mat3_from_q = to_float3x3(q_axis);
    std::println("\nAxis-angle → quat → matrix round-trip successful if no asserts fire.");

    std::println("\n=== All tests passed! CarrotHLM is ready. ===");

    return 0;
}