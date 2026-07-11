cmake_minimum_required(VERSION 3.24)

project(CarrotHLM LANGUAGES CXX)

option(CARROTHLM_REQUIRE_CLANG "Fail configure unless using a Clang-family compiler." ON)

if(CARROTHLM_REQUIRE_CLANG AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR
        "CarrotHLM requires Clang or clang-cl for full ext_vector_type swizzle support. "
        "Set CARROTHLM_REQUIRE_CLANG=OFF to configure at your own risk."
    )
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
            include/chlm/Utilities.h
            include/chlm/Vector.h
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
