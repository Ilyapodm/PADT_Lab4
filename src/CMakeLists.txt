add_executable(lab_main
    main.cpp
    # console/console_menu.cpp
    # console/lazy_sequence_demo.cpp
)

target_link_libraries(lab_main
    PRIVATE
        lab4::core
)

target_include_directories(lab_main
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)