//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <array>
#include <cstdint>
#include <limits>
#include <type_traits>

static_assert(std::is_trivially_copyable_v<chlm::rng32_state>);

constexpr chlm::rng32 constexpr_rng{ 42u, 54u };
static_assert(constexpr_rng.state().stream == 109u);

void run_random_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Random Tests");

    rng32 reference_rng{ 42u, 54u };
    constexpr std::array<std::uint32_t, 6> reference_sequence{
        0xA15C02B7u,
        0x7B47F409u,
        0xBA1D3330u,
        0x83D2F293u,
        0xBFA4784Bu,
        0xCBED606Eu
    };
    bool reference_matches{ true };
    for (const std::uint32_t expected : reference_sequence)
        reference_matches = reference_matches && reference_rng.next_u32() == expected;
    ctx.expect(reference_matches, "PCG32 reference sequence test");

    rng32 first{ 123456789u };
    rng32 second{ 123456789u };
    bool same_seed_matches{ true };
    for (int i{ 0 }; i < 1000; ++i)
        same_seed_matches = same_seed_matches && first.next_u32() == second.next_u32();
    ctx.expect(same_seed_matches, "same seed determinism test");

    rng32 stream_a{ 99u, 1u };
    rng32 stream_b{ 99u, 2u };
    bool streams_differ{ false };
    for (int i{ 0 }; i < 16; ++i)
        streams_differ = streams_differ || stream_a.next_u32() != stream_b.next_u32();
    ctx.expect(streams_differ, "independent stream test");

    rng32 reseeded{ 7u };
    const std::uint32_t initial_value{ reseeded.next_u32() };
    static_cast<void>(reseeded.next_u64());
    reseeded.seed(7u);
    ctx.expect(reseeded.next_u32() == initial_value, "reseed sequence restart test");

    rng32 state_rng{ 987654321u, 17u };
    for (int i{ 0 }; i < 25; ++i)
        static_cast<void>(state_rng.next_u32());
    const rng32_state saved_state{ state_rng.state() };
    std::array<std::uint32_t, 32> future{};
    for (std::uint32_t& value : future)
        value = state_rng.next_u32();
    state_rng.set_state(saved_state);
    bool state_replays{ true };
    for (const std::uint32_t expected : future)
        state_replays = state_replays && state_rng.next_u32() == expected;
    ctx.expect(state_replays, "state save and restore test");

    rng32 combined_a{ 321u };
    rng32 combined_b{ 321u };
    const std::uint64_t expected_u64{
        static_cast<std::uint64_t>(combined_b.next_u32()) << 32u | combined_b.next_u32()
    };
    ctx.expect(combined_a.next_u64() == expected_u64, "next_u64 composition test");

    rng32 range_rng{ 2468u };
    bool integer_ranges_valid{ true };
    for (int i{ 0 }; i < 100000; ++i)
    {
        const std::uint32_t unsigned_value{ range_rng.next_uint(10u, 27u) };
        const int signed_value{ range_rng.next_int(-13, 8) };
        integer_ranges_valid = integer_ranges_valid &&
                               unsigned_value >= 10u && unsigned_value <= 27u &&
                               signed_value >= -13 && signed_value <= 8;
    }
    ctx.expect(integer_ranges_valid, "integer range bounds test");

    ctx.expect(range_rng.next_uint(5u, 5u) == 5u && range_rng.next_int(-4, -4) == -4,
               "degenerate integer range test");

    rng32 full_unsigned_rng{ 112233u };
    rng32 raw_unsigned_rng{ 112233u };
    rng32 full_signed_rng{ 445566u };
    rng32 raw_signed_rng{ 445566u };
    bool full_ranges_match_raw_bits{ true };
    for (int i{ 0 }; i < 1000; ++i)
    {
        const std::uint32_t unsigned_value{
            full_unsigned_rng.next_uint(0u, std::numeric_limits<std::uint32_t>::max())
        };
        const int signed_value{
            full_signed_rng.next_int(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())
        };
        const int expected_signed{
            static_cast<int>(
                static_cast<std::int64_t>(std::numeric_limits<int>::min()) +
                raw_signed_rng.next_u32())
        };
        full_ranges_match_raw_bits = full_ranges_match_raw_bits &&
                                     unsigned_value == raw_unsigned_rng.next_u32() &&
                                     signed_value == expected_signed;
    }
    ctx.expect(full_ranges_match_raw_bits, "full-width integer range test");

    rng32 float_rng{ 13579u };
    bool float_ranges_valid{ true };
    for (int i{ 0 }; i < 100000; ++i)
    {
        const float unit{ float_rng.next_float() };
        const float ranged{ float_rng.next_float(-7.5f, 2.25f) };
        float_ranges_valid = float_ranges_valid &&
                             unit >= 0.f && unit < 1.f &&
                             ranged >= -7.5f && ranged < 2.25f;
    }
    ctx.expect(float_ranges_valid, "half-open float range test");
    ctx.expect(float_rng.next_float(3.5f, 3.5f) == 3.5f, "degenerate float range test");

    bool chance_endpoints_valid{ true };
    bool saw_true{ false };
    bool saw_false{ false };
    for (int i{ 0 }; i < 1000; ++i)
    {
        chance_endpoints_valid = chance_endpoints_valid &&
                                 !float_rng.chance(0.f) && float_rng.chance(1.f);
        const bool value{ float_rng.next_bool() };
        saw_true = saw_true || value;
        saw_false = saw_false || !value;
    }
    ctx.expect(chance_endpoints_valid, "chance endpoint test");
    ctx.expect(saw_true && saw_false, "next_bool outcome test");

    rng32 bias_rng{ 86420u };
    constexpr int bucket_count{ 6 };
    constexpr int sample_count{ 120000 };
    std::array<int, bucket_count> buckets{};
    for (int i{ 0 }; i < sample_count; ++i)
        ++buckets[bias_rng.next_int(0, bucket_count - 1)];

    bool frequencies_sane{ true };
    constexpr int expected_frequency{ sample_count / bucket_count };
    constexpr int tolerance{ 700 };
    for (const int frequency : buckets)
        frequencies_sane = frequencies_sane &&
                           frequency >= expected_frequency - tolerance &&
                           frequency <= expected_frequency + tolerance;
    ctx.expect(frequencies_sane, "bounded integer distribution sanity test");

    rng32 geometry_rng{ 97531u };
    bool vector_ranges_valid{ true };
    bool geometry_domains_valid{ true };
    bool unit_lengths_valid{ true };
    float2 circle_mean{};
    float3 sphere_mean{};
    float circle_radius_squared_mean{ 0.f };
    float sphere_radius_squared_mean{ 0.f };
    constexpr int geometry_sample_count{ 100000 };

    for (int i{ 0 }; i < geometry_sample_count; ++i)
    {
        const float2 ranged2{
            random_float2(geometry_rng, { -2.f, 10.f }, { 3.f, 20.f })
        };
        const float3 ranged3{
            random_float3(geometry_rng, { -3.f, 4.f, 20.f }, { -1.f, 8.f, 30.f })
        };
        const float4 ranged4{
            random_float4(geometry_rng, { 1.f, 2.f, 3.f, 4.f }, { 2.f, 3.f, 4.f, 5.f })
        };
        vector_ranges_valid = vector_ranges_valid &&
                              ranged2.x >= -2.f && ranged2.x < 3.f &&
                              ranged2.y >= 10.f && ranged2.y < 20.f &&
                              ranged3.x >= -3.f && ranged3.x < -1.f &&
                              ranged3.y >= 4.f && ranged3.y < 8.f &&
                              ranged3.z >= 20.f && ranged3.z < 30.f &&
                              ranged4.x >= 1.f && ranged4.x < 2.f &&
                              ranged4.y >= 2.f && ranged4.y < 3.f &&
                              ranged4.z >= 3.f && ranged4.z < 4.f &&
                              ranged4.w >= 4.f && ranged4.w < 5.f;

        const float2 unit2{ random_unit_vector2(geometry_rng) };
        const float3 unit3{ random_unit_vector3(geometry_rng) };
        const float2 circle_point{ random_point_in_unit_circle(geometry_rng) };
        const float3 sphere_point{ random_point_in_unit_sphere(geometry_rng) };
        const float2 circle_surface{ random_point_on_unit_circle(geometry_rng) };
        const float3 sphere_surface{ random_point_on_unit_sphere(geometry_rng) };

        const float circle_radius_squared{ length_squared(circle_point) };
        const float sphere_radius_squared{ length_squared(sphere_point) };
        geometry_domains_valid = geometry_domains_valid &&
                                 circle_radius_squared < 1.f &&
                                 sphere_radius_squared < 1.f;
        unit_lengths_valid = unit_lengths_valid &&
                             test_almost_equal(length_squared(unit2), 1.f, 2e-4f) &&
                             test_almost_equal(length_squared(unit3), 1.f, 2e-4f) &&
                             test_almost_equal(length_squared(circle_surface), 1.f, 2e-4f) &&
                             test_almost_equal(length_squared(sphere_surface), 1.f, 2e-4f);

        circle_mean += circle_point;
        sphere_mean += sphere_point;
        circle_radius_squared_mean += circle_radius_squared;
        sphere_radius_squared_mean += sphere_radius_squared;
    }

    const float inverse_geometry_sample_count{ 1.f / geometry_sample_count };
    circle_mean *= inverse_geometry_sample_count;
    sphere_mean *= inverse_geometry_sample_count;
    circle_radius_squared_mean *= inverse_geometry_sample_count;
    sphere_radius_squared_mean *= inverse_geometry_sample_count;

    ctx.expect(vector_ranges_valid, "random vector component range test");
    ctx.expect(geometry_domains_valid, "unit circle and sphere interior domain test");
    ctx.expect(unit_lengths_valid, "unit circle and sphere surface length test");
    ctx.expect(abs(circle_mean.x) < .01f && abs(circle_mean.y) < .01f &&
               abs(sphere_mean.x) < .01f && abs(sphere_mean.y) < .01f && abs(sphere_mean.z) < .01f,
               "geometry sampling symmetry sanity test");
    ctx.expect(abs(circle_radius_squared_mean - .5f) < .01f &&
               abs(sphere_radius_squared_mean - .6f) < .01f,
               "geometry area and volume distribution sanity test");

    rng32 geometry_first{ 1234u };
    rng32 geometry_second{ 1234u };
    bool geometry_deterministic{ true };
    for (int i{ 0 }; i < 100; ++i)
    {
        geometry_deterministic = geometry_deterministic &&
                                 test_almost_equal(random_unit_vector2(geometry_first),
                                                   random_unit_vector2(geometry_second), 0.f) &&
                                 test_almost_equal(random_point_in_unit_sphere(geometry_first),
                                                   random_point_in_unit_sphere(geometry_second), 0.f);
    }
    ctx.expect(geometry_deterministic, "geometry sampling determinism test");
}
