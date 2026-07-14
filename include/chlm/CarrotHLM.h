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
//   - Portable float2/float3/float4, int2/int3/int4, and uint2/uint3/uint4 value types
//   - Generated HLSL-style swizzle families with read/write support where lanes are unique
//   - Quaternion (float4-based), float3x3 and float4x4 matrices (column-major)
//   - Core operations: dot, cross, normalize, lerp/slerp/nlerp
//   - Matrix builders: translate, scale, rotate, look_at, perspective, ortho
//   - Conversions: quat ↔ matrix, affine inverse, normal matrix
//   - Left- and right-handed variants for view/projection
//   - Owned scalar sqrt/trigonometry with fast defaults and `_precise` variants
//   - Constants: pi, unit vectors (right/up/forward), epsilon, etc.
//
// CONVENTIONS:
//   - Column-major matrices (HLSL/DirectX style)
//   - +Z forward (DirectX convention)
//   - Right-handed rotation by default
//   - Angles in radians
//   - All functions marked noexcept where possible
//   - float3 is publicly packed to 12 bytes while float math still uses a 4-lane backend internally
//   - Assumes normalized inputs for performance-critical paths (e.g. quat → matrix)
//
// REQUIREMENTS:
//   - C++23 or later recommended
//   - Header-only consumption for users
//   - Internal backend selection supports scalar fallback, SSE2, and ARM64 NEON
//

#pragma once

//
// All functionality is brought in via the following headers:
#include "Core.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "MathConversions.h"
#include "Rect.h"
#include "Utilities.h"
