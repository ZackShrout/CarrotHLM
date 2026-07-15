add_executable(CarrotHLM_test
    BackendSelectionTests.cpp
    FloatVectorExhaustiveTests.cpp
    IntVectorTests.cpp
    main.cpp
    MatrixTests.cpp
    QuaternionTests.cpp
    RandomTests.cpp
    ScalarMathEdgeCaseTests.cpp
    ScalarMathExhaustiveTests.cpp
    ScalarMathTests.cpp
    SwizzleScaffoldingTests.cpp
    UintVectorTests.cpp
    UtilitiesTests.cpp
    VectorTests.cpp
    VectorScaffoldingTests.cpp
)
target_link_libraries(CarrotHLM_test PRIVATE CarrotHLM::CarrotHLM)
target_compile_features(CarrotHLM_test PRIVATE cxx_std_23)

if(CARROTHLM_TEST_FORCE_SCALAR)
    target_compile_definitions(CarrotHLM_test PRIVATE CHLM_FORCE_SCALAR=1)
endif()

if(CARROTHLM_TEST_FORCE_SSE2)
    target_compile_definitions(CarrotHLM_test PRIVATE CHLM_FORCE_SSE2=1)
endif()

if(CARROTHLM_TEST_FORCE_NEON)
    target_compile_definitions(CarrotHLM_test PRIVATE CHLM_FORCE_NEON=1)
endif()

add_test(NAME CarrotHLM.validation COMMAND $<TARGET_FILE:CarrotHLM_test>)
