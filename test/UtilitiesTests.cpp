//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <algorithm>
#include <cmath>
#include <limits>

static_assert(chlm::inverse_lerp(2.f, 6.f, 4.f) == .5f);
static_assert(chlm::inverse_lerp(6.f, 2.f, 4.f) == .5f);
static_assert(chlm::inverse_lerp(2.f, 2.f, 10.f) == 0.f);
static_assert(chlm::remap(0.f, 1.f, 10.f, 20.f, .5f) == 15.f);
static_assert(chlm::step(1.f, 1.f) == 1.f);
static_assert(chlm::smoothstep(0.f, 1.f, .5f) == .5f);
static_assert(chlm::smootherstep(0.f, 1.f, .5f) == .5f);
static_assert(chlm::move_towards(0.f, 10.f, 2.f) == 2.f);
static_assert(chlm::move_towards(10.f, 0.f, 2.f) == 8.f);
static_assert(chlm::move_towards(0.f, 1.f, 2.f) == 1.f);
static_assert(chlm::repeat(-1.f, 5.f) == 4.f);
static_assert(chlm::wrap(6.f, 2.f, 5.f) == 3.f);
static_assert(chlm::delta_angle(0.f, chlm::pi) == chlm::pi);
static_assert(chlm::ping_pong(3.f, 2.f) == 1.f);

