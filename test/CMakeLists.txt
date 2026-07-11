add_executable(CarrotHLM_test main.cpp)
target_link_libraries(CarrotHLM_test PRIVATE CarrotHLM::CarrotHLM)
target_compile_features(CarrotHLM_test PRIVATE cxx_std_23)

add_test(NAME CarrotHLM.validation COMMAND $<TARGET_FILE:CarrotHLM_test>)
