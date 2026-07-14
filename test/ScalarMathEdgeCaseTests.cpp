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

    ctx.expect(chlm::rsqrt(0.f) == inf && !std::signbit(chlm::rsqrt(0.f)) &&
               chlm::rsqrt(negative_zero) == -inf && std::signbit(chlm::rsqrt(negative_zero)),
               "rsqrt signed zero test");
    ctx.expect(rsqrt_precise(0.f) == inf && rsqrt_precise(negative_zero) == -inf,
               "rsqrt_precise signed zero test");
    ctx.expect(chlm::rsqrt(inf) == 0.f && !std::signbit(chlm::rsqrt(inf)) &&
               rsqrt_precise(inf) == 0.f,
               "rsqrt positive infinity test");
    ctx.expect(std::isnan(chlm::rsqrt(-1.f)) && std::isnan(chlm::rsqrt(-inf)) &&
               std::isnan(rsqrt_precise(-1.f)) && std::isnan(rsqrt_precise(-inf)),
               "rsqrt negative domain test");
    ctx.expect(std::isnan(chlm::rsqrt(nan)) && std::isnan(rsqrt_precise(nan)),
               "rsqrt NaN propagation test");
    ctx.expect(std::isfinite(chlm::rsqrt(std::numeric_limits<float>::denorm_min())) &&
               std::isfinite(rsqrt_precise(std::numeric_limits<float>::denorm_min())),
               "rsqrt subnormal input test");

    ctx.expect(std::isnan(chlm::acos(1.0001f)), "acos out-of-range domain test");
    ctx.expect(std::isnan(acos_precise(1.0001f)), "acos_precise out-of-range domain test");
    ctx.expect(std::isnan(chlm::acos(-1.0001f)), "acos negative out-of-range domain test");
    ctx.expect(std::isnan(acos_precise(-1.0001f)),
               "acos_precise negative out-of-range domain test");
    ctx.expect(std::isnan(chlm::acos(nan)), "acos NaN propagation test");
    ctx.expect(std::isnan(acos_precise(nan)), "acos_precise NaN propagation test");

    ctx.expect(std::isnan(chlm::asin(1.0001f)) && std::isnan(chlm::asin(-1.0001f)),
               "asin out-of-range domain test");
    ctx.expect(std::isnan(asin_precise(1.0001f)) && std::isnan(asin_precise(-1.0001f)),
               "asin_precise out-of-range domain test");
    ctx.expect(std::isnan(chlm::asin(nan)) && std::isnan(asin_precise(nan)),
               "asin NaN propagation test");
    ctx.expect(std::signbit(chlm::asin(negative_zero)) &&
               std::signbit(asin_precise(negative_zero)),
               "asin signed zero test");

    ctx.expect(std::isnan(chlm::atan(nan)) && std::isnan(atan_precise(nan)),
               "atan NaN propagation test");
    ctx.expect(std::signbit(chlm::atan(negative_zero)) &&
               std::signbit(atan_precise(negative_zero)),
               "atan signed zero test");
    ctx.expect(chlm::atan(inf) == half_pi && chlm::atan(-inf) == -half_pi &&
               atan_precise(inf) == half_pi && atan_precise(-inf) == -half_pi,
               "atan infinity limit test");

    ctx.expect(chlm::atan2(0.f, 1.f) == 0.f &&
               std::signbit(chlm::atan2(negative_zero, 1.f)) &&
               chlm::atan2(0.f, -1.f) == pi &&
               chlm::atan2(negative_zero, -1.f) == -pi,
               "atan2 signed-zero axis test");
    ctx.expect(chlm::atan2(0.f, 0.f) == 0.f &&
               std::signbit(chlm::atan2(negative_zero, 0.f)) &&
               chlm::atan2(0.f, negative_zero) == pi &&
               chlm::atan2(negative_zero, negative_zero) == -pi,
               "atan2 signed-zero pair test");
    ctx.expect(chlm::atan2(1.f, 0.f) == half_pi &&
               chlm::atan2(-1.f, 0.f) == -half_pi,
               "atan2 zero-x axis test");
    ctx.expect(chlm::atan2(inf, inf) == pi * .25f &&
               chlm::atan2(inf, -inf) == pi * .75f &&
               chlm::atan2(-inf, inf) == -pi * .25f &&
               chlm::atan2(-inf, -inf) == -pi * .75f,
               "atan2 infinite quadrant test");
    ctx.expect(chlm::atan2(1.f, inf) == 0.f &&
               std::signbit(chlm::atan2(-1.f, inf)) &&
               chlm::atan2(1.f, -inf) == pi &&
               chlm::atan2(-1.f, -inf) == -pi &&
               chlm::atan2(inf, 1.f) == half_pi &&
               chlm::atan2(-inf, 1.f) == -half_pi,
               "atan2 single-infinity test");
    ctx.expect(atan2_precise(0.f, 0.f) == 0.f &&
               std::signbit(atan2_precise(negative_zero, 0.f)) &&
               atan2_precise(0.f, negative_zero) == pi &&
               atan2_precise(negative_zero, negative_zero) == -pi &&
               atan2_precise(1.f, 0.f) == half_pi &&
               atan2_precise(-1.f, 0.f) == -half_pi,
               "atan2_precise signed-zero and axis test");
    ctx.expect(atan2_precise(inf, inf) == pi * .25f &&
               atan2_precise(inf, -inf) == pi * .75f &&
               atan2_precise(-inf, inf) == -pi * .25f &&
               atan2_precise(-inf, -inf) == -pi * .75f &&
               atan2_precise(1.f, inf) == 0.f &&
               std::signbit(atan2_precise(-1.f, inf)) &&
               atan2_precise(1.f, -inf) == pi &&
               atan2_precise(-1.f, -inf) == -pi &&
               atan2_precise(inf, 1.f) == half_pi &&
               atan2_precise(-inf, 1.f) == -half_pi,
               "atan2_precise infinity matrix test");
    ctx.expect(std::isnan(chlm::atan2(nan, 1.f)) &&
               std::isnan(chlm::atan2(1.f, nan)) &&
               std::isnan(atan2_precise(nan, 1.f)) &&
               std::isnan(atan2_precise(1.f, nan)),
               "atan2 NaN propagation test");

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

    const sin_cos_result zero_pair{ chlm::sin_cos(negative_zero) };
    const sin_cos_result precise_zero_pair{ sin_cos_precise(negative_zero) };
    ctx.expect(std::signbit(zero_pair.sine) && zero_pair.cosine == 1.f &&
               std::signbit(precise_zero_pair.sine) && precise_zero_pair.cosine == 1.f,
               "sin_cos signed zero test");
    const sin_cos_result nan_pair{ chlm::sin_cos(nan) };
    const sin_cos_result precise_nan_pair{ sin_cos_precise(nan) };
    ctx.expect(std::isnan(nan_pair.sine) && std::isnan(nan_pair.cosine) &&
               std::isnan(precise_nan_pair.sine) && std::isnan(precise_nan_pair.cosine),
               "sin_cos NaN propagation test");
    const sin_cos_result infinity_pair{ chlm::sin_cos(inf) };
    const sin_cos_result precise_infinity_pair{ sin_cos_precise(inf) };
    ctx.expect(std::isnan(infinity_pair.sine) && std::isnan(infinity_pair.cosine) &&
               std::isnan(precise_infinity_pair.sine) && std::isnan(precise_infinity_pair.cosine),
               "sin_cos infinity input test");
}
