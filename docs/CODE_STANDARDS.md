# BunnySoft Official Coding Standards

**BunnySoft**  
**Official Studio Coding Standards**  
**Effective: July 2026**

---

## Purpose

This document defines the official BunnySoft coding standards for all active and future projects.

Unless a project has a documented exception approved on purpose, these rules apply by default.

Project-specific standards documents may copy this file and customize only the project namespace and any narrowly-scoped project rules that must differ.

---

## Core Principles

- Optimize for readability first.
- Prefer consistency over cleverness.
- Keep public API polished and predictable.
- Make implementation details obvious and easy to separate from public-facing code.
- Favor simple, explicit C++ over abstraction-heavy style.

---

## File And Folder Naming

### Public Include Layout

- Public include namespace folders use short lowercase slugs.
- Examples: `cpng/`, `cjpg/`, `chlm/`
- The slug should be easy to type, easy to read, and stable over time.

### C++ Folder Naming

- C++ code folders use `UpperCamelCase`.
- Examples: `Engine/`, `Renderer/`, `Assets/`, `World/`, `Audio/`

### C++ File Naming

- C++ header files use `UpperCamelCase.h`
- C++ source files use `UpperCamelCase.cpp`
- Examples: `CarrotPNG.h`, `CarrotJPEG.cpp`, `AssetManager.h`, `ShaderWatcherLinux.cpp`

### Implementation Detail Folders

- `detail/` is the one intentional lowercase exception for implementation-detail organization.
- `detail/` indicates non-public helper code that supports a public API or implementation unit.
- `detail/` may appear under a public include tree.
- `detail/` may also appear under `src/` when that better expresses the structure of a small standalone library.
- Do not use both `internal/` and `detail/` in the same codebase for the same concept.
- Prefer `detail/` over `internal/` going forward.

Examples:

- `include/chlm/detail/`
- `include/cpng/detail/`
- `src/detail/`

### Non-Code Folders

- Non-code folders may use lowercase when that is clearer and more conventional.
- Examples: `assets/`, `config/`, `saved/`, `shaders/`, `docs/`

---

## Naming Conventions

- All user-defined types (`struct`, `class`, `enum class`, `typedef`, `using`) use `snake_case_t`
- Functions use `snake_case`
- Variables use `snake_case`
- Parameters use `snake_case`
- Namespaces use `snake_case`
- Member functions use `snake_case`
- Private and protected member variables use `_snake_case`
- Compile-time constants and static globals use names like `k_max_entities`, `g_frames_in_flight`
- Macros are rare and use screaming case only when unavoidable

Examples:

- `image_info_t`
- `decode_error`
- `load_from_memory`
- `_is_static`
- `CARROT_ENABLE_TRACY`

---

## Namespace Rules

- Every project defines one primary root namespace.
- The project-specific root namespace must be declared near the top of the project-local standards file or README and kept consistent across the codebase.
- The root namespace should usually match the public include slug where practical.
- Examples: `cpng`, `cjpg`, `chlm`
- Sub-namespaces are preferred over excessively deep type names.
- Anonymous namespaces are encouraged in `.cpp` files for file-local symbols.
- Do not use anonymous namespaces in headers. Header-only implementation helpers use named `detail` sub-namespaces with `inline` or `constexpr` definitions so every translation unit shares one ODR-safe identity.
- Never use `using namespace` in headers.
- `using` declarations in `.cpp` files are allowed when they clearly improve readability and do not create ambiguity.

Project-local standards should fill in this line first:

- Project root namespace: `chlm`

---

## Core Language Style

- Use uniform/braced initialization everywhere practical
- Example: `int32_t frame_count{ 0 };`
- Float literals are always suffixed
- Examples: `0.f`, `1.f`, `3.14159f`
- Use `auto` only when:
- iterating containers
- the type is long and obvious from context
- it does not hide pointer or reference semantics
- Single-statement `if` / `for` / `while` blocks do not use braces
- Multi-statement blocks always use braces
- Braces use Allman style
- Apply `constexpr`, `const`, `noexcept`, and `[[nodiscard]]` aggressively wherever legal and useful
- `goto` is forbidden except for a deliberate cleanup path in low-level code where that is the clearest option

Examples:

```cpp
if (is_valid) return true;

for (uint32_t i{ 0 }; i < count; ++i)
    do_thing(i);
```

```cpp
if (!ready)
{
    log_error();
    return false;
}
```

---

## Type Discipline

- Prefer `struct` over `class`
- Use `class` only when private sections, invariants, or inheritance actually justify it
- Prefer free functions in namespaces over member functions when behavior is not tied to internal state
- Prefer functions in anonymous namespaces inside `.cpp` files over file-local free functions with `static` wherever practicable

---

## Struct And Class Layout Order

Use this layout order exactly:

```cpp
struct foo_t
{
public:
    // public methods
    // public data (rare, only if truly POD)

protected:
    // protected methods
    // protected data

private:
    // private methods
    // private data
};
```

