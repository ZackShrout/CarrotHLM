//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <bit>
#include <cstdint>

static_assert(chlm::isfinite(1.f));
static_assert(!chlm::isfinite(std::bit_cast<float>(0x7F800000u)));
static_assert(chlm::signbit(-0.f));
static_assert(std::bit_cast<std::uint32_t>(chlm::copysign(1.f, -0.f)) == 0xBF800000u);
static_assert(chlm::floor(-1.25f) == -2.f);
static_assert(chlm::ceil(-1.25f) == -1.f);
static_assert(chlm::trunc(-1.25f) == -1.f);
static_assert(chlm::round(-1.5f) == -2.f);
static_assert(chlm::fmod(7.5f, 2.f) == 1.5f);
static_assert(chlm::frac(-1.25f) == .75f);
static_assert(sizeof(chlm::sin_cos_result) == sizeof(float) * 2u);

void run_scalar_math_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Scalar Math Tests");

    ctx.expect(chlm::isfinite(1.f) && !chlm::isnan(1.f) && !chlm::isinf(1.f),
               "finite classification basic test");
    ctx.expect(chlm::signbit(-1.f) && !chlm::signbit(1.f), "signbit basic test");
    ctx.expect(chlm::copysign(3.5f, -1.f) == -3.5f && chlm::copysign(-3.5f, 1.f) == 3.5f,
               "copysign basic test");

    ctx.expect(chlm::floor(3.75f) == 3.f && chlm::floor(-3.25f) == -4.f,
               "floor basic test");
    ctx.expect(chlm::ceil(3.25f) == 4.f && chlm::ceil(-3.75f) == -3.f,
               "ceil basic test");
    ctx.expect(chlm::trunc(3.75f) == 3.f && chlm::trunc(-3.75f) == -3.f,
               "trunc basic test");
    ctx.expect(chlm::round(3.5f) == 4.f && chlm::round(-3.5f) == -4.f &&
               chlm::round(3.49f) == 3.f && chlm::round(-3.49f) == -3.f,
               "round halfway-away basic test");
    ctx.expect(chlm::fmod(7.5f, 2.f) == 1.5f && chlm::fmod(-7.5f, 2.f) == -1.5f,
               "fmod basic test");
    ctx.expect(chlm::frac(1.25f) == .25f && chlm::frac(-1.25f) == .75f,
               "frac basic test");

    ctx.expect(test_almost_equal(chlm::sqrt(25.f), 5.f), "sqrt basic value test");
    ctx.expect(test_almost_equal(sqrt_precise(25.f), 5.f), "sqrt_precise basic value test");
    ctx.expect(test_almost_equal(chlm::rsqrt(25.f), .2f, 5e-6f), "rsqrt basic value test");
    ctx.expect(test_almost_equal(rsqrt_precise(25.f), .2f, 2e-7f),
               "rsqrt_precise basic value test");

    ctx.expect(test_almost_equal(chlm::sin(pi * 0.5f), 1.f, GENERAL_EPS), "sin basic value test");
    ctx.expect(test_almost_equal(sin_precise(pi * 0.5f), 1.f, GENERAL_EPS), "sin_precise basic value test");

    ctx.expect(test_almost_equal(chlm::cos(0.f), 1.f, GENERAL_EPS), "cos basic value test");
    ctx.expect(test_almost_equal(cos_precise(0.f), 1.f, GENERAL_EPS), "cos_precise basic value test");

    const sin_cos_result pair{ chlm::sin_cos(pi * .25f) };
    const sin_cos_result precise_pair{ sin_cos_precise(pi * .25f) };
    ctx.expect(test_almost_equal(pair.sine, chlm::sin(pi * .25f), 5e-6f) &&
               test_almost_equal(pair.cosine, chlm::cos(pi * .25f), 5e-6f),
               "sin_cos basic value test");
    ctx.expect(test_almost_equal(precise_pair.sine, sin_precise(pi * .25f), 2e-7f) &&
               test_almost_equal(precise_pair.cosine, cos_precise(pi * .25f), 2e-7f),
               "sin_cos_precise basic value test");

    constexpr float symmetry_input{ 1.2345f };
    ctx.expect(test_almost_equal(chlm::sin(-symmetry_input), -chlm::sin(symmetry_input), 5e-6f),
               "sin odd symmetry test");
    ctx.expect(test_almost_equal(chlm::cos(-symmetry_input), chlm::cos(symmetry_input), 5e-6f),
               "cos even symmetry test");
    const float sine{ chlm::sin(symmetry_input) };
    const float cosine{ chlm::cos(symmetry_input) };
    ctx.expect(test_almost_equal(sine * sine + cosine * cosine, 1.f, 1e-5f),
               "sin/cos identity test");

    ctx.expect(test_almost_equal(chlm::tan(0.f), 0.f, GENERAL_EPS), "tan basic value test");
    ctx.expect(test_almost_equal(tan_precise(0.f), 0.f, GENERAL_EPS), "tan_precise basic value test");

    ctx.expect(test_almost_equal(chlm::acos(1.f), 0.f, GENERAL_EPS), "acos basic value test");
    ctx.expect(test_almost_equal(acos_precise(1.f), 0.f, GENERAL_EPS), "acos_precise basic value test");
    ctx.expect(test_almost_equal(chlm::acos(0.f), half_pi, 2e-6f), "acos midpoint test");
    ctx.expect(test_almost_equal(acos_precise(0.f), half_pi, 2e-7f),
               "acos_precise midpoint test");
    ctx.expect(chlm::acos(-1.f) == pi, "acos negative endpoint test");
    ctx.expect(acos_precise(-1.f) == pi, "acos_precise negative endpoint test");

    ctx.expect(test_almost_equal(chlm::asin(1.f), half_pi, GENERAL_EPS),
               "asin positive endpoint test");
    ctx.expect(test_almost_equal(asin_precise(-1.f), -half_pi, 2e-7f),
               "asin_precise negative endpoint test");
    ctx.expect(test_almost_equal(chlm::atan(1.f), pi * .25f, 6e-6f),
               "atan basic value test");
    ctx.expect(test_almost_equal(atan_precise(-1.f), -pi * .25f, 2e-7f),
               "atan_precise basic value test");
    ctx.expect(test_almost_equal(chlm::atan2(1.f, -1.f), pi * .75f, 6e-6f),
               "atan2 quadrant test");
    ctx.expect(test_almost_equal(atan2_precise(-1.f, -1.f), -pi * .75f, 2e-7f),
               "atan2_precise quadrant test");

    constexpr float acos_symmetry_input{ 0.7654321f };
    ctx.expect(test_almost_equal(chlm::acos(-acos_symmetry_input),
                                 pi - chlm::acos(acos_symmetry_input), 2e-6f),
               "acos symmetry test");
    ctx.expect(test_almost_equal(acos_precise(-acos_symmetry_input),
                                 pi - acos_precise(acos_symmetry_input), 2e-7f),
               "acos_precise symmetry test");

    ctx.expect(test_almost_equal(chlm::asin(-acos_symmetry_input),
                                 -chlm::asin(acos_symmetry_input), 2e-6f),
               "asin odd symmetry test");
    ctx.expect(test_almost_equal(chlm::atan(-symmetry_input),
                                 -chlm::atan(symmetry_input), 2e-6f),
               "atan odd symmetry test");
}
