add_executable(CarrotHLM_PractRandStream PractRandStream.cpp)
target_link_libraries(CarrotHLM_PractRandStream PRIVATE CarrotHLM::CarrotHLM)

set(CARROTHLM_TESTU01_ROOT "$ENV{HOME}/.local/testu01" CACHE PATH
    "TestU01 installation prefix used by the random validation runner."
)

find_path(CARROTHLM_TESTU01_INCLUDE_DIR
    NAMES bbattery.h unif01.h
    HINTS "${CARROTHLM_TESTU01_ROOT}/include"
)
find_library(CARROTHLM_TESTU01_LIBRARY
    NAMES testu01
    HINTS "${CARROTHLM_TESTU01_ROOT}/lib"
)
find_library(CARROTHLM_PROBDIST_LIBRARY
    NAMES probdist
    HINTS "${CARROTHLM_TESTU01_ROOT}/lib"
)
find_library(CARROTHLM_MYLIB_LIBRARY
    NAMES mylib
    HINTS "${CARROTHLM_TESTU01_ROOT}/lib"
)

if(NOT CARROTHLM_TESTU01_INCLUDE_DIR
    OR NOT CARROTHLM_TESTU01_LIBRARY
    OR NOT CARROTHLM_PROBDIST_LIBRARY
    OR NOT CARROTHLM_MYLIB_LIBRARY)
    message(FATAL_ERROR
        "TestU01 was not found. Set CARROTHLM_TESTU01_ROOT to its installation prefix."
    )
endif()

add_executable(CarrotHLM_TestU01 TestU01Runner.cpp)
target_include_directories(CarrotHLM_TestU01 PRIVATE ${CARROTHLM_TESTU01_INCLUDE_DIR})
target_link_libraries(CarrotHLM_TestU01 PRIVATE
    CarrotHLM::CarrotHLM
    ${CARROTHLM_TESTU01_LIBRARY}
    ${CARROTHLM_PROBDIST_LIBRARY}
    ${CARROTHLM_MYLIB_LIBRARY}
)

if(NOT WIN32)
    target_link_libraries(CarrotHLM_TestU01 PRIVATE m)
endif()