Notes:

- `public:` should still be written explicitly even for `struct`
- Omit `protected:` or `private:` sections when unused
- Public data is acceptable for plain data carriers and POD-style types

---

## Pointers, References, And Const

- Pointers and references stick to the type
- Examples: `int32_t* ptr`, `float& value`
- East-const style is forbidden

---

## Header Rules

- Use `#pragma once`
- Prefer forward declarations over unnecessary includes in headers
- Never use `using namespace` in headers
- Public-facing functions that return important status or values should generally be `[[nodiscard]]`

---

## Error Handling

- Prefer explicit error returns over hidden failure
- Use project-appropriate error enums or result types where practical
- Keep failure modes easy to discover from the API
- Validate external data defensively

---

## Memory And Resource Management

- Avoid raw `new` and `delete` in normal code
- Prefer RAII and explicit ownership
- Low-level allocation code must make ownership obvious

---

## Module And System Boundaries

- Engine and runtime subsystems should expose explicit lifecycle or setup boundaries when needed
- Public API should not leak internal detail types unless that tradeoff is intentional
- Keep implementation detail code in `detail/` or private source units whenever possible

---

## Example Style

```cpp
#pragma once

#include <cstdint>

namespace cjpg::ecs {

using entity_id_t = uint32_t;

struct transform_component_t
{
public:
    void set_position(float x, float y, float z) noexcept;
    [[nodiscard]] bool is_static() const noexcept { return _is_static; }

private:
    float _position[3]{ 0.f, 0.f, 0.f };
    float _rotation[4]{ 0.f, 0.f, 0.f, 1.f };
    float _scale[3]{ 1.f, 1.f, 1.f };
    bool  _is_static{ false };
};

} // namespace cjpg::ecs
```

---

## Adoption Guidance

- New projects should start with this standard as-is.
- Existing projects should move toward compliance incrementally.
- When renaming old files or folders, prefer doing so alongside meaningful work in that area unless the inconsistency is causing active confusion.
- Avoid broad churn that does not improve clarity.

---

## Official Policy

This is BunnySoft's official coding standards document from July 2026 onward.

Project-specific copies may specialize the root namespace and add narrow local rules, but they should not casually diverge from the spirit or structure of this document.

---

## CarrotHLM Project-Specific Exceptions

CarrotHLM is a numerics and geometry library with a public API that intentionally follows established math-library conventions in a few areas rather than the default studio naming model.

These exceptions are intentional and approved for this project.

### Public Math Type Naming

The general studio rule says user-defined types use `snake_case_t`.

CarrotHLM intentionally does **not** follow that rule for its public math primitives.

Approved public type forms include:

- `float2`
- `float3`
- `float4`
- `int2`
- `uint4`
- `float3x3`
- `float4x4`

Reason:

- these names are conventional, compact, and immediately readable in math and graphics code
- they are already the established public identity of the library
- forcing `_t` suffixes here would make the API feel less natural for engine and shader-adjacent work

### Public Math Constant Naming

The general studio rule prefers constant names such as `k_max_entities`.

CarrotHLM intentionally allows conventional math names for public constants when those names are clearer and more idiomatic.

Approved examples include:

- `pi`
- `half_pi`
- `two_pi`
- `epsilon`
- `deg_to_rad`
- `rad_to_deg`

Reason:

- these are conventional mathematical names
- they read naturally in formulas and examples
- prefixing them with `k_` would not improve clarity in this domain

### Additional Lowercase Detail Subfolder

The general studio rule says `detail/` is the one intentional lowercase exception for implementation-detail organization.

CarrotHLM additionally allows:

- `include/chlm/detail/simd/`

Reason:

- `simd` is a well-known technical acronym and reads more naturally in lowercase as a submodule name
- `detail/simd/` expresses the backend implementation layer clearly
- renaming it to `SIMD/` or `Simd/` would not improve clarity for this project

### POD-Style Public Struct Layout

The general studio rule says `public:` should still be written explicitly even for `struct`.

CarrotHLM allows omission of an explicit `public:` section for simple POD-style public math structs when the entire type is intentionally public-facing data and inline behavior.

Approved examples include:

- vector types
- matrix types
- other small math carrier types with no private/protected sections

Reason:

- these types are intentionally lightweight value types
- their layout is usually entirely public by design
- omitting an empty `public:` section keeps them visually compact without making access intent ambiguous

If a type introduces private or protected members, explicit access sections should still be used normally.

### Operator-Heavy Public API

The general studio rule is written primarily for broader engine/application code and does not explicitly discuss numerics libraries with operator-rich APIs.

CarrotHLM explicitly allows:

- arithmetic operators
- index operators
- comparison operators where appropriate
- swizzle-assignment and similar math-facing syntactic conveniences

Reason:

- these are core to an ergonomic math library
- avoiding them would make the public API substantially worse
- this remains consistent with the project's goal of a polished, predictable game-math surface
