add_library(lazy_sequence INTERFACE)

target_include_directories(lazy_sequence
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lab_2
)

target_compile_features(lazy_sequence INTERFACE cxx_std_20)

file(GLOB_RECURSE LAZY_SEQUENCE_HEADERS
    CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.tpp
)

target_sources(lazy_sequence
    INTERFACE
        ${LAZY_SEQUENCE_HEADERS}
)