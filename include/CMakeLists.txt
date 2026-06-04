add_library(lab4_core INTERFACE)  # создаем header библиотеку
add_library(lab4::core ALIAS lab4_core)  # просто другое имя

target_compile_features(lab4_core INTERFACE cxx_std_20)

target_include_directories(lab4_core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}  # добавляем путь к папке
)

# подключаем подпапки
add_subdirectory(adt)
add_subdirectory(adt_lab_2)
add_subdirectory(core)
add_subdirectory(generators)
add_subdirectory(nodes)
add_subdirectory(lazy_sequence)
add_subdirectory(streams)
# add_subdirectory(smart_data)