//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

#include <chlm/detail/simd/SimdSelect.h>

void run_backend_selection_tests(test_context& ctx)
{
    using namespace chlm::detail;

    ctx.section("Backend Selection Tests");

    test_println("  active backend = {}", to_string(active_simd_backend));

    ctx.expect(active_simd::lane_count == 4, "Active backend lane-count test");
    ctx.expect(scalar_simd::lane_count == 4, "Scalar backend lane-count test");

    const auto active_sum = active_simd::add(
        active_simd::set(1.f, 2.f, 3.f, 4.f),
        active_simd::splat(2.f)
    );
    ctx.expect(
        test_almost_equal(active_simd::extract_lane<0>(active_sum), 3.f) &&
        test_almost_equal(active_simd::extract_lane<1>(active_sum), 4.f) &&
        test_almost_equal(active_simd::extract_lane<2>(active_sum), 5.f) &&
        test_almost_equal(active_simd::extract_lane<3>(active_sum), 6.f),
        "Active backend add test"
    );

    const auto scalar_product = scalar_simd::mul(
        scalar_simd::set(1.f, 2.f, 3.f, 4.f),
        scalar_simd::set(5.f, 6.f, 7.f, 8.f)
    );
    ctx.expect(
        test_almost_equal(scalar_simd::extract_lane<0>(scalar_product), 5.f) &&
        test_almost_equal(scalar_simd::extract_lane<1>(scalar_product), 12.f) &&
        test_almost_equal(scalar_simd::extract_lane<2>(scalar_product), 21.f) &&
        test_almost_equal(scalar_simd::extract_lane<3>(scalar_product), 32.f),
        "Scalar backend multiply test"
    );

    const auto a = active_simd::set(1.f, 8.f, 3.f, 6.f);
    const auto b = active_simd::set(2.f, 4.f, 5.f, 6.f);
    const auto min_value = active_simd::min(a, b);
    const auto max_value = active_simd::max(a, b);
    const auto eq_mask = active_simd::cmp_eq(a, b);
    const auto gt_mask = active_simd::cmp_gt(a, b);
    const auto selected = active_simd::select(gt_mask, a, b);
    const auto shuffled = active_simd::template shuffle<2, 0, 3, 1>(a);

    ctx.expect(
        test_almost_equal(active_simd::extract_lane<0>(min_value), 1.f) &&
        test_almost_equal(active_simd::extract_lane<1>(min_value), 4.f) &&
        test_almost_equal(active_simd::extract_lane<2>(min_value), 3.f) &&
        test_almost_equal(active_simd::extract_lane<3>(min_value), 6.f) &&
        test_almost_equal(active_simd::extract_lane<0>(max_value), 2.f) &&
        test_almost_equal(active_simd::extract_lane<1>(max_value), 8.f) &&
        test_almost_equal(active_simd::extract_lane<2>(max_value), 5.f) &&
        test_almost_equal(active_simd::extract_lane<3>(max_value), 6.f),
        "Active backend min/max test"
    );

    ctx.expect(
        test_almost_equal(active_simd::extract_lane<0>(selected), 2.f) &&
        test_almost_equal(active_simd::extract_lane<1>(selected), 8.f) &&
        test_almost_equal(active_simd::extract_lane<2>(selected), 5.f) &&
        test_almost_equal(active_simd::extract_lane<3>(selected), 6.f),
        "Active backend compare/select test"
    );

    ctx.expect(
        active_simd::extract_lane<0>(eq_mask) == 0.f &&
        active_simd::extract_lane<1>(eq_mask) == 0.f &&
        active_simd::extract_lane<2>(eq_mask) == 0.f &&
        active_simd::extract_lane<3>(eq_mask) != 0.f,
        "Active backend equality mask test"
    );

    ctx.expect(
        test_almost_equal(active_simd::extract_lane<0>(shuffled), 3.f) &&
        test_almost_equal(active_simd::extract_lane<1>(shuffled), 1.f) &&
        test_almost_equal(active_simd::extract_lane<2>(shuffled), 6.f) &&
        test_almost_equal(active_simd::extract_lane<3>(shuffled), 8.f),
        "Active backend shuffle test"
    );
}
