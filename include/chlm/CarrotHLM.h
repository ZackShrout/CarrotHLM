//
// CarrotHLM.h - Carrot High-Level Math library
//
// Created by Zack Shrout on 1/5/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//
// USAGE:
//   Simply #include "CarrotHLM.h" in your source files.
//   All types and functions are in the 'chlm' namespace.
//
// OVERVIEW:
//   A lightweight, header-only, high-performance math library focused on
//   3D graphics, games, and real-time applications.
//
// FEATURES:
//   - float2, float3, float4 using Clang/GCC extended vector types
//   - Quaternion (float4-based), float3x3 and float4x4 matrices (column-major)
//   - Core operations: dot, cross, normalize, lerp/slerp/nlerp
//   - Matrix builders: translate, scale, rotate, look_at, perspective, ortho
//   - Conversions: quat ↔ matrix, affine inverse, normal matrix
//   - Left- and right-handed variants for view/projection
//   - Constants: pi, unit vectors (right/up/forward), epsilon, etc.
//
// CONVENTIONS:
//   - Column-major matrices (HLSL/DirectX style)
//   - +Z forward (DirectX convention)
//   - Right-handed rotation by default
//   - Angles in radians
//   - All functions marked noexcept where possible
//   - Assumes normalized inputs for performance-critical paths (e.g. quat → matrix)
//
// REQUIREMENTS:
//   - Clang or GCC with support for __attribute__((ext_vector_type(...)))
//   - C++17 or later recommended
//

#pragma once

//
// All functionality is brought in via the following headers (in dependency order):
#include "Core.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Matrix3x3.h"
#include "MathConversions.h"
