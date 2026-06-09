target_sources(lab_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/call_counter.hpp
)

target_include_directories(lab_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)