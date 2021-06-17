project(GoogleTests)

add_subdirectory(google-test-lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(GoogleTestsRun
        ${CMAKE_CURRENT_SOURCE_DIR}/data/string_test.cpp
        )

target_include_directories(GoogleTestsRun PUBLIC
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(GoogleTestsRun gtest gtest_main)