void run_utility_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Utility Tests");

    ctx.expect(inverse_lerp(2.f, 6.f, 2.f) == 0.f &&
               inverse_lerp(2.f, 6.f, 4.f) == .5f &&
               inverse_lerp(2.f, 6.f, 6.f) == 1.f,
               "inverse_lerp endpoints and midpoint test");
    ctx.expect(inverse_lerp(2.f, 6.f, 0.f) == -.5f &&
               inverse_lerp(2.f, 6.f, 8.f) == 1.5f,
               "inverse_lerp extrapolation test");
    ctx.expect(inverse_lerp(6.f, 2.f, 6.f) == 0.f &&
               inverse_lerp(6.f, 2.f, 4.f) == .5f &&
               inverse_lerp(6.f, 2.f, 2.f) == 1.f,
               "inverse_lerp reversed range test");
    ctx.expect(inverse_lerp(3.f, 3.f, -100.f) == 0.f &&
               inverse_lerp(3.f, 3.f, 100.f) == 0.f,
               "inverse_lerp zero-width range test");
    constexpr float maximum{ std::numeric_limits<float>::max() };
    ctx.expect(inverse_lerp(-maximum, maximum, 0.f) == .5f,
               "inverse_lerp extreme finite range test");

    ctx.expect(remap(0.f, 10.f, -1.f, 1.f, 0.f) == -1.f &&
               remap(0.f, 10.f, -1.f, 1.f, 5.f) == 0.f &&
               remap(0.f, 10.f, -1.f, 1.f, 10.f) == 1.f,
               "remap endpoints and midpoint test");
    ctx.expect(remap(0.f, 10.f, -1.f, 1.f, 15.f) == 2.f,
               "remap extrapolation test");
    ctx.expect(remap(10.f, 0.f, -1.f, 1.f, 7.5f) == -.5f &&
               remap(0.f, 10.f, 1.f, -1.f, 7.5f) == -.5f,
               "remap reversed range test");
    ctx.expect(remap(4.f, 4.f, 10.f, 20.f, 100.f) == 10.f,
               "remap zero-width input range test");
    ctx.expect(remap(-maximum, maximum, -maximum, maximum, 0.f) == 0.f,
               "remap extreme finite midpoint test");

    ctx.expect(step(2.f, 1.999f) == 0.f && step(2.f, 2.f) == 1.f &&
               step(2.f, 2.001f) == 1.f,
               "step threshold test");

    ctx.expect(smoothstep(0.f, 1.f, -1.f) == 0.f &&
               smoothstep(0.f, 1.f, 0.f) == 0.f &&
               smoothstep(0.f, 1.f, .5f) == .5f &&
               smoothstep(0.f, 1.f, 1.f) == 1.f &&
               smoothstep(0.f, 1.f, 2.f) == 1.f,
               "smoothstep clamping and landmarks test");
    ctx.expect(smootherstep(0.f, 1.f, -1.f) == 0.f &&
               smootherstep(0.f, 1.f, 0.f) == 0.f &&
               smootherstep(0.f, 1.f, .5f) == .5f &&
               smootherstep(0.f, 1.f, 1.f) == 1.f &&
               smootherstep(0.f, 1.f, 2.f) == 1.f,
               "smootherstep clamping and landmarks test");

    ctx.expect(smoothstep(1.f, 0.f, 1.f) == 0.f &&
               smoothstep(1.f, 0.f, .5f) == .5f &&
               smoothstep(1.f, 0.f, 0.f) == 1.f &&
               smootherstep(1.f, 0.f, .75f) < smootherstep(1.f, 0.f, .25f),
               "easing reversed range test");
    ctx.expect(smoothstep(1.f, 1.f, .99f) == 0.f &&
               smoothstep(1.f, 1.f, 1.f) == 1.f &&
               smootherstep(1.f, 1.f, 1.01f) == 1.f,
               "easing zero-width threshold test");

    bool symmetric{ true };
    bool monotonic{ true };
    bool reversed_monotonic{ true };
    float previous_smooth{ 0.f };
    float previous_smoother{ 0.f };
    float previous_reversed_smooth{ 1.f };
    float previous_reversed_smoother{ 1.f };
    for (int i{ 0 }; i <= 1000; ++i)
    {
        const float t{ static_cast<float>(i) / 1000.f };
        const float smooth{ smoothstep(0.f, 1.f, t) };
        const float smoother{ smootherstep(0.f, 1.f, t) };
        const float reversed_smooth{ smoothstep(1.f, 0.f, t) };
        const float reversed_smoother{ smootherstep(1.f, 0.f, t) };

        symmetric = symmetric && test_almost_equal(smooth + smoothstep(0.f, 1.f, 1.f - t), 1.f) &&
                    test_almost_equal(smoother + smootherstep(0.f, 1.f, 1.f - t), 1.f);
        monotonic = monotonic && smooth >= previous_smooth && smoother >= previous_smoother &&
                    smooth >= 0.f && smooth <= 1.f && smoother >= 0.f && smoother <= 1.f;
        reversed_monotonic = reversed_monotonic &&
                             reversed_smooth <= previous_reversed_smooth &&
                             reversed_smoother <= previous_reversed_smoother;

        previous_smooth = smooth;
        previous_smoother = smoother;
        previous_reversed_smooth = reversed_smooth;
        previous_reversed_smoother = reversed_smoother;
    }

    ctx.expect(symmetric, "easing midpoint symmetry sweep");
    ctx.expect(monotonic, "easing monotonicity and bounds sweep");
    ctx.expect(reversed_monotonic, "reversed easing monotonicity sweep");

    ctx.expect(move_towards(0.f, 10.f, 2.f) == 2.f &&
               move_towards(10.f, 0.f, 2.f) == 8.f,
               "move_towards bidirectional movement test");
    ctx.expect(move_towards(8.f, 10.f, 2.f) == 10.f &&
               move_towards(8.f, 10.f, 3.f) == 10.f &&
               move_towards(-8.f, -10.f, 3.f) == -10.f,
               "move_towards target clamping test");
    ctx.expect(move_towards(3.f, 10.f, 0.f) == 3.f &&
               move_towards(3.f, 10.f, -2.f) == 3.f,
               "move_towards non-positive delta test");
    ctx.expect(move_towards(-0.f, 0.f, 1.f) == 0.f &&
               chlm::signbit(move_towards(-0.f, 0.f, 1.f)),
               "move_towards equal value representation test");

    ctx.expect(move_towards(-maximum, maximum, maximum) == 0.f &&
               move_towards(0.f, maximum, maximum) == maximum &&
               move_towards(maximum, -maximum, maximum) == 0.f,
               "move_towards extreme finite range test");

    bool bounded_movement{ true };
    for (int i{ -1000 }; i <= 1000; ++i)
    {
        const float current{ static_cast<float>(i) * .25f };
        const float positive_target{ current + 10.f };
        const float negative_target{ current - 10.f };

        const float positive_result{ move_towards(current, positive_target, 1.25f) };
        const float negative_result{ move_towards(current, negative_target, 1.25f) };
        bounded_movement = bounded_movement &&
                           positive_result >= current && positive_result <= positive_target &&
                           negative_result <= current && negative_result >= negative_target;
    }
    ctx.expect(bounded_movement, "move_towards no-overshoot sweep");

    const float nan{ std::numeric_limits<float>::quiet_NaN() };
    ctx.expect(chlm::isnan(step(nan, 1.f)) && chlm::isnan(step(1.f, nan)),
               "step NaN propagation test");
    ctx.expect(chlm::isnan(move_towards(nan, 1.f, 1.f)) &&
               chlm::isnan(move_towards(1.f, nan, 1.f)) &&
               chlm::isnan(move_towards(1.f, 2.f, nan)),
               "move_towards NaN propagation test");

    ctx.expect(repeat(1.f, 5.f) == 1.f && repeat(6.f, 5.f) == 1.f &&
               repeat(-1.f, 5.f) == 4.f && repeat(-6.f, 5.f) == 4.f,
               "repeat positive and negative input test");
    ctx.expect(repeat(0.f, 5.f) == 0.f && repeat(5.f, 5.f) == 0.f &&
               repeat(-5.f, 5.f) == 0.f && !chlm::signbit(repeat(-5.f, 5.f)),
               "repeat exact boundary test");
    ctx.expect(repeat(3.f, 0.f) == 0.f && repeat(3.f, -2.f) == 0.f,
               "repeat non-positive length test");

    const float below_period{ std::nextafter(5.f, 0.f) };
    const float above_period{ std::nextafter(5.f, std::numeric_limits<float>::infinity()) };
    ctx.expect(repeat(below_period, 5.f) == below_period &&
               repeat(5.f, 5.f) == 0.f &&
               repeat(above_period, 5.f) == above_period - 5.f,
               "repeat adjacent period boundary test");

    ctx.expect(wrap(3.f, 2.f, 5.f) == 3.f && wrap(6.f, 2.f, 5.f) == 3.f &&
               wrap(1.f, 2.f, 5.f) == 4.f,
               "wrap explicit interval test");
    ctx.expect(wrap(6.f, 5.f, 2.f) == 3.f && wrap(1.f, 5.f, 2.f) == 4.f,
               "wrap reversed bounds test");
    ctx.expect(wrap(-100.f, 3.f, 3.f) == 3.f && wrap(100.f, 3.f, 3.f) == 3.f,
               "wrap zero-width interval test");
    ctx.expect(wrap(maximum, -maximum, 0.f) == -maximum &&
               wrap(-maximum, 0.f, maximum) == 0.f,
               "wrap extreme finite subtraction test");

    const float below_upper{ std::nextafter(5.f, 0.f) };
    const float above_upper{ std::nextafter(5.f, std::numeric_limits<float>::infinity()) };
    ctx.expect(wrap(below_upper, 2.f, 5.f) == below_upper &&
               wrap(5.f, 2.f, 5.f) == 2.f &&
               wrap(above_upper, 2.f, 5.f) > 2.f,
               "wrap adjacent upper boundary test");

    ctx.expect(delta_angle(0.f, 0.f) == 0.f &&
               delta_angle(0.f, two_pi) == 0.f &&
               delta_angle(0.f, -two_pi) == 0.f,
               "delta_angle equivalent revolution test");
    ctx.expect(delta_angle(0.f, pi) == pi && delta_angle(0.f, -pi) == pi,
               "delta_angle canonical half-turn test");
    ctx.expect(test_almost_equal(delta_angle(to_radians(350.f), to_radians(10.f)), to_radians(20.f)) &&
               test_almost_equal(delta_angle(to_radians(10.f), to_radians(350.f)), to_radians(-20.f)),
               "delta_angle shortest arc test");

    const float below_pi{ std::nextafter(pi, 0.f) };
    const float above_pi{ std::nextafter(pi, std::numeric_limits<float>::infinity()) };
    ctx.expect(delta_angle(0.f, below_pi) == below_pi &&
               delta_angle(0.f, pi) == pi && delta_angle(0.f, above_pi) < 0.f,
               "delta_angle adjacent half-turn boundary test");
    ctx.expect(chlm::isfinite(delta_angle(maximum, -maximum)) &&
               delta_angle(maximum, -maximum) > -pi &&
               delta_angle(maximum, -maximum) <= pi,
               "delta_angle extreme finite input test");

    ctx.expect(test_almost_equal(lerp_angle(to_radians(350.f), to_radians(10.f), .5f), two_pi) &&
               test_almost_equal(lerp_angle(to_radians(10.f), to_radians(350.f), .5f), 0.f),
               "lerp_angle wrap-crossing midpoint test");
    ctx.expect(test_almost_equal(lerp_angle(0.f, half_pi, 1.5f), half_pi * 1.5f),
               "lerp_angle extrapolation test");

    ctx.expect(ping_pong(0.f, 2.f) == 0.f && ping_pong(1.f, 2.f) == 1.f &&
               ping_pong(2.f, 2.f) == 2.f && ping_pong(3.f, 2.f) == 1.f &&
               ping_pong(4.f, 2.f) == 0.f,
               "ping_pong landmarks test");
    ctx.expect(ping_pong(-1.f, 2.f) == 1.f && ping_pong(-3.f, 2.f) == 1.f,
               "ping_pong negative input symmetry test");
    ctx.expect(ping_pong(1.f, 0.f) == 0.f && ping_pong(1.f, -2.f) == 0.f,
               "ping_pong non-positive length test");
    const float minimum_subnormal{ std::numeric_limits<float>::denorm_min() };
    ctx.expect(ping_pong(minimum_subnormal, 2.f) == minimum_subnormal &&
               ping_pong(-minimum_subnormal, 2.f) == minimum_subnormal &&
               ping_pong(minimum_subnormal, maximum) == minimum_subnormal,
               "ping_pong subnormal input preservation test");

    const float below_turn{ std::nextafter(2.f, 0.f) };
    const float above_turn{ std::nextafter(2.f, std::numeric_limits<float>::infinity()) };
    ctx.expect(ping_pong(below_turn, 2.f) <= 2.f && ping_pong(2.f, 2.f) == 2.f &&
               ping_pong(above_turn, 2.f) <= 2.f,
               "ping_pong adjacent turnaround test");

    const float infinity{ std::numeric_limits<float>::infinity() };
    ctx.expect(chlm::isnan(repeat(nan, 1.f)) && chlm::isnan(repeat(infinity, 1.f)) &&
               chlm::isnan(repeat(1.f, infinity)) && chlm::isnan(wrap(nan, 0.f, 1.f)) &&
               chlm::isnan(delta_angle(infinity, 0.f)) && chlm::isnan(ping_pong(infinity, 1.f)),
               "periodic helper non-finite input test");

    ctx.expect(exp_decay(10.f, 2.f, 2.f, 2.f) == 6.f &&
               exp_decay(10.f, 2.f, 2.f, 4.f) == 4.f,
               "exp_decay half-life landmark test");
    ctx.expect(exp_decay(10.f, 2.f, 2.f, 0.f) == 10.f &&
               exp_decay(10.f, 2.f, 2.f, -1.f) == 10.f,
               "exp_decay non-positive delta time test");
    ctx.expect(exp_decay(10.f, 2.f, 0.f, 1.f) == 10.f &&
               exp_decay(10.f, 2.f, -1.f, 1.f) == 10.f &&
               exp_decay(10.f, 2.f, infinity, 1.f) == 10.f,
               "exp_decay invalid half-life test");
    ctx.expect(exp_decay(10.f, 2.f, 2.f, infinity) == 2.f,
               "exp_decay infinite delta time test");
    ctx.expect(exp_decay(-maximum, maximum, 1.f, 1.f) == 0.f &&
               exp_decay(maximum, -maximum, 1.f, 1.f) == 0.f,
               "exp_decay extreme finite span test");

    bool decay_is_bounded{ true };
    float descending{ 100.f };
    float ascending{ -100.f };
    for (int i{ 0 }; i < 1000; ++i)
    {
        const float next_descending{ exp_decay(descending, -25.f, .75f, 1.f / 120.f) };
        const float next_ascending{ exp_decay(ascending, 25.f, .75f, 1.f / 120.f) };
        decay_is_bounded = decay_is_bounded &&
                           next_descending <= descending && next_descending >= -25.f &&
                           next_ascending >= ascending && next_ascending <= 25.f;
        descending = next_descending;
        ascending = next_ascending;
    }
    ctx.expect(decay_is_bounded, "exp_decay monotonic no-overshoot sweep");

    constexpr float half_lives[]{ .01f, .1f, 1.f, 10.f };
    constexpr float elapsed_times[]{ .001f, .016f, .1f, 1.f, 10.f };
    constexpr int subdivisions[]{ 2, 3, 10, 60, 240 };
    struct decay_range
    {
        float current;
        float target;
    };
    constexpr decay_range decay_ranges[]{
        { 3.f, -7.f },
        { -100.f, 25.f },
        { 0.f, 1.f },
        { 1e10f, -1e10f },
        { 1e-10f, -3e-10f }
    };
    float worst_composition_error{ 0.f };
    for (const decay_range range : decay_ranges)
    {
        const float span{ std::abs(range.current - range.target) };
        for (const float half_life : half_lives)
        {
            for (const float elapsed : elapsed_times)
            {
                const float single_step{ exp_decay(range.current, range.target, half_life, elapsed) };
                for (const int count : subdivisions)
                {
                    float split_step{ range.current };
                    const float step_time{ elapsed / static_cast<float>(count) };
                    for (int step_index{ 0 }; step_index < count; ++step_index)
                        split_step = exp_decay(split_step, range.target, half_life, step_time);

                    const float normalized_error{ std::abs(split_step - single_step) / span };
                    worst_composition_error = std::max(worst_composition_error, normalized_error);
                }
            }
        }
    }
    test_println("  exp_decay worst split-frame normalized error: {}", worst_composition_error);
    ctx.expect(worst_composition_error <= 2e-5f,
               "exp_decay frame-rate-independent composition sweep");

    ctx.expect(chlm::isnan(exp_decay(nan, 1.f, 1.f, 1.f)) &&
               chlm::isnan(exp_decay(1.f, nan, 1.f, 1.f)) &&
               chlm::isnan(exp_decay(1.f, 2.f, nan, 1.f)) &&
               chlm::isnan(exp_decay(1.f, 2.f, 1.f, nan)),
               "exp_decay NaN propagation test");
}
