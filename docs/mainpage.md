# CarrotHLM API Documentation

CarrotHLM is a fast, header-only, dependency-free C++23 math library for games,
graphics, and real-time applications. Its public API combines HLSL-style vectors
and swizzles with matrices, quaternions, transforms, and owned scalar math.

@section features Features

- Packed `float2`, `float3`, and `float4` types with HLSL-style swizzles.
- `int2/3/4` and `uint2/3/4` integer vector families.
- Column-major `float3x3` and `float4x4` matrices.
- Quaternion construction, interpolation, rotation, and matrix conversion.
- Left- and right-handed view, perspective, and orthographic transforms.
- Portable scalar, SSE2, and ARM64 NEON backends selected at compile time.
- Owned root, trigonometric, and inverse-trigonometric implementations, including
  `sqrt`, `rsqrt`, paired `sin_cos`, and matching higher-accuracy variants.
- Constexpr `isnan`, `isinf`, `isfinite`, `signbit`, and `copysign` helpers.
- Constexpr `floor`, `ceil`, `trunc`, `round`, `fmod`, and `frac` operations.

@section scalar_math Scalar Math

The default scalar functions are tuned for high-quality real-time use. Matching
`_precise` variants use additional work to provide tighter error bounds. Each
function's API documentation states its finite-input error target and special
value behavior.

Downstream vector, matrix, quaternion, and utility operations use this same
public scalar layer, so backend behavior remains consistent across the library.

@section platforms Platforms And Compilers

CarrotHLM supports macOS, Linux, and Windows with Clang, GCC, and MSVC. SSE2 is
used on x86/x64, NEON on ARM64, and the scalar backend is available as a portable
fallback and explicit test configuration.

@section usage Usage

Add the `CarrotHLM::CarrotHLM` CMake target and include the umbrella header:

```cpp
#include <chlm/CarrotHLM.h>

chlm::float3 direction{ 3.f, 4.f, 0.f };
chlm::float3 unit_direction{ chlm::normalize(direction) };
chlm::float4x4 projection{
    chlm::float4x4::perspective_lh(chlm::half_pi, 16.f / 9.f, .1f, 1000.f)
};
```

Angles are expressed in radians. Matrices are column-major and use column
vectors. The library's default spatial convention is left-handed with +Z forward.
