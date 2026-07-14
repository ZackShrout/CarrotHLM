//
// Created by Zack Shrout on 7/14/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <bit>
#include <cstdint>
#include <limits>

void run_scalar_math_edge_case_tests(test_context& ctx)
{
    using namespace chlm;

    ctx.section("Scalar Math Edge Case Tests");

    const float nan{ std::numeric_limits<float>::quiet_NaN() };
    const float inf{ std::numeric_limits<float>::infinity() };
    const float negative_zero{ -0.f };

    ctx.expect(chlm::isnan(nan) && !chlm::isinf(nan) && !chlm::isfinite(nan),
               "NaN public classification test");
    ctx.expect(!chlm::isnan(inf) && chlm::isinf(inf) && !chlm::isfinite(inf),
               "infinity public classification test");
    ctx.expect(chlm::isfinite(negative_zero) && chlm::signbit(negative_zero),
               "negative zero public classification test");

    constexpr std::uint32_t nan_payload_bits{ 0x7FC12345u };
    const float nan_payload{ std::bit_cast<float>(nan_payload_bits) };
    const float negative_nan{ chlm::copysign(nan_payload, negative_zero) };
    const float positive_nan{ chlm::copysign(negative_nan, 0.f) };
    ctx.expect(std::bit_cast<std::uint32_t>(negative_nan) == (nan_payload_bits | 0x80000000u) &&
               std::bit_cast<std::uint32_t>(positive_nan) == nan_payload_bits,
               "copysign NaN payload preservation test");
    ctx.expect(std::bit_cast<std::uint32_t>(chlm::copysign(0.f, negative_zero)) == 0x80000000u &&
               std::bit_cast<std::uint32_t>(chlm::copysign(negative_zero, 0.f)) == 0u,
               "copysign signed zero test");

    ctx.expect(std::signbit(chlm::floor(negative_zero)) &&
               std::signbit(chlm::ceil(negative_zero)) &&
               std::signbit(chlm::trunc(negative_zero)) &&
               std::signbit(chlm::round(negative_zero)),
               "rounding signed zero preservation test");
    ctx.expect(chlm::floor(inf) == inf && chlm::ceil(inf) == inf &&
               chlm::trunc(inf) == inf && chlm::round(inf) == inf,
               "rounding infinity preservation test");
    ctx.expect(chlm::isnan(chlm::floor(nan)) && chlm::isnan(chlm::ceil(nan)) &&
               chlm::isnan(chlm::trunc(nan)) && chlm::isnan(chlm::round(nan)),
               "rounding NaN propagation test");
    ctx.expect(chlm::isnan(chlm::fmod(1.f, 0.f)) && chlm::isnan(chlm::fmod(inf, 1.f)),
               "fmod invalid input test");
    ctx.expect(chlm::fmod(1.f, inf) == 1.f &&
               std::signbit(chlm::fmod(-4.f, 2.f)),
               "fmod infinity and signed zero test");
    ctx.expect(chlm::isnan(chlm::frac(inf)) && chlm::isnan(chlm::frac(nan)),
               "frac non-finite input test");
    ctx.expect(chlm::frac(std::numeric_limits<float>::denorm_min()) ==
               std::numeric_limits<float>::denorm_min(),
               "frac subnormal preservation test");

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
