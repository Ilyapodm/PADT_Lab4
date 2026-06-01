include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(lab_tests)

target_link_libraries(lab_tests
    PRIVATE
        lab4::core
        GTest::gtest_main
)

add_subdirectory(adt)
add_subdirectory(core)
add_subdirectory(generators)
add_subdirectory(nodes)
# add_subdirectory(lazy_sequence)
# add_subdirectory(smart_data)
# add_subdirectory(streams)

include(GoogleTest)
gtest_discover_tests(lab_tests)