cmake_minimum_required(VERSION 4.1)
project(CarrotHLM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Enforce Clang (or clang-cl on Windows)
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #message(FATAL_ERROR "CarrotHLM requires Clang or clang-cl for full feature support (ext_vector_type swizzles).")
endif()

# Header-only library interface
add_library(CarrotHLM INTERFACE)
target_include_directories(CarrotHLM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(CarrotHLM INTERFACE cxx_std_23)

add_library(CarrotHLM::CarrotHLM ALIAS CarrotHLM)

# Only build tests if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(CARROTHLM_BUILD_TESTS "Build CarrotHLM validation tests" ON)
    if(CARROTHLM_BUILD_TESTS)
        enable_testing()
        add_subdirectory(test)
    endif()
endif()
