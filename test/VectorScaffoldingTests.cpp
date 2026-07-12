//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <chlm/detail/VectorBase.h>

void run_vector_scaffolding_tests(test_context& ctx)
{
    using namespace chlm::detail;

    ctx.section("Vector Scaffolding Tests");

    const float_storage lanes{ 1.f, 2.f, 3.f, 4.f };
    ctx.expect(
        test_almost_equal(lanes.lane<0>(), 1.f) &&
        test_almost_equal(lanes.lane<1>(), 2.f) &&
        test_almost_equal(lanes.lane<2>(), 3.f) &&
        test_almost_equal(lanes.lane<3>(), 4.f),
        "Float storage lane extraction test"
    );

    const auto replaced = lanes.with_lane<2>(9.f);
    ctx.expect(
        test_almost_equal(replaced.lane<0>(), 1.f) &&
        test_almost_equal(replaced.lane<1>(), 2.f) &&
        test_almost_equal(replaced.lane<2>(), 9.f) &&
        test_almost_equal(replaced.lane<3>(), 4.f),
        "Float storage lane replacement test"
    );

    const auto sum = float_storage{ 1.f, 2.f, 3.f, 4.f } + float_storage::splat(2.f);
    ctx.expect(
        test_almost_equal(sum.lane<0>(), 3.f) &&
        test_almost_equal(sum.lane<1>(), 4.f) &&
        test_almost_equal(sum.lane<2>(), 5.f) &&
        test_almost_equal(sum.lane<3>(), 6.f),
        "Float storage arithmetic test"
    );

    const float_storage lhs{ 1.f, 8.f, 3.f, 6.f };
    const float_storage rhs{ 2.f, 4.f, 5.f, 6.f };
    const auto min_value = float_storage::min(lhs, rhs);
    const auto max_value = float_storage::max(lhs, rhs);
    const auto gt_mask = float_storage::cmp_gt(lhs, rhs);
    const auto eq_mask = float_storage::cmp_eq(lhs, rhs);
    const auto neq_mask = float_storage::cmp_neq(lhs, rhs);
    const auto lt_mask = float_storage::cmp_lt(lhs, rhs);
    const auto lte_mask = float_storage::cmp_lte(lhs, rhs);
    const auto gte_mask = float_storage::cmp_gte(lhs, rhs);
    const float_storage gt_mask_value{ gt_mask };
    const float_storage eq_mask_value{ eq_mask };
    const float_storage neq_mask_value{ neq_mask };
    const float_storage lt_mask_value{ lt_mask };
    const float_storage lte_mask_value{ lte_mask };
    const float_storage gte_mask_value{ gte_mask };
    const auto selected = float_storage::select(gt_mask, lhs, rhs);
    const auto shuffled = float_storage::shuffle<2, 0, 3, 1>(lhs);

    ctx.expect(
        test_almost_equal(min_value.lane<0>(), 1.f) &&
        test_almost_equal(min_value.lane<1>(), 4.f) &&
        test_almost_equal(min_value.lane<2>(), 3.f) &&
        test_almost_equal(min_value.lane<3>(), 6.f) &&
        test_almost_equal(max_value.lane<0>(), 2.f) &&
        test_almost_equal(max_value.lane<1>(), 8.f) &&
        test_almost_equal(max_value.lane<2>(), 5.f) &&
        test_almost_equal(max_value.lane<3>(), 6.f),
        "Float storage min/max test"
    );

    ctx.expect(
        test_almost_equal(selected.lane<0>(), 2.f) &&
        test_almost_equal(selected.lane<1>(), 8.f) &&
        test_almost_equal(selected.lane<2>(), 5.f) &&
        test_almost_equal(selected.lane<3>(), 6.f),
        "Float storage compare/select test"
    );

    const auto zero = float_storage::bit_xor(lhs, lhs);
    const auto masked_high = float_storage::bit_and(gt_mask_value, lhs);
    const auto masked_low = float_storage::bit_and(lte_mask_value, lhs);
    const auto recombined = float_storage::bit_or(masked_high, masked_low);
    const auto removed_high = float_storage::bit_xor(lhs, masked_high);

    ctx.expect(
        test_almost_equal(zero.lane<0>(), 0.f) &&
        test_almost_equal(zero.lane<1>(), 0.f) &&
        test_almost_equal(zero.lane<2>(), 0.f) &&
        test_almost_equal(zero.lane<3>(), 0.f) &&
        test_almost_equal(masked_high.lane<0>(), 0.f) &&
        test_almost_equal(masked_high.lane<1>(), 8.f) &&
        test_almost_equal(masked_high.lane<2>(), 0.f) &&
        test_almost_equal(masked_high.lane<3>(), 0.f) &&
        test_almost_equal(masked_low.lane<0>(), 1.f) &&
        test_almost_equal(masked_low.lane<1>(), 0.f) &&
        test_almost_equal(masked_low.lane<2>(), 3.f) &&
        test_almost_equal(masked_low.lane<3>(), 6.f) &&
        test_almost_equal(recombined.lane<0>(), 1.f) &&
        test_almost_equal(recombined.lane<1>(), 8.f) &&
        test_almost_equal(recombined.lane<2>(), 3.f) &&
        test_almost_equal(recombined.lane<3>(), 6.f) &&
        test_almost_equal(removed_high.lane<0>(), 1.f) &&
        test_almost_equal(removed_high.lane<1>(), 0.f) &&
        test_almost_equal(removed_high.lane<2>(), 3.f) &&
        test_almost_equal(removed_high.lane<3>(), 6.f),
        "Float storage bitwise test"
    );

    ctx.expect(
        eq_mask_value.lane<0>() == 0.f &&
        eq_mask_value.lane<1>() == 0.f &&
        eq_mask_value.lane<2>() == 0.f &&
        eq_mask_value.lane<3>() != 0.f &&
        neq_mask_value.lane<0>() != 0.f &&
        neq_mask_value.lane<1>() != 0.f &&
        neq_mask_value.lane<2>() != 0.f &&
        neq_mask_value.lane<3>() == 0.f &&
        lt_mask_value.lane<0>() != 0.f &&
        lt_mask_value.lane<1>() == 0.f &&
        lt_mask_value.lane<2>() != 0.f &&
        lt_mask_value.lane<3>() == 0.f &&
        lte_mask_value.lane<0>() != 0.f &&
        lte_mask_value.lane<1>() == 0.f &&
        lte_mask_value.lane<2>() != 0.f &&
        lte_mask_value.lane<3>() != 0.f &&
        gte_mask_value.lane<0>() == 0.f &&
        gte_mask_value.lane<1>() != 0.f &&
        gte_mask_value.lane<2>() == 0.f &&
        gte_mask_value.lane<3>() != 0.f &&
        test_almost_equal(shuffled.lane<0>(), 3.f) &&
        test_almost_equal(shuffled.lane<1>(), 1.f) &&
        test_almost_equal(shuffled.lane<2>(), 6.f) &&
        test_almost_equal(shuffled.lane<3>(), 8.f),
        "Float storage compare family and shuffle test"
    );

    const float3_base a{ 1.f, 2.f, 3.f };
    float3_base b{ 4.f, 5.f, 6.f };
    b.set_lane<1>(8.f);
    const auto c = a + b;
    ctx.expect(
        test_almost_equal(c.lane<0>(), 5.f) &&
        test_almost_equal(c.lane<1>(), 10.f) &&
        test_almost_equal(c.lane<2>(), 9.f),
        "Float3 base arithmetic test"
    );

    const float4_base d{ 1.f, 2.f, 3.f, 4.f };
    ctx.expect(
        test_almost_equal(d.lane<0>(), 1.f) &&
        test_almost_equal(d.lane<1>(), 2.f) &&
        test_almost_equal(d.lane<2>(), 3.f) &&
        test_almost_equal(d.lane<3>(), 4.f),
        "Float4 base construction test"
    );
}
