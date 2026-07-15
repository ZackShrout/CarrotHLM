//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#include "TestHarness.h"

int main()
{
    test_context ctx{};

    test_println("=== CarrotHLM Validation Test ===\n");

    run_vector_tests(ctx);
    run_float_vector_exhaustive_tests(ctx);
    run_int_vector_tests(ctx);
    run_uint_vector_tests(ctx);
    run_vector_scaffolding_tests(ctx);
    run_swizzle_scaffolding_tests(ctx);
    run_quaternion_tests(ctx);
    run_matrix_tests(ctx);
    run_backend_selection_tests(ctx);
    run_scalar_math_tests(ctx);
    run_scalar_math_edge_case_tests(ctx);
    run_scalar_math_exhaustive_tests(ctx);
    run_utility_tests(ctx);

    if (ctx.failures == 0)
    {
        test_println("\n=== All tests passed! CarrotHLM is ready. ===");
        return 0;
    }

    test_println("\n=== {} validation test(s) failed. ===", ctx.failures);
    return 1;
}
