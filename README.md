# CarrotHLM - Carrot HLSL Math 🥕

A fast, header-only, dependency-free math library for game engines and graphics, designed to feel like writing HLSL on the CPU.

Built around a portable SIMD backend, CarrotHLM delivers **native 128-bit SIMD acceleration** where available (NEON on ARM64/Apple Silicon, SSE2 on x86/x64) with a scalar fallback everywhere else, while providing the exact swizzle syntax and semantics you know from HLSL:

```c++
float4 pos{ 10.f, 20.f, 30.f, 40.f };
float3 xyz = pos.xyz;             // (10, 20, 30)
float4 weird = pos.yxwz;          // (20, 10, 30, 40)
float4 aaaa = pos.aaaa;           // (40, 40, 40, 40)
float3 rgb = pos.rgb;             // color aliases too!
float2 uv  = pos.st;              // texture coords
```

### Features
- **Full HLSL-style swizzles** - `.xyz`, `.wwww`, `.yzwx`, `.rgba`, `.stpq` - all zero-cost and native.
- **`float2` / `float3` / `float4`** with component-wise arithmetic, dot, cross, normalize, lerp.
- **`float4x4`** - column-major, full transform suite (translate, scale, rotate, axis-angle, look_at/perspective/ortho LH & RH).
- **`float3x3`** - pure rotation matrices, fast inverse (transpose).
- **Quaternions** - `using quat = float4`, axis-angle, slerp/nlerp, matrix conversion.
- **Integer vectors**: `int2/3/4`, `uint2/3/4` - perfect for pixel coords, grids, bitmasks.
- **Portable SIMD backends**: scalar fallback, SSE2 on x86/x64, NEON on ARM64.
- **Owned scalar math**: fast-by-default `sqrt`, `sin`, `cos`, `tan`, and `acos`, plus higher-accuracy `_precise` variants.
- Utilities: affine inverse, normal matrix, conversions.
- Header-only · No external dependencies · C++23.

Cross-platform: macOS (Apple Silicon + Intel), Linux, Windows.

[![API Documentation](https://img.shields.io/badge/docs-Doxygen-blue.svg)](https://zackshrout.github.io/CarrotHLM/)

### Requirements
- **Compiler**: A modern C++23 compiler.
- Clang, GCC, and MSVC are all intended targets for the portable SIMD implementation.
- SIMD backends are selected per platform: SSE2 on x86/x64, NEON on ARM64, scalar fallback otherwise.

### Integration (Git Submodule Recommended)
```bash
git submodule add https://github.com/ZackShrout/CarrotHLM your_dependencies_folder/CarrotHLM
```
In your CMakeLists.txt:
```cmake
add_subdirectory(your_dependencies_folder/CarrotHLM)
target_link_libraries(your_engine PRIVATE CarrotHLM::CarrotHLM)
```
Then simply:
```c++
#include <chlm/CarrotHLM.h>

using namespace chlm;
// ... use float4, quat, float4x4, etc.
```

### Scalar Math

CarrotHLM owns the runtime implementations behind its core scalar math API instead of forwarding to a platform C math library. The default functions target high-quality real-time game math, while matching `_precise` functions provide tighter bounds when accuracy matters more than throughput:

```c++
float fast_length = sqrt(length_squared(direction));
float precise_length = sqrt_precise(length_squared(direction));

float fast_angle = acos(dot(normal_a, normal_b));
float precise_angle = acos_precise(dot(normal_a, normal_b));
```

The public API documents each function's finite-input error target. Special values, signed zero, domains, and exact endpoints are covered by the conformance test suite.

## Why CarrotHLM?
- Feels like writing HLSL on the CPU.
- Maximum performance with a small portable SIMD layer under the hood.
- Native NEON on Apple Silicon and ARM64, SSE2 on x86/x64, and a portable scalar fallback.
- Minimal, readable, engine-focused - no bloat.

Perfect for anyone building a custom game engine who wants fast, expressive, and portable math without the bloat of GLM or the complexity of Eigen.
### CarrotHLM: Because your CPU deserves to speak HLSL too. 🥕
