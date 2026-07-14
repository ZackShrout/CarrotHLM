//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <limits>

void run_scalar_math_edge_case_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Scalar Math Edge Case Tests");

    const float nan{ std::numeric_limits<float>::quiet_NaN() };
    const float inf{ std::numeric_limits<float>::infinity() };
    const float negative_zero{ -0.f };

    ctx.expect(std::isnan(chlm::sqrt(-1.f)), "sqrt negative domain test");
    ctx.expect(std::isnan(sqrt_precise(-1.f)), "sqrt_precise negative domain test");
    ctx.expect(std::isnan(chlm::sqrt(-inf)), "sqrt negative infinity test");
    ctx.expect(std::isnan(sqrt_precise(-inf)), "sqrt_precise negative infinity test");
    ctx.expect(chlm::sqrt(inf) == inf, "sqrt positive infinity test");
    ctx.expect(sqrt_precise(inf) == inf, "sqrt_precise positive infinity test");
    ctx.expect(std::isnan(chlm::sqrt(nan)), "sqrt NaN propagation test");
    ctx.expect(std::isnan(sqrt_precise(nan)), "sqrt_precise NaN propagation test");
    ctx.expect(std::signbit(chlm::sqrt(negative_zero)), "sqrt signed zero test");
    ctx.expect(std::signbit(sqrt_precise(negative_zero)), "sqrt_precise signed zero test");

    ctx.expect(std::isnan(chlm::acos(1.0001f)), "acos out-of-range domain test");
    ctx.expect(std::isnan(acos_precise(1.0001f)), "acos_precise out-of-range domain test");
    ctx.expect(std::isnan(chlm::acos(-1.0001f)), "acos negative out-of-range domain test");
    ctx.expect(std::isnan(acos_precise(-1.0001f)),
               "acos_precise negative out-of-range domain test");
    ctx.expect(std::isnan(chlm::acos(nan)), "acos NaN propagation test");
    ctx.expect(std::isnan(acos_precise(nan)), "acos_precise NaN propagation test");

    ctx.expect(std::isnan(chlm::sin(nan)), "sin NaN propagation test");
    ctx.expect(std::isnan(sin_precise(nan)), "sin_precise NaN propagation test");
    ctx.expect(std::isnan(chlm::cos(nan)), "cos NaN propagation test");
    ctx.expect(std::isnan(cos_precise(nan)), "cos_precise NaN propagation test");
    ctx.expect(std::isnan(chlm::tan(nan)), "tan NaN propagation test");
    ctx.expect(std::isnan(tan_precise(nan)), "tan_precise NaN propagation test");

    ctx.expect(std::isnan(chlm::sin(inf)), "sin infinity input test");
    ctx.expect(std::isnan(sin_precise(inf)), "sin_precise infinity input test");
    ctx.expect(std::isnan(chlm::cos(inf)), "cos infinity input test");
    ctx.expect(std::isnan(cos_precise(inf)), "cos_precise infinity input test");
    ctx.expect(std::isnan(chlm::tan(inf)), "tan infinity input test");
    ctx.expect(std::isnan(tan_precise(inf)), "tan_precise infinity input test");
    ctx.expect(std::signbit(chlm::sin(negative_zero)), "sin signed zero test");
    ctx.expect(std::signbit(sin_precise(negative_zero)), "sin_precise signed zero test");
    ctx.expect(chlm::cos(negative_zero) == 1.f, "cos negative zero test");
    ctx.expect(cos_precise(negative_zero) == 1.f, "cos_precise negative zero test");
    ctx.expect(std::signbit(chlm::tan(negative_zero)), "tan signed zero test");
    ctx.expect(std::signbit(tan_precise(negative_zero)), "tan_precise signed zero test");
}
