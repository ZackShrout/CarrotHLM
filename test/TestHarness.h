//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

#include <chlm/CarrotHLM.h>

#include <cmath>
#include <string_view>

#if __has_include(<print>)
    #include <print>
    #define test_println(...) std::println(__VA_ARGS__)
#else
    #include <iostream>
    #include <utility>

    inline void test_print(const std::string_view format)
    {
        std::cout << format;
    }

    template<typename T, typename... Args>
    inline void test_print(const std::string_view format, T&& value, Args&&... args)
    {
        const std::size_t placeholder{ format.find(static_cast<char>(123)) };
        if (placeholder == std::string_view::npos)
        {
            std::cout << format;
            return;
        }

        std::cout << format.substr(0, placeholder) << std::forward<T>(value);
        test_print(format.substr(placeholder + 2), std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline void test_println(const std::string_view format, Args&&... args)
    {
        test_print(format, std::forward<Args>(args)...);
        std::cout << std::endl;
    }
#endif

constexpr float GENERAL_EPS = 1e-4f;

inline bool test_almost_equal(float a, float b, float eps = 1e-6f)
{
    return std::abs(a - b) <= eps;
}

inline bool test_almost_equal(const chlm::float2& a, const chlm::float2& b, float eps = GENERAL_EPS)
{
    return test_almost_equal(a.x, b.x, eps) &&
           test_almost_equal(a.y, b.y, eps);
}

inline bool test_almost_equal(const chlm::float3& a, const chlm::float3& b, float eps = GENERAL_EPS)
{
    return test_almost_equal(a.x, b.x, eps) &&
           test_almost_equal(a.y, b.y, eps) &&
           test_almost_equal(a.z, b.z, eps);
}

inline bool test_almost_equal(const chlm::float4& a, const chlm::float4& b, float eps = GENERAL_EPS)
{
    return test_almost_equal(a.x, b.x, eps) &&
           test_almost_equal(a.y, b.y, eps) &&
           test_almost_equal(a.z, b.z, eps) &&
           test_almost_equal(a.w, b.w, eps);
}

inline bool test_equal(const chlm::uint2& a, const chlm::uint2& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool test_equal(const chlm::uint3& a, const chlm::uint3& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool test_equal(const chlm::uint4& a, const chlm::uint4& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline bool test_equal(const chlm::int2& a, const chlm::int2& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool test_equal(const chlm::int3& a, const chlm::int3& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool test_equal(const chlm::int4& a, const chlm::int4& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline bool test_almost_equal(const chlm::float4x4& a, const chlm::float4x4& b)
{
    for (int i = 0; i < 4; ++i)
    {
        if (std::abs(a[i].x - b[i].x) > GENERAL_EPS) return false;
        if (std::abs(a[i].y - b[i].y) > GENERAL_EPS) return false;
        if (std::abs(a[i].z - b[i].z) > GENERAL_EPS) return false;
        if (std::abs(a[i].w - b[i].w) > GENERAL_EPS) return false;
    }
    return true;
}

struct test_context
{
    int failures{ 0 };

    void section(const std::string_view name) const
    {
        test_println("\n[{}]", name);
    }

    void expect(bool condition, const std::string_view test_name)
    {
        if (condition)
        {
            test_println("{}: PASSED", test_name);
            return;
        }

        test_println("{}: FAILED", test_name);
        ++failures;
    }
};

void run_vector_tests(test_context& ctx);
void run_float_vector_exhaustive_tests(test_context& ctx);
void run_int_vector_tests(test_context& ctx);
void run_uint_vector_tests(test_context& ctx);
void run_vector_scaffolding_tests(test_context& ctx);
void run_swizzle_scaffolding_tests(test_context& ctx);
void run_quaternion_tests(test_context& ctx);
void run_matrix_tests(test_context& ctx);
void run_backend_selection_tests(test_context& ctx);
void run_scalar_math_tests(test_context& ctx);
void run_scalar_math_edge_case_tests(test_context& ctx);
void run_scalar_math_exhaustive_tests(test_context& ctx);
