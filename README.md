# CarrotHLM — Carrot HLSL Math 🥕

A fast, header-only, dependency-free math library for game engines and graphics, designed to feel like writing HLSL on the CPU.

Built from the ground up with modern Clang/GCC **vector extensions**, CarrotHLM delivers **native 128-bit SIMD acceleration** (NEON on ARM64/Apple Silicon, SSE on x86_64) while providing the exact swizzle syntax and semantics you know from HLSL:

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
- Utilities: affine inverse, normal matrix, conversions.
- Header-only · No external dependencies · C++23.

Cross-platform: macOS (Apple Silicon + Intel), Linux, Windows (via clang-cl).

### Requirements
- **Compiler**: Clang 15 or newer (required for full `ext_vector_type` swizzle support and consistent behavior across platforms).
- GCC 11+ has partial support; MSVC is not supported (no equivalent vector extensions).

### Integration (Git Submodule Recommended)
```bash
git submodule add https://github.com/yourname/CarrotHLM extern/CarrotHLM
```
In your CMakeLists.txt:
```cmake
add_subdirectory(extern/CarrotHLM)
target_link_libraries(your_engine PRIVATE CarrotHLM::CarrotHLM)
```
Then simply:
```c++
#include <chlm/CarrotHLM.h>

using namespace chlm;
// ... use float4, quat, float4x4, etc.
```

## Why CarrotHLM?
- Feels like writing HLSL on the CPU.
- Maximum performance - direct use of compiler vector extensions, no wrapper overhead.
- Battle-tested on Apple Silicon - no aliasing bugs, full NEON.
- Minimal, readable, engine-focused - no bloat.

Perfect for anyone building a custom game engine who wants fast, expressive, and portable math without the bloat of GLM or the complexity of Eigen.
### CarrotHLM: Because your CPU deserves to speak HLSL too. 🥕

