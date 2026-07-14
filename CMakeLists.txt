cmake_minimum_required(VERSION 3.24)

project(CarrotHLM LANGUAGES CXX)

option(CARROTHLM_TEST_FORCE_SCALAR "Force the test target to use the scalar backend." OFF)
option(CARROTHLM_TEST_FORCE_SSE2 "Force the test target to use the SSE2 backend on x86/x64 builds." OFF)
option(CARROTHLM_TEST_FORCE_NEON "Force the test target to use the NEON backend on ARM64 builds." OFF)

set(CARROTHLM_TEST_FORCE_COUNT 0)

if(CARROTHLM_TEST_FORCE_SCALAR)
    math(EXPR CARROTHLM_TEST_FORCE_COUNT "${CARROTHLM_TEST_FORCE_COUNT} + 1")
endif()

if(CARROTHLM_TEST_FORCE_SSE2)
    math(EXPR CARROTHLM_TEST_FORCE_COUNT "${CARROTHLM_TEST_FORCE_COUNT} + 1")
endif()

if(CARROTHLM_TEST_FORCE_NEON)
    math(EXPR CARROTHLM_TEST_FORCE_COUNT "${CARROTHLM_TEST_FORCE_COUNT} + 1")
endif()

if(CARROTHLM_TEST_FORCE_COUNT GREATER 1)
    message(FATAL_ERROR
        "Only one of CARROTHLM_TEST_FORCE_SCALAR, CARROTHLM_TEST_FORCE_SSE2, or "
        "CARROTHLM_TEST_FORCE_NEON may be enabled at a time."
    )
endif()

if(CARROTHLM_TEST_FORCE_SSE2)
    if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64|i[3-6]86|x86)$")
        message(FATAL_ERROR
            "CARROTHLM_TEST_FORCE_SSE2 requires an x86/x64 target architecture."
        )
    endif()
endif()

if(CARROTHLM_TEST_FORCE_NEON)
    if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|ARM64|aarch64)$")
        message(FATAL_ERROR
            "CARROTHLM_TEST_FORCE_NEON requires an ARM64 target architecture."
        )
    endif()
endif()

add_library(CarrotHLM INTERFACE)
target_sources(CarrotHLM
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            include/chlm/CarrotHLM.h
            include/chlm/Core.h
            include/chlm/MathConversions.h
            include/chlm/Matrix3x3.h
            include/chlm/Matrix4x4.h
            include/chlm/Quaternion.h
            include/chlm/Rect.h
            include/chlm/ScalarMath.h
            include/chlm/Utilities.h
            include/chlm/Vector.h
            include/chlm/detail/CompilerConfig.h
            include/chlm/detail/GeneratedFloat2Swizzles.h
            include/chlm/detail/GeneratedFloat3Swizzles.h
            include/chlm/detail/GeneratedFloat4Swizzles.h
            include/chlm/detail/GeneratedInt2Swizzles.h
            include/chlm/detail/GeneratedInt3Swizzles.h
            include/chlm/detail/GeneratedInt4Swizzles.h
            include/chlm/detail/GeneratedUint2Swizzles.h
            include/chlm/detail/GeneratedUint3Swizzles.h
            include/chlm/detail/GeneratedUint4Swizzles.h
            include/chlm/detail/math/FloatBits.h
            include/chlm/detail/math/InverseTrig.h
            include/chlm/detail/math/MathCommon.h
            include/chlm/detail/math/Polynomial.h
            include/chlm/detail/math/RangeReduction.h
            include/chlm/detail/math/ScalarMath.h
            include/chlm/detail/math/Sqrt.h
            include/chlm/detail/math/Trig.h
            include/chlm/detail/PlatformConfig.h
            include/chlm/detail/PublicFloatVectors.h
            include/chlm/detail/PublicIntVectors.h
            include/chlm/detail/PublicSwizzleTypes.h
            include/chlm/detail/PublicUintVectors.h
            include/chlm/detail/SwizzleCommon.h
            include/chlm/detail/SwizzleProxy.h
            include/chlm/detail/VectorBase.h
            include/chlm/detail/VectorStorage.h
            include/chlm/detail/simd/SimdNeon.h
            include/chlm/detail/simd/SimdScalar.h
            include/chlm/detail/simd/SimdSelect.h
            include/chlm/detail/simd/SimdSse.h
            include/chlm/detail/simd/SimdTraits.h
)
target_include_directories(CarrotHLM INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(CarrotHLM INTERFACE cxx_std_23)

add_library(CarrotHLM::CarrotHLM ALIAS CarrotHLM)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()
