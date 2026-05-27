include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES
    CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(lab_tests ${TEST_SOURCES})

target_link_libraries(lab_tests
    PRIVATE
        lazy_sequence
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lab_tests)