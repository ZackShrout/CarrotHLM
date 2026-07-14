//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

void run_scalar_math_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Scalar Math Tests");

    ctx.expect(test_almost_equal(chlm::sqrt(25.f), 5.f), "sqrt basic value test");
    ctx.expect(test_almost_equal(sqrt_precise(25.f), 5.f), "sqrt_precise basic value test");

    ctx.expect(test_almost_equal(chlm::sin(pi * 0.5f), 1.f, GENERAL_EPS), "sin basic value test");
    ctx.expect(test_almost_equal(sin_precise(pi * 0.5f), 1.f, GENERAL_EPS), "sin_precise basic value test");

    ctx.expect(test_almost_equal(chlm::cos(0.f), 1.f, GENERAL_EPS), "cos basic value test");
    ctx.expect(test_almost_equal(cos_precise(0.f), 1.f, GENERAL_EPS), "cos_precise basic value test");

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

    constexpr float acos_symmetry_input{ 0.7654321f };
    ctx.expect(test_almost_equal(chlm::acos(-acos_symmetry_input),
                                 pi - chlm::acos(acos_symmetry_input), 2e-6f),
               "acos symmetry test");
    ctx.expect(test_almost_equal(acos_precise(-acos_symmetry_input),
                                 pi - acos_precise(acos_symmetry_input), 2e-7f),
               "acos_precise symmetry test");
}
