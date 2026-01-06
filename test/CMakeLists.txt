add_executable(CarrotHLM_test main.cpp)
target_link_libraries(CarrotHLM_test PRIVATE CarrotHLM::CarrotHLM)
add_test(NAME CarrotHLM_validation COMMAND CarrotHLM_test)