#include "console/console_ui.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "console/console_io.hpp"
#include "utils/generator_functions.hpp"
#include "adt_lab_2/list_sequence.hpp"
#include "core/ordinal.hpp"
#include "smart_data/smart_data_generator.hpp"

ConsoleUI::ConsoleUI() {}

ConsoleUI::~ConsoleUI() {
    cleanup_();
}

void ConsoleUI::run() {
    main_menu_();
}

void ConsoleUI::main_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== main menu ===\n";
        std::cout << "1. ordinary sequences\n";
        std::cout << "2. lazy sequences\n";
        std::cout << "3. smart data configs\n";
        std::cout << "4. input streams\n";
        std::cout << "5. output streams\n";
        std::cout << "6. delete objects\n";
        std::cout << "0. exit\n";

        int choice = read_menu_choice("choose: ", 0, 6);

        try {
            switch (choice) {
                case 1:
                    ordinary_sequence_menu_();
                    break;
                case 2:
                    lazy_sequence_menu_();
                    break;
                case 3:
                    smart_data_menu_();
                    break;
                case 4:
                    input_stream_menu_();
                    break;
                case 5:
                    output_stream_menu_();
                    break;
                case 6:
                    deletion_menu_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::ordinary_sequence_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== ordinary sequence menu ===\n";
        std::cout << "1. create sequence\n";
        std::cout << "2. list sequences\n";
        std::cout << "3. print prefix\n";
        std::cout << "4. get element\n";
        std::cout << "5. show size\n";
        std::cout << "6. append\n";
        std::cout << "7. prepend\n";
        std::cout << "8. insert_at\n";
        std::cout << "9. set\n";
        std::cout << "10. remove_at\n";
        std::cout << "11. subsequence [start, end]\n";
        std::cout << "12. concat\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 12);

        try {
            switch (choice) {
                case 1:
                    create_ordinary_sequence_();
                    break;
                case 2:
                    list_ordinary_sequences_();
                    break;
                case 3:
                    print_ordinary_sequence_prefix_();
                    break;
                case 4:
                    get_ordinary_sequence_element_();
                    break;
                case 5:
                    show_ordinary_sequence_size_();
                    break;
                case 6:
                    append_to_ordinary_sequence_();
                    break;
                case 7:
                    prepend_to_ordinary_sequence_();
                    break;
                case 8:
                    insert_into_ordinary_sequence_();
                    break;
                case 9:
                    set_ordinary_sequence_element_();
                    break;
                case 10:
                    remove_ordinary_sequence_element_();
                    break;
                case 11:
                    ordinary_sequence_subsequence_();
                    break;
                case 12:
                    concat_ordinary_sequences_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::lazy_sequence_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== lazy sequence menu ===\n";
        std::cout << "1. create empty\n";
        std::cout << "2. create from array input\n";
        std::cout << "3. create snapshot from ordinary sequence\n";
        std::cout << "4. create from function preset\n";
        std::cout << "5. create from recurrence preset\n";
        std::cout << "6. create from smart data config\n";
        std::cout << "7. list lazy sequences\n";
        std::cout << "8. print prefix\n";
        std::cout << "9. get by finite index\n";
        std::cout << "10. get by ordinal index\n";
        std::cout << "11. show length and materialized count\n";
        std::cout << "12. append\n";
        std::cout << "13. prepend\n";
        std::cout << "14. insert_at\n";
        std::cout << "15. set\n";
        std::cout << "16. remove_at\n";
        std::cout << "17. subsequence [start, end)\n";
        std::cout << "18. concat\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 18);

        try {
            switch (choice) {
                case 1:
                    create_empty_lazy_sequence_();
                    break;
                case 2:
                    create_lazy_sequence_from_array_();
                    break;
                case 3:
                    create_lazy_sequence_from_sequence_();
                    break;
                case 4:
                    create_lazy_sequence_from_function_preset_();
                    break;
                case 5:
                    create_lazy_sequence_from_recurrence_preset_();
                    break;
                case 6:
                    create_lazy_sequence_from_smart_config_();
                    break;
                case 7:
                    list_lazy_sequences_();
                    break;
                case 8:
                    print_lazy_sequence_prefix_();
                    break;
                case 9:
                    get_lazy_sequence_finite_element_();
                    break;
                case 10:
                    get_lazy_sequence_ordinal_element_();
                    break;
                case 11:
                    show_lazy_sequence_info_();
                    break;
                case 12:
                    append_to_lazy_sequence_();
                    break;
                case 13:
                    prepend_to_lazy_sequence_();
                    break;
                case 14:
                    insert_into_lazy_sequence_();
                    break;
                case 15:
                    set_lazy_sequence_element_();
                    break;
                case 16:
                    remove_lazy_sequence_element_();
                    break;
                case 17:
                    lazy_sequence_subsequence_();
                    break;
                case 18:
                    concat_lazy_sequences_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::smart_data_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== smart data config menu ===\n";
        std::cout << "1. create config\n";
        std::cout << "2. list configs\n";
        std::cout << "3. preview generated values\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 3);

        try {
            switch (choice) {
                case 1:
                    create_smart_data_config_();
                    break;
                case 2:
                    list_smart_data_configs_();
                    break;
                case 3:
                    preview_smart_data_config_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::input_stream_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== input streams ===\n";
        std::cout << "1. sequence input streams\n";
        std::cout << "2. lazy sequence input streams\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 2);

        try {
            switch (choice) {
                case 1:
                    sequence_input_stream_menu_();
                    break;
                case 2:
                    lazy_input_stream_menu_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::output_stream_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== output streams ===\n";
        std::cout << "1. sequence output streams\n";
        std::cout << "2. lazy sequence output streams\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 2);

        try {
            switch (choice) {
                case 1:
                    sequence_output_stream_menu_();
                    break;
                case 2:
                    lazy_output_stream_menu_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::sequence_input_stream_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== sequence input streams ===\n";
        std::cout << "1. create from ordinary sequence\n";
        std::cout << "2. list\n";
        std::cout << "3. open\n";
        std::cout << "4. close\n";
        std::cout << "5. reset\n";
        std::cout << "6. read one\n";
        std::cout << "7. read many\n";
        std::cout << "8. seek by finite position\n";
        std::cout << "9. show finite position\n";
        std::cout << "10. is end of stream\n";
        std::cout << "11. delete\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 11);

        try {
            switch (choice) {
                case 1:
                    create_sequence_input_stream_();
                    break;
                case 2:
                    list_sequence_input_streams_();
                    break;
                case 3:
                    open_sequence_input_stream_();
                    break;
                case 4:
                    close_sequence_input_stream_();
                    break;
                case 5:
                    reset_sequence_input_stream_();
                    break;
                case 6:
                    read_one_from_sequence_input_stream_();
                    break;
                case 7:
                    read_many_from_sequence_input_stream_();
                    break;
                case 8:
                    seek_sequence_input_stream_();
                    break;
                case 9:
                    show_sequence_input_stream_position_();
                    break;
                case 10:
                    show_sequence_input_stream_end_state_();
                    break;
                case 11:
                    delete_sequence_input_stream_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::lazy_input_stream_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== lazy sequence input streams ===\n";
        std::cout << "1. create from lazy sequence\n";
        std::cout << "2. list\n";
        std::cout << "3. open\n";
        std::cout << "4. close\n";
        std::cout << "5. reset\n";
        std::cout << "6. read one\n";
        std::cout << "7. read many\n";
        std::cout << "8. seek by finite position\n";
        std::cout << "9. seek by ordinal position\n";
        std::cout << "10. show finite position\n";
        std::cout << "11. show ordinal position\n";
        std::cout << "12. is end of stream\n";
        std::cout << "13. delete\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 13);

        try {
            switch (choice) {
                case 1:
                    create_lazy_input_stream_();
                    break;
                case 2:
                    list_lazy_input_streams_();
                    break;
                case 3:
                    open_lazy_input_stream_();
                    break;
                case 4:
                    close_lazy_input_stream_();
                    break;
                case 5:
                    reset_lazy_input_stream_();
                    break;
                case 6:
                    read_one_from_lazy_input_stream_();
                    break;
                case 7:
                    read_many_from_lazy_input_stream_();
                    break;
                case 8:
                    seek_lazy_input_stream_finite_();
                    break;
                case 9:
                    seek_lazy_input_stream_ordinal_();
                    break;
                case 10:
                    show_lazy_input_stream_finite_position_();
                    break;
                case 11:
                    show_lazy_input_stream_ordinal_position_();
                    break;
                case 12:
                    show_lazy_input_stream_end_state_();
                    break;
                case 13:
                    delete_lazy_input_stream_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::sequence_output_stream_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== sequence output streams ===\n";
        std::cout << "1. create from ordinary sequence\n";
        std::cout << "2. list\n";
        std::cout << "3. open\n";
        std::cout << "4. close\n";
        std::cout << "5. write one\n";
        std::cout << "6. write many\n";
        std::cout << "7. show finite position\n";
        std::cout << "8. release ordinary sequence\n";
        std::cout << "9. delete\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 9);

        try {
            switch (choice) {
                case 1:
                    create_sequence_output_stream_();
                    break;
                case 2:
                    list_sequence_output_streams_();
                    break;
                case 3:
                    open_sequence_output_stream_();
                    break;
                case 4:
                    close_sequence_output_stream_();
                    break;
                case 5:
                    write_one_to_sequence_output_stream_();
                    break;
                case 6:
                    write_many_to_sequence_output_stream_();
                    break;
                case 7:
                    show_sequence_output_stream_position_();
                    break;
                case 8:
                    release_sequence_output_stream_sequence_();
                    break;
                case 9:
                    delete_sequence_output_stream_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::lazy_output_stream_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== lazy sequence output streams ===\n";
        std::cout << "1. create from lazy sequence\n";
        std::cout << "2. list\n";
        std::cout << "3. open\n";
        std::cout << "4. close\n";
        std::cout << "5. write one\n";
        std::cout << "6. write many\n";
        std::cout << "7. show finite position\n";
        std::cout << "8. show ordinal position\n";
        std::cout << "9. release lazy sequence\n";
        std::cout << "10. delete\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 10);

        try {
            switch (choice) {
                case 1:
                    create_lazy_output_stream_();
                    break;
                case 2:
                    list_lazy_output_streams_();
                    break;
                case 3:
                    open_lazy_output_stream_();
                    break;
                case 4:
                    close_lazy_output_stream_();
                    break;
                case 5:
                    write_one_to_lazy_output_stream_();
                    break;
                case 6:
                    write_many_to_lazy_output_stream_();
                    break;
                case 7:
                    show_lazy_output_stream_finite_position_();
                    break;
                case 8:
                    show_lazy_output_stream_ordinal_position_();
                    break;
                case 9:
                    release_lazy_output_stream_sequence_();
                    break;
                case 10:
                    delete_lazy_output_stream_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::deletion_menu_() {
    bool running = true;

    while (running) {
        std::cout << "\n=== delete objects menu ===\n";
        std::cout << "1. delete ordinary sequence\n";
        std::cout << "2. delete lazy sequence\n";
        std::cout << "3. delete smart data config\n";
        std::cout << "0. back\n";

        int choice = read_menu_choice("choose: ", 0, 3);

        try {
            switch (choice) {
                case 1:
                    delete_ordinary_sequence_();
                    break;
                case 2:
                    delete_lazy_sequence_();
                    break;
                case 3:
                    delete_smart_config_();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    break;
            }
        } catch (const std::exception& error) {
            print_exception(error);
        }
    }
}

void ConsoleUI::create_ordinary_sequence_() {
    std::cout << "\ncontainer type:\n";
    std::cout << "1. array\n";
    std::cout << "2. list\n";
    int container_choice = read_menu_choice("choose: ", 1, 2);

    std::cout << "\nmutability:\n";
    std::cout << "1. mutable\n";
    std::cout << "2. immutable\n";
    int mutability_choice = read_menu_choice("choose: ", 1, 2);

    int count = read_non_negative_int("count: ");
    int* items = read_int_array_(count);

    Sequence<int>* sequence = nullptr;
    SequenceContainerKind container_kind = container_choice == 1 ? SequenceContainerKind::Array : SequenceContainerKind::List;
    SequenceMutability mutability = mutability_choice == 1 ? SequenceMutability::Mutable : SequenceMutability::Immutable;

    try {
        if (container_kind == SequenceContainerKind::Array && mutability == SequenceMutability::Mutable) {
            sequence = new MutableArraySequence<int>(items, count);
        } else if (container_kind == SequenceContainerKind::Array && mutability == SequenceMutability::Immutable) {
            sequence = new ImmutableArraySequence<int>(items, count);
        } else if (container_kind == SequenceContainerKind::List && mutability == SequenceMutability::Mutable) {
            sequence = new MutableListSequence<int>(items, count);
        } else {
            sequence = new ImmutableListSequence<int>(items, count);
        }

        std::ostringstream description;
        description << to_string(mutability) << " " << to_string(container_kind) << " sequence from console input";
        int id = add_sequence_record_(sequence, container_kind, mutability, description.str());

        std::cout << "created ordinary sequence #" << id << "\n";
    } catch (...) {
        delete[] items;
        delete sequence;
        throw;
    }

    delete[] items;
}

void ConsoleUI::list_ordinary_sequences_() const {
    std::cout << "\n=== ordinary sequences ===\n";

    for (int i = 0; i < sequence_records_.get_size(); ++i) {
        SequenceRecord* record = sequence_records_.get(i);

        std::cout << "#" << i << " ";

        if (record == nullptr || record->is_deleted || record->sequence == nullptr) {
            std::cout << "[deleted]\n";
            continue;
        }

        std::cout << sequence_record_title_(record)
                  << ", size = " << record->sequence->get_size()
                  << ", " << record->description << "\n";
    }
}

void ConsoleUI::print_ordinary_sequence_prefix_() {
    int id = choose_sequence_id_();
    std::size_t count = read_size_t("prefix count: ");

    SequenceRecord* record = get_sequence_record_(id);
    print_sequence_prefix_(record->sequence, count);
}

void ConsoleUI::get_ordinary_sequence_element_() {
    int id = choose_sequence_id_();
    int index = read_int("index: ");

    SequenceRecord* record = get_sequence_record_(id);
    std::cout << "value = " << record->sequence->get(index) << "\n";
}

void ConsoleUI::show_ordinary_sequence_size_() {
    int id = choose_sequence_id_();
    SequenceRecord* record = get_sequence_record_(id);
    std::cout << "size = " << record->sequence->get_size() << "\n";
}

void ConsoleUI::append_to_ordinary_sequence_() {
    int id = choose_sequence_id_();
    int value = read_int("value: ");

    SequenceRecord* record = get_sequence_record_(id);
    Sequence<int>* result = record->sequence->append(value);
    handle_ordinary_sequence_result_(id, result, "append(value) result from ordinary sequence");
}

void ConsoleUI::prepend_to_ordinary_sequence_() {
    int id = choose_sequence_id_();
    int value = read_int("value: ");

    SequenceRecord* record = get_sequence_record_(id);
    Sequence<int>* result = record->sequence->prepend(value);
    handle_ordinary_sequence_result_(id, result, "prepend(value) result from ordinary sequence");
}

void ConsoleUI::insert_into_ordinary_sequence_() {
    int id = choose_sequence_id_();
    int index = read_int("index: ");
    int value = read_int("value: ");

    SequenceRecord* record = get_sequence_record_(id);
    Sequence<int>* result = record->sequence->insert_at(value, index);
    handle_ordinary_sequence_result_(id, result, "insert_at(value, index) result from ordinary sequence");
}

void ConsoleUI::set_ordinary_sequence_element_() {
    int id = choose_sequence_id_();
    int index = read_int("index: ");
    int value = read_int("value: ");

    SequenceRecord* record = get_sequence_record_(id);
    Sequence<int>* result = record->sequence->set(value, index);
    handle_ordinary_sequence_result_(id, result, "set(value, index) result from ordinary sequence");
}

void ConsoleUI::remove_ordinary_sequence_element_() {
    int id = choose_sequence_id_();
    int index = read_int("index: ");

    SequenceRecord* record = get_sequence_record_(id);
    Sequence<int>* result = record->sequence->remove_at(index);
    handle_ordinary_sequence_result_(id, result, "remove_at(index) result from ordinary sequence");
}

void ConsoleUI::ordinary_sequence_subsequence_() {
    std::cout << "ordinary subsequence uses inclusive end: [start, end]\n";

    int id = choose_sequence_id_();
    int start = read_int("start index: ");
    int end = read_int("end index: ");

    SequenceRecord* record = get_sequence_record_(id);
    Sequence<int>* result = record->sequence->get_subsequence(start, end);
    handle_ordinary_sequence_result_(id, result, "subsequence [start, end] result from ordinary sequence");
}

void ConsoleUI::concat_ordinary_sequences_() {
    std::cout << "left operand defines result container/mutability metadata\n";

    int left_id = choose_sequence_id_();
    int right_id = choose_sequence_id_();

    SequenceRecord* left = get_sequence_record_(left_id);
    SequenceRecord* right = get_sequence_record_(right_id);

    Sequence<int>* result = left->sequence->concat(*right->sequence);
    handle_ordinary_sequence_result_(left_id, result, "concat(left, right) result from ordinary sequence");
}

void ConsoleUI::create_empty_lazy_sequence_() {
    LazySequence<int>* sequence = new LazySequence<int>();
    int id = add_lazy_record_(sequence, LazySequenceOrigin::Empty, "empty lazy sequence");
    std::cout << "created lazy sequence #" << id << "\n";
}

void ConsoleUI::create_lazy_sequence_from_array_() {
    int count = read_non_negative_int("count: ");
    int* items = read_int_array_(count);

    try {
        LazySequence<int>* sequence = new LazySequence<int>(items, count);
        int id = add_lazy_record_(sequence, LazySequenceOrigin::ArrayInput, "lazy sequence from console array input");
        std::cout << "created lazy sequence #" << id << "\n";
    } catch (...) {
        delete[] items;
        throw;
    }

    delete[] items;
}

void ConsoleUI::create_lazy_sequence_from_sequence_() {
    int sequence_id = choose_sequence_id_();
    SequenceRecord* record = get_sequence_record_(sequence_id);

    LazySequence<int>* sequence = new LazySequence<int>(*record->sequence);

    std::ostringstream description;
    description << "lazy sequence snapshot from ordinary sequence #" << sequence_id;

    int id = add_lazy_record_(sequence, LazySequenceOrigin::SequenceSnapshot, description.str());
    std::cout << "created lazy sequence #" << id << "\n";
}

void ConsoleUI::create_lazy_sequence_from_function_preset_() {
    std::cout << "\nfunction presets:\n";
    std::cout << "1. identity: a_i = i\n";
    std::cout << "2. square: a_i = i * i\n";
    std::cout << "3. odd linear: a_i = 2 * i + 1\n";
    std::cout << "4. constant seven\n";

    int preset = read_menu_choice("choose: ", 1, 4);
    Ordinal length = read_generator_length("choose length");

    int (*function)(std::size_t) = nullptr;
    std::string name;

    if (preset == 1) {
        function = identity_function;
        name = "identity function preset";
    } else if (preset == 2) {
        function = square_function;
        name = "square function preset";
    } else if (preset == 3) {
        function = odd_linear_function;
        name = "odd linear function preset";
    } else {
        function = constant_seven_function;
        name = "constant seven function preset";
    }

    LazySequence<int>* sequence = new LazySequence<int>(function, length);

    std::ostringstream description;
    description << name << ", length = " << length;

    int id = add_lazy_record_(sequence, LazySequenceOrigin::FunctionPreset, description.str());
    std::cout << "created lazy sequence #" << id << "\n";
}

void ConsoleUI::create_lazy_sequence_from_recurrence_preset_() {
    std::cout << "\nrecurrence presets:\n";
    std::cout << "1. fibonacci: 0, 1, 1, 2, 3, ...\n";
    std::cout << "2. increment by one from start\n";
    std::cout << "3. double previous from start\n";

    int preset = read_menu_choice("choose: ", 1, 3);
    Ordinal length = read_generator_length("choose length");

    int* initial = nullptr;
    std::size_t count = 0;
    int (*rule)(const RingBuffer<int>&) = nullptr;
    std::string name;

    try {
        if (preset == 1) {
            count = 2;
            initial = new int[2];
            initial[0] = 0;
            initial[1] = 1;
            rule = fibonacci_ring_rule;
            name = "fibonacci recurrence preset";
        } else if (preset == 2) {
            count = 1;
            initial = new int[1];
            initial[0] = read_int("start value: ");
            rule = increment_by_one_ring_rule;
            name = "increment-by-one recurrence preset";
        } else {
            count = 1;
            initial = new int[1];
            initial[0] = read_int("start value: ");
            rule = double_previous_ring_rule;
            name = "double-previous recurrence preset";
        }

        LazySequence<int>* sequence = new LazySequence<int>(rule, initial, count, length);

        std::ostringstream description;
        description << name << ", length = " << length;

        int id = add_lazy_record_(sequence, LazySequenceOrigin::RecurrencePreset, description.str());
        std::cout << "created lazy sequence #" << id << "\n";
    } catch (...) {
        delete[] initial;
        throw;
    }

    delete[] initial;
}

void ConsoleUI::create_lazy_sequence_from_smart_config_() {
    int config_id = choose_smart_config_id_();
    SmartConfigRecord* record = get_smart_config_record_(config_id);

    SmartDataGenerator generator(record->config);
    LazySequence<int>* sequence = new LazySequence<int>(generator);

    std::ostringstream description;
    description << "lazy sequence over smart data config #" << config_id << ": " << smart_config_to_string_(record->config);

    int id = add_lazy_record_(sequence, LazySequenceOrigin::SmartDataConfig, description.str());
    std::cout << "created lazy sequence #" << id << "\n";
}

void ConsoleUI::list_lazy_sequences_() const {
    std::cout << "\n=== lazy sequences ===\n";

    for (int i = 0; i < lazy_records_.get_size(); ++i) {
        LazySequenceRecord* record = lazy_records_.get(i);

        std::cout << "#" << i << " ";

        if (record == nullptr || record->is_deleted || record->sequence == nullptr) {
            std::cout << "[deleted]\n";
            continue;
        }

        std::cout << "length = " << lazy_length_to_string_(record->sequence)
                  << ", materialized = " << record->sequence->get_materialized_count()
                  << ", " << record->description << "\n";
    }
}

void ConsoleUI::print_lazy_sequence_prefix_() {
    int id = choose_lazy_id_();
    std::size_t count = read_size_t("prefix count: ");

    LazySequenceRecord* record = get_lazy_record_(id);
    print_lazy_prefix_(record->sequence, count);
}

void ConsoleUI::get_lazy_sequence_finite_element_() {
    int id = choose_lazy_id_();
    int index = read_int("finite index: ");

    LazySequenceRecord* record = get_lazy_record_(id);
    std::cout << "value = " << record->sequence->get(index) << "\n";
}

void ConsoleUI::get_lazy_sequence_ordinal_element_() {
    int id = choose_lazy_id_();
    Ordinal index = read_ordinal_index("choose index");

    LazySequenceRecord* record = get_lazy_record_(id);
    std::cout << "value = " << record->sequence->get(index) << "\n";
}

void ConsoleUI::show_lazy_sequence_info_() {
    int id = choose_lazy_id_();
    LazySequenceRecord* record = get_lazy_record_(id);

    std::cout << "length = " << record->sequence->get_ordinal_length() << "\n";
    std::cout << "materialized count = " << record->sequence->get_materialized_count() << "\n";

    try {
        std::cout << "finite size = " << record->sequence->get_size() << "\n";
    } catch (const std::exception& error) {
        std::cout << "finite size unavailable: " << error.what() << "\n";
    }
}

void ConsoleUI::append_to_lazy_sequence_() {
    int id = choose_lazy_id_();
    int value = read_int("value: ");

    LazySequenceRecord* record = get_lazy_record_(id);
    LazySequence<int>* result = record->sequence->append(value);

    std::ostringstream description;
    description << "append(" << value << ") from lazy sequence #" << id;

    int result_id = add_lazy_record_(result, LazySequenceOrigin::Append, description.str());
    std::cout << "created lazy sequence #" << result_id << "\n";
}

void ConsoleUI::prepend_to_lazy_sequence_() {
    int id = choose_lazy_id_();
    int value = read_int("value: ");

    LazySequenceRecord* record = get_lazy_record_(id);
    LazySequence<int>* result = record->sequence->prepend(value);

    std::ostringstream description;
    description << "prepend(" << value << ") from lazy sequence #" << id;

    int result_id = add_lazy_record_(result, LazySequenceOrigin::Prepend, description.str());
    std::cout << "created lazy sequence #" << result_id << "\n";
}

void ConsoleUI::insert_into_lazy_sequence_() {
    int id = choose_lazy_id_();
    Ordinal index = read_ordinal_index("choose insert index");
    int value = read_int("value: ");

    LazySequenceRecord* record = get_lazy_record_(id);
    LazySequence<int>* result = record->sequence->insert_at(value, index);

    std::ostringstream description;
    description << "insert_at(" << value << ", " << index << ") from lazy sequence #" << id;

    int result_id = add_lazy_record_(result, LazySequenceOrigin::InsertAt, description.str());
    std::cout << "created lazy sequence #" << result_id << "\n";
}

void ConsoleUI::set_lazy_sequence_element_() {
    int id = choose_lazy_id_();
    Ordinal index = read_ordinal_index("choose set index");
    int value = read_int("value: ");

    LazySequenceRecord* record = get_lazy_record_(id);
    LazySequence<int>* result = record->sequence->set(value, index);

    std::ostringstream description;
    description << "set(" << value << ", " << index << ") from lazy sequence #" << id;

    int result_id = add_lazy_record_(result, LazySequenceOrigin::Set, description.str());
    std::cout << "created lazy sequence #" << result_id << "\n";
}

void ConsoleUI::remove_lazy_sequence_element_() {
    int id = choose_lazy_id_();
    Ordinal index = read_ordinal_index("choose remove index");

    LazySequenceRecord* record = get_lazy_record_(id);
    LazySequence<int>* result = record->sequence->remove_at(index);

    std::ostringstream description;
    description << "remove_at(" << index << ") from lazy sequence #" << id;

    int result_id = add_lazy_record_(result, LazySequenceOrigin::RemoveAt, description.str());
    std::cout << "created lazy sequence #" << result_id << "\n";
}

void ConsoleUI::lazy_sequence_subsequence_() {
    std::cout << "lazy subsequence uses half-open interval: [start, end)\n";

    int id = choose_lazy_id_();
    Ordinal start = read_ordinal_index("choose start index");
    Ordinal end = read_ordinal_index("choose end exclusive index");

    LazySequenceRecord* record = get_lazy_record_(id);
    LazySequence<int>* result = record->sequence->get_subsequence(start, end);

    std::ostringstream description;
    description << "subsequence [" << start << ", " << end << ") from lazy sequence #" << id;

    int result_id = add_lazy_record_(result, LazySequenceOrigin::Subsequence, description.str());
    std::cout << "created lazy sequence #" << result_id << "\n";
}

void ConsoleUI::concat_lazy_sequences_() {
    int left_id = choose_lazy_id_();
    int right_id = choose_lazy_id_();

    LazySequenceRecord* left = get_lazy_record_(left_id);
    LazySequenceRecord* right = get_lazy_record_(right_id);

    LazySequence<int>* result = left->sequence->concat(*right->sequence);

    std::ostringstream description;
    description << "concat(lazy #" << left_id << ", lazy #" << right_id << ")";

    int result_id = add_lazy_record_(result, LazySequenceOrigin::Concat, description.str());
    std::cout << "created lazy sequence #" << result_id << "\n";
}

void ConsoleUI::create_smart_data_config_() {
    SmartDataConfig config;

    std::cout << "\ndistribution type:\n";
    std::cout << "1. uniform\n";
    std::cout << "2. normal\n";
    int distribution_choice = read_menu_choice("choose: ", 1, 2);

    config.distribution_type = distribution_choice == 1
        ? SmartDataDistributionType::Uniform
        : SmartDataDistributionType::Normal;

    config.length = read_generator_length("choose length");
    config.seed = read_uint32("seed: ");
    config.min_value = read_int("min value: ");
    config.max_value = read_int("max value: ");

    if (config.distribution_type == SmartDataDistributionType::Normal) {
        config.mean = read_double("mean: ");
        config.standard_deviation = read_double("standard deviation: ");
    }

    // validate config by constructing a temporary generator
    SmartDataGenerator validator(config);
    static_cast<void>(validator);

    int id = add_smart_config_record_(config, smart_config_to_string_(config));
    std::cout << "created smart data config #" << id << "\n";
}

void ConsoleUI::list_smart_data_configs_() const {
    std::cout << "\n=== smart data configs ===\n";

    for (int i = 0; i < smart_config_records_.get_size(); ++i) {
        SmartConfigRecord* record = smart_config_records_.get(i);

        std::cout << "#" << i << " ";

        if (record == nullptr || record->is_deleted) {
            std::cout << "[deleted]\n";
            continue;
        }

        std::cout << record->description << "\n";
    }
}

void ConsoleUI::preview_smart_data_config_() {
    int id = choose_smart_config_id_();
    std::size_t count = read_size_t("how many values to preview: ");

    SmartConfigRecord* record = get_smart_config_record_(id);
    SmartDataGenerator generator(record->config);

    std::cout << "[";

    for (std::size_t i = 0; i < count; ++i) {
        if (!generator.has_next()) {
            break;
        }

        if (i > 0) {
            std::cout << ", ";
        }

        std::cout << generator.get_next();
    }

    std::cout << "]\n";
}


void ConsoleUI::create_sequence_input_stream_() {
    int id = choose_sequence_id_();
    SequenceRecord* record = get_sequence_record_(id);

    SequenceInputStream<int>* stream = new SequenceInputStream<int>(record->sequence);

    std::ostringstream description;
    description << "sequence input stream over ordinary sequence #" << id;

    int stream_id = add_sequence_input_stream_record_(stream, id, description.str());
    std::cout << "created sequence input stream #" << stream_id << "\n";
}

void ConsoleUI::list_sequence_input_streams_() const {
    std::cout << "\n=== sequence input streams ===\n";

    for (int i = 0; i < sequence_input_stream_records_.get_size(); ++i) {
        SequenceInputStreamRecord* record = sequence_input_stream_records_.get(i);

        std::cout << "#" << i << " ";

        if (record == nullptr || record->is_deleted || record->stream == nullptr) {
            std::cout << "[deleted]\n";
            continue;
        }

        std::cout << record->description
                  << ", source = ordinary sequence #" << record->source_sequence_id
                  << ", " << (record->stream->is_open() ? "open" : "closed");

        if (record->stream->is_open()) {
            std::cout << ", position = " << record->stream->get_position();
        }

        std::cout << "\n";
    }
}

void ConsoleUI::open_sequence_input_stream_() {
    int id = choose_sequence_input_stream_id_();
    get_sequence_input_stream_record_(id)->stream->open();
    std::cout << "sequence input stream opened\n";
}

void ConsoleUI::close_sequence_input_stream_() {
    int id = choose_sequence_input_stream_id_();
    get_sequence_input_stream_record_(id)->stream->close();
    std::cout << "sequence input stream closed\n";
}

void ConsoleUI::reset_sequence_input_stream_() {
    int id = choose_sequence_input_stream_id_();
    get_sequence_input_stream_record_(id)->stream->reset();
    std::cout << "sequence input stream reset\n";
}

void ConsoleUI::read_one_from_sequence_input_stream_() {
    int id = choose_sequence_input_stream_id_();
    int value = get_sequence_input_stream_record_(id)->stream->input();
    std::cout << "value = " << value << "\n";
}

void ConsoleUI::read_many_from_sequence_input_stream_() {
    int id = choose_sequence_input_stream_id_();
    std::size_t count = read_size_t("count: ");

    SequenceInputStream<int>* stream = get_sequence_input_stream_record_(id)->stream;

    std::cout << "[";

    for (std::size_t i = 0; i < count; ++i) {
        if (stream->is_end_of_stream()) {
            break;
        }

        if (i > 0) {
            std::cout << ", ";
        }

        std::cout << stream->input();
    }

    std::cout << "]\n";
}

void ConsoleUI::seek_sequence_input_stream_() {
    int id = choose_sequence_input_stream_id_();
    std::size_t index = read_size_t("position: ");

    std::size_t result = get_sequence_input_stream_record_(id)->stream->seek(index);
    std::cout << "new position = " << result << "\n";
}

void ConsoleUI::show_sequence_input_stream_position_() {
    int id = choose_sequence_input_stream_id_();
    std::cout << "position = " << get_sequence_input_stream_record_(id)->stream->get_position() << "\n";
}

void ConsoleUI::show_sequence_input_stream_end_state_() {
    int id = choose_sequence_input_stream_id_();
    bool is_end = get_sequence_input_stream_record_(id)->stream->is_end_of_stream();
    std::cout << (is_end ? "end of stream" : "not end of stream") << "\n";
}

void ConsoleUI::delete_sequence_input_stream_() {
    int id = choose_sequence_input_stream_id_();
    SequenceInputStreamRecord* record = get_sequence_input_stream_record_(id);

    if (record->stream->is_open()) {
        record->stream->close();
    }

    delete record->stream;
    record->stream = nullptr;
    record->is_deleted = true;

    std::cout << "sequence input stream #" << id << " deleted\n";
}

void ConsoleUI::create_lazy_input_stream_() {
    int id = choose_lazy_id_();
    LazySequenceRecord* record = get_lazy_record_(id);

    LazySequenceInputStream<int>* stream = new LazySequenceInputStream<int>(record->sequence);

    std::ostringstream description;
    description << "lazy input stream over lazy sequence #" << id;

    int stream_id = add_lazy_input_stream_record_(stream, id, description.str());
    std::cout << "created lazy input stream #" << stream_id << "\n";
}

void ConsoleUI::list_lazy_input_streams_() const {
    std::cout << "\n=== lazy sequence input streams ===\n";

    for (int i = 0; i < lazy_input_stream_records_.get_size(); ++i) {
        LazyInputStreamRecord* record = lazy_input_stream_records_.get(i);

        std::cout << "#" << i << " ";

        if (record == nullptr || record->is_deleted || record->stream == nullptr) {
            std::cout << "[deleted]\n";
            continue;
        }

        std::cout << record->description
                  << ", source = lazy sequence #" << record->source_lazy_id
                  << ", " << (record->stream->is_open() ? "open" : "closed");

        if (record->stream->is_open()) {
            std::cout << ", ordinal position = " << record->stream->get_ordinal_position();
        }

        std::cout << "\n";
    }
}

void ConsoleUI::open_lazy_input_stream_() {
    int id = choose_lazy_input_stream_id_();
    get_lazy_input_stream_record_(id)->stream->open();
    std::cout << "lazy input stream opened\n";
}

void ConsoleUI::close_lazy_input_stream_() {
    int id = choose_lazy_input_stream_id_();
    get_lazy_input_stream_record_(id)->stream->close();
    std::cout << "lazy input stream closed\n";
}

void ConsoleUI::reset_lazy_input_stream_() {
    int id = choose_lazy_input_stream_id_();
    get_lazy_input_stream_record_(id)->stream->reset();
    std::cout << "lazy input stream reset\n";
}

void ConsoleUI::read_one_from_lazy_input_stream_() {
    int id = choose_lazy_input_stream_id_();
    int value = get_lazy_input_stream_record_(id)->stream->input();
    std::cout << "value = " << value << "\n";
}

void ConsoleUI::read_many_from_lazy_input_stream_() {
    int id = choose_lazy_input_stream_id_();
    std::size_t count = read_size_t("count: ");

    LazySequenceInputStream<int>* stream = get_lazy_input_stream_record_(id)->stream;

    std::cout << "[";

    for (std::size_t i = 0; i < count; ++i) {
        if (stream->is_end_of_stream()) {
            break;
        }

        if (i > 0) {
            std::cout << ", ";
        }

        std::cout << stream->input();
    }

    std::cout << "]\n";
}

void ConsoleUI::seek_lazy_input_stream_finite_() {
    int id = choose_lazy_input_stream_id_();
    std::size_t index = read_size_t("finite position: ");

    std::size_t result = get_lazy_input_stream_record_(id)->stream->seek(index);
    std::cout << "new finite position = " << result << "\n";
}

void ConsoleUI::seek_lazy_input_stream_ordinal_() {
    int id = choose_lazy_input_stream_id_();
    Ordinal index = read_ordinal_index("choose ordinal position");

    Ordinal result = get_lazy_input_stream_record_(id)->stream->seek(index);
    std::cout << "new ordinal position = " << result << "\n";
}

void ConsoleUI::show_lazy_input_stream_finite_position_() {
    int id = choose_lazy_input_stream_id_();
    std::cout << "finite position = " << get_lazy_input_stream_record_(id)->stream->get_position() << "\n";
}

void ConsoleUI::show_lazy_input_stream_ordinal_position_() {
    int id = choose_lazy_input_stream_id_();
    std::cout << "ordinal position = " << get_lazy_input_stream_record_(id)->stream->get_ordinal_position() << "\n";
}

void ConsoleUI::show_lazy_input_stream_end_state_() {
    int id = choose_lazy_input_stream_id_();
    bool is_end = get_lazy_input_stream_record_(id)->stream->is_end_of_stream();
    std::cout << (is_end ? "end of stream" : "not end of stream") << "\n";
}

void ConsoleUI::delete_lazy_input_stream_() {
    int id = choose_lazy_input_stream_id_();
    LazyInputStreamRecord* record = get_lazy_input_stream_record_(id);

    if (record->stream->is_open()) {
        record->stream->close();
    }

    delete record->stream;
    record->stream = nullptr;
    record->is_deleted = true;

    std::cout << "lazy input stream #" << id << " deleted\n";
}

void ConsoleUI::create_sequence_output_stream_() {
    int id = choose_sequence_id_();
    SequenceRecord* record = get_sequence_record_(id);

    SequenceOutputStream<int>* stream = new SequenceOutputStream<int>(record->sequence);

    std::ostringstream description;
    description << "sequence output stream over ordinary sequence #" << id;

    int stream_id = add_sequence_output_stream_record_(stream, id, description.str());
    std::cout << "created sequence output stream #" << stream_id << "\n";
}

void ConsoleUI::list_sequence_output_streams_() const {
    std::cout << "\n=== sequence output streams ===\n";

    for (int i = 0; i < sequence_output_stream_records_.get_size(); ++i) {
        SequenceOutputStreamRecord* record = sequence_output_stream_records_.get(i);

        std::cout << "#" << i << " ";

        if (record == nullptr || record->is_deleted || record->stream == nullptr) {
            std::cout << "[deleted]\n";
            continue;
        }

        std::cout << record->description
                  << ", target = ordinary sequence #" << record->target_sequence_id
                  << ", " << (record->released ? "released" : "not released")
                  << ", " << (record->stream->is_open() ? "open" : "closed");

        if (record->stream->is_open()) {
            std::cout << ", position = " << record->stream->get_position();
        }

        std::cout << "\n";
    }
}

void ConsoleUI::open_sequence_output_stream_() {
    int id = choose_sequence_output_stream_id_();
    SequenceOutputStreamRecord* record = get_sequence_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("sequence output stream is already released");
    }

    record->stream->open();
    std::cout << "sequence output stream opened\n";
}

void ConsoleUI::close_sequence_output_stream_() {
    int id = choose_sequence_output_stream_id_();
    get_sequence_output_stream_record_(id)->stream->close();
    std::cout << "sequence output stream closed\n";
}

void ConsoleUI::write_one_to_sequence_output_stream_() {
    int id = choose_sequence_output_stream_id_();
    SequenceOutputStreamRecord* record = get_sequence_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("sequence output stream is already released");
    }

    int value = read_int("value: ");
    std::size_t position = record->stream->output(value);
    std::cout << "new position = " << position << "\n";
}

void ConsoleUI::write_many_to_sequence_output_stream_() {
    int id = choose_sequence_output_stream_id_();
    SequenceOutputStreamRecord* record = get_sequence_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("sequence output stream is already released");
    }

    std::size_t count = read_size_t("count: ");

    for (std::size_t i = 0; i < count; ++i) {
        std::ostringstream prompt;
        prompt << "value[" << i << "]: ";
        int value = read_int(prompt.str());
        record->stream->output(value);
    }

    std::cout << "new position = " << record->stream->get_position() << "\n";
}

void ConsoleUI::show_sequence_output_stream_position_() {
    int id = choose_sequence_output_stream_id_();
    SequenceOutputStreamRecord* record = get_sequence_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("sequence output stream is already released");
    }

    std::cout << "position = " << record->stream->get_position() << "\n";
}

void ConsoleUI::release_sequence_output_stream_sequence_() {
    int id = choose_sequence_output_stream_id_();
    SequenceOutputStreamRecord* record = get_sequence_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("sequence output stream is already released");
    }

    Sequence<int>* released = record->stream->release_sequence();
    record->released = true;

    SequenceRecord* target = get_sequence_record_(record->target_sequence_id);

    if (released == target->sequence) {
        std::cout << "released original ordinary sequence; registry object was updated in place or unchanged\n";
        return;
    }

    std::ostringstream description;
    description << "released from sequence output stream #" << id << ", based on ordinary sequence #" << record->target_sequence_id;

    int result_id = add_sequence_record_(released, target->container_kind, target->mutability, description.str());
    std::cout << "released sequence added as ordinary sequence #" << result_id << "\n";
}

void ConsoleUI::delete_sequence_output_stream_() {
    int id = choose_sequence_output_stream_id_();
    SequenceOutputStreamRecord* record = get_sequence_output_stream_record_(id);

    if (record->stream->is_open()) {
        record->stream->close();
    }

    delete record->stream;
    record->stream = nullptr;
    record->is_deleted = true;

    std::cout << "sequence output stream #" << id << " deleted\n";
}

void ConsoleUI::create_lazy_output_stream_() {
    int id = choose_lazy_id_();
    LazySequenceRecord* record = get_lazy_record_(id);

    LazySequenceOutputStream<int>* stream = new LazySequenceOutputStream<int>(record->sequence);

    std::ostringstream description;
    description << "lazy output stream over lazy sequence #" << id;

    int stream_id = add_lazy_output_stream_record_(stream, id, description.str());
    std::cout << "created lazy output stream #" << stream_id << "\n";
}

void ConsoleUI::list_lazy_output_streams_() const {
    std::cout << "\n=== lazy sequence output streams ===\n";

    for (int i = 0; i < lazy_output_stream_records_.get_size(); ++i) {
        LazyOutputStreamRecord* record = lazy_output_stream_records_.get(i);

        std::cout << "#" << i << " ";

        if (record == nullptr || record->is_deleted || record->stream == nullptr) {
            std::cout << "[deleted]\n";
            continue;
        }

        std::cout << record->description
                  << ", target = lazy sequence #" << record->target_lazy_id
                  << ", " << (record->released ? "released" : "not released")
                  << ", " << (record->stream->is_open() ? "open" : "closed");

        if (record->stream->is_open()) {
            std::cout << ", ordinal position = " << record->stream->get_ordinal_position();
        }

        std::cout << "\n";
    }
}

void ConsoleUI::open_lazy_output_stream_() {
    int id = choose_lazy_output_stream_id_();
    LazyOutputStreamRecord* record = get_lazy_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("lazy output stream is already released");
    }

    record->stream->open();
    std::cout << "lazy output stream opened\n";
}

void ConsoleUI::close_lazy_output_stream_() {
    int id = choose_lazy_output_stream_id_();
    get_lazy_output_stream_record_(id)->stream->close();
    std::cout << "lazy output stream closed\n";
}

void ConsoleUI::write_one_to_lazy_output_stream_() {
    int id = choose_lazy_output_stream_id_();
    LazyOutputStreamRecord* record = get_lazy_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("lazy output stream is already released");
    }

    int value = read_int("value: ");
    Ordinal position = record->stream->output_ordinal(value);
    std::cout << "new ordinal position = " << position << "\n";
}

void ConsoleUI::write_many_to_lazy_output_stream_() {
    int id = choose_lazy_output_stream_id_();
    LazyOutputStreamRecord* record = get_lazy_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("lazy output stream is already released");
    }

    std::size_t count = read_size_t("count: ");

    for (std::size_t i = 0; i < count; ++i) {
        std::ostringstream prompt;
        prompt << "value[" << i << "]: ";
        int value = read_int(prompt.str());
        record->stream->output_ordinal(value);
    }

    std::cout << "new ordinal position = " << record->stream->get_ordinal_position() << "\n";
}

void ConsoleUI::show_lazy_output_stream_finite_position_() {
    int id = choose_lazy_output_stream_id_();
    LazyOutputStreamRecord* record = get_lazy_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("lazy output stream is already released");
    }

    std::cout << "finite position = " << record->stream->get_position() << "\n";
}

void ConsoleUI::show_lazy_output_stream_ordinal_position_() {
    int id = choose_lazy_output_stream_id_();
    LazyOutputStreamRecord* record = get_lazy_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("lazy output stream is already released");
    }

    std::cout << "ordinal position = " << record->stream->get_ordinal_position() << "\n";
}

void ConsoleUI::release_lazy_output_stream_sequence_() {
    int id = choose_lazy_output_stream_id_();
    LazyOutputStreamRecord* record = get_lazy_output_stream_record_(id);

    if (record->released) {
        throw std::logic_error("lazy output stream is already released");
    }

    LazySequence<int>* released = record->stream->release_sequence();
    record->released = true;

    LazySequenceRecord* target = get_lazy_record_(record->target_lazy_id);

    if (released == target->sequence) {
        std::cout << "released original lazy sequence; no new registry object added\n";
        return;
    }

    std::ostringstream description;
    description << "released from lazy output stream #" << id << ", based on lazy sequence #" << record->target_lazy_id;

    int result_id = add_lazy_record_(released, LazySequenceOrigin::OutputStreamRelease, description.str());
    std::cout << "released lazy sequence added as lazy sequence #" << result_id << "\n";
}

void ConsoleUI::delete_lazy_output_stream_() {
    int id = choose_lazy_output_stream_id_();
    LazyOutputStreamRecord* record = get_lazy_output_stream_record_(id);

    if (record->stream->is_open()) {
        record->stream->close();
    }

    delete record->stream;
    record->stream = nullptr;
    record->is_deleted = true;

    std::cout << "lazy output stream #" << id << " deleted\n";
}

void ConsoleUI::delete_ordinary_sequence_() {
    int id = choose_sequence_id_();

    if (ordinary_sequence_has_stream_dependency_(id)) {
        throw std::logic_error("ordinary sequence cannot be deleted while streams reference it");
    }

    SequenceRecord* record = get_sequence_record_(id);
    delete record->sequence;
    record->sequence = nullptr;
    record->is_deleted = true;

    std::cout << "ordinary sequence #" << id << " deleted\n";
}

void ConsoleUI::delete_lazy_sequence_() {
    int id = choose_lazy_id_();

    if (lazy_sequence_has_stream_dependency_(id)) {
        throw std::logic_error("lazy sequence cannot be deleted while streams reference it");
    }

    LazySequenceRecord* record = get_lazy_record_(id);
    delete record->sequence;
    record->sequence = nullptr;
    record->is_deleted = true;

    std::cout << "lazy sequence #" << id << " deleted\n";
}

void ConsoleUI::delete_smart_config_() {
    int id = choose_smart_config_id_();
    SmartConfigRecord* record = get_smart_config_record_(id);
    record->is_deleted = true;
    std::cout << "smart data config #" << id << " deleted\n";
}

int ConsoleUI::add_sequence_record_(Sequence<int>* sequence, SequenceContainerKind container_kind, SequenceMutability mutability, const std::string& description) {
    if (sequence == nullptr) {
        throw std::invalid_argument("cannot add null sequence record");
    }

    SequenceRecord* record = new SequenceRecord;
    record->sequence = sequence;
    record->container_kind = container_kind;
    record->mutability = mutability;
    record->is_deleted = false;
    record->description = description;

    sequence_records_.append(record);
    return sequence_records_.get_size() - 1;
}

int ConsoleUI::add_lazy_record_(LazySequence<int>* sequence, LazySequenceOrigin origin, const std::string& description) {
    if (sequence == nullptr) {
        throw std::invalid_argument("cannot add null lazy sequence record");
    }

    LazySequenceRecord* record = new LazySequenceRecord;
    record->sequence = sequence;
    record->origin = origin;
    record->is_deleted = false;
    record->description = description;

    lazy_records_.append(record);
    return lazy_records_.get_size() - 1;
}

int ConsoleUI::add_smart_config_record_(const SmartDataConfig& config, const std::string& description) {
    SmartConfigRecord* record = new SmartConfigRecord;
    record->config = config;
    record->is_deleted = false;
    record->description = description;

    smart_config_records_.append(record);
    return smart_config_records_.get_size() - 1;
}

int ConsoleUI::add_sequence_input_stream_record_(SequenceInputStream<int>* stream, int source_sequence_id, const std::string& description) {
    if (stream == nullptr) {
        throw std::invalid_argument("cannot add null sequence input stream record");
    }

    SequenceInputStreamRecord* record = new SequenceInputStreamRecord;
    record->stream = stream;
    record->source_sequence_id = source_sequence_id;
    record->is_deleted = false;
    record->description = description;

    sequence_input_stream_records_.append(record);
    return sequence_input_stream_records_.get_size() - 1;
}

int ConsoleUI::add_lazy_input_stream_record_(LazySequenceInputStream<int>* stream, int source_lazy_id, const std::string& description) {
    if (stream == nullptr) {
        throw std::invalid_argument("cannot add null lazy input stream record");
    }

    LazyInputStreamRecord* record = new LazyInputStreamRecord;
    record->stream = stream;
    record->source_lazy_id = source_lazy_id;
    record->is_deleted = false;
    record->description = description;

    lazy_input_stream_records_.append(record);
    return lazy_input_stream_records_.get_size() - 1;
}

int ConsoleUI::add_sequence_output_stream_record_(SequenceOutputStream<int>* stream, int target_sequence_id, const std::string& description) {
    if (stream == nullptr) {
        throw std::invalid_argument("cannot add null sequence output stream record");
    }

    SequenceOutputStreamRecord* record = new SequenceOutputStreamRecord;
    record->stream = stream;
    record->target_sequence_id = target_sequence_id;
    record->released = false;
    record->is_deleted = false;
    record->description = description;

    sequence_output_stream_records_.append(record);
    return sequence_output_stream_records_.get_size() - 1;
}

int ConsoleUI::add_lazy_output_stream_record_(LazySequenceOutputStream<int>* stream, int target_lazy_id, const std::string& description) {
    if (stream == nullptr) {
        throw std::invalid_argument("cannot add null lazy output stream record");
    }

    LazyOutputStreamRecord* record = new LazyOutputStreamRecord;
    record->stream = stream;
    record->target_lazy_id = target_lazy_id;
    record->released = false;
    record->is_deleted = false;
    record->description = description;

    lazy_output_stream_records_.append(record);
    return lazy_output_stream_records_.get_size() - 1;
}

SequenceRecord* ConsoleUI::get_sequence_record_(int id) const {
    if (id < 0 || id >= sequence_records_.get_size()) {
        throw std::out_of_range("ordinary sequence id is out of range");
    }

    SequenceRecord* record = sequence_records_.get(id);

    if (record == nullptr || record->is_deleted || record->sequence == nullptr) {
        throw std::logic_error("ordinary sequence record is deleted");
    }

    return record;
}

LazySequenceRecord* ConsoleUI::get_lazy_record_(int id) const {
    if (id < 0 || id >= lazy_records_.get_size()) {
        throw std::out_of_range("lazy sequence id is out of range");
    }

    LazySequenceRecord* record = lazy_records_.get(id);

    if (record == nullptr || record->is_deleted || record->sequence == nullptr) {
        throw std::logic_error("lazy sequence record is deleted");
    }

    return record;
}

SmartConfigRecord* ConsoleUI::get_smart_config_record_(int id) const {
    if (id < 0 || id >= smart_config_records_.get_size()) {
        throw std::out_of_range("smart config id is out of range");
    }

    SmartConfigRecord* record = smart_config_records_.get(id);

    if (record == nullptr || record->is_deleted) {
        throw std::logic_error("smart config record is deleted");
    }

    return record;
}

SequenceInputStreamRecord* ConsoleUI::get_sequence_input_stream_record_(int id) const {
    if (id < 0 || id >= sequence_input_stream_records_.get_size()) {
        throw std::out_of_range("sequence input stream id is out of range");
    }

    SequenceInputStreamRecord* record = sequence_input_stream_records_.get(id);

    if (record == nullptr || record->is_deleted || record->stream == nullptr) {
        throw std::logic_error("sequence input stream record is deleted");
    }

    return record;
}

LazyInputStreamRecord* ConsoleUI::get_lazy_input_stream_record_(int id) const {
    if (id < 0 || id >= lazy_input_stream_records_.get_size()) {
        throw std::out_of_range("lazy input stream id is out of range");
    }

    LazyInputStreamRecord* record = lazy_input_stream_records_.get(id);

    if (record == nullptr || record->is_deleted || record->stream == nullptr) {
        throw std::logic_error("lazy input stream record is deleted");
    }

    return record;
}

SequenceOutputStreamRecord* ConsoleUI::get_sequence_output_stream_record_(int id) const {
    if (id < 0 || id >= sequence_output_stream_records_.get_size()) {
        throw std::out_of_range("sequence output stream id is out of range");
    }

    SequenceOutputStreamRecord* record = sequence_output_stream_records_.get(id);

    if (record == nullptr || record->is_deleted || record->stream == nullptr) {
        throw std::logic_error("sequence output stream record is deleted");
    }

    return record;
}

LazyOutputStreamRecord* ConsoleUI::get_lazy_output_stream_record_(int id) const {
    if (id < 0 || id >= lazy_output_stream_records_.get_size()) {
        throw std::out_of_range("lazy output stream id is out of range");
    }

    LazyOutputStreamRecord* record = lazy_output_stream_records_.get(id);

    if (record == nullptr || record->is_deleted || record->stream == nullptr) {
        throw std::logic_error("lazy output stream record is deleted");
    }

    return record;
}

int ConsoleUI::choose_sequence_id_() const {
    list_ordinary_sequences_();
    int id = read_int("ordinary sequence id: ");
    get_sequence_record_(id);
    return id;
}

int ConsoleUI::choose_lazy_id_() const {
    list_lazy_sequences_();
    int id = read_int("lazy sequence id: ");
    get_lazy_record_(id);
    return id;
}

int ConsoleUI::choose_smart_config_id_() const {
    list_smart_data_configs_();
    int id = read_int("smart data config id: ");
    get_smart_config_record_(id);
    return id;
}

int ConsoleUI::choose_sequence_input_stream_id_() const {
    list_sequence_input_streams_();
    int id = read_int("sequence input stream id: ");
    get_sequence_input_stream_record_(id);
    return id;
}

int ConsoleUI::choose_lazy_input_stream_id_() const {
    list_lazy_input_streams_();
    int id = read_int("lazy input stream id: ");
    get_lazy_input_stream_record_(id);
    return id;
}

int ConsoleUI::choose_sequence_output_stream_id_() const {
    list_sequence_output_streams_();
    int id = read_int("sequence output stream id: ");
    get_sequence_output_stream_record_(id);
    return id;
}

int ConsoleUI::choose_lazy_output_stream_id_() const {
    list_lazy_output_streams_();
    int id = read_int("lazy output stream id: ");
    get_lazy_output_stream_record_(id);
    return id;
}

void ConsoleUI::handle_ordinary_sequence_result_(int source_id, Sequence<int>* result, const std::string& operation_description) {
    if (result == nullptr) {
        throw std::runtime_error("sequence operation returned null");
    }

    SequenceRecord* source = get_sequence_record_(source_id);

    if (result == source->sequence) {
        std::cout << "sequence #" << source_id << " updated in place\n";
        return;
    }

    std::ostringstream description;
    description << operation_description << " #" << source_id;

    int result_id = add_sequence_record_(result, source->container_kind, source->mutability, description.str());
    std::cout << "created ordinary sequence #" << result_id << "\n";
}

bool ConsoleUI::ordinary_sequence_has_stream_dependency_(int id) const {
    for (int i = 0; i < sequence_input_stream_records_.get_size(); ++i) {
        SequenceInputStreamRecord* record = sequence_input_stream_records_.get(i);

        if (record != nullptr && !record->is_deleted && record->stream != nullptr && record->source_sequence_id == id) {
            return true;
        }
    }

    for (int i = 0; i < sequence_output_stream_records_.get_size(); ++i) {
        SequenceOutputStreamRecord* record = sequence_output_stream_records_.get(i);

        if (record != nullptr && !record->is_deleted && record->stream != nullptr && !record->released && record->target_sequence_id == id) {
            return true;
        }
    }

    return false;
}

bool ConsoleUI::lazy_sequence_has_stream_dependency_(int id) const {
    for (int i = 0; i < lazy_input_stream_records_.get_size(); ++i) {
        LazyInputStreamRecord* record = lazy_input_stream_records_.get(i);

        if (record != nullptr && !record->is_deleted && record->stream != nullptr && record->source_lazy_id == id) {
            return true;
        }
    }

    for (int i = 0; i < lazy_output_stream_records_.get_size(); ++i) {
        LazyOutputStreamRecord* record = lazy_output_stream_records_.get(i);

        if (record != nullptr && !record->is_deleted && record->stream != nullptr && !record->released && record->target_lazy_id == id) {
            return true;
        }
    }

    return false;
}

std::string ConsoleUI::sequence_record_title_(const SequenceRecord* record) const {
    std::ostringstream out;
    out << to_string(record->mutability) << " " << to_string(record->container_kind) << " sequence";
    return out.str();
}

std::string ConsoleUI::smart_config_to_string_(const SmartDataConfig& config) const {
    std::ostringstream out;

    if (config.distribution_type == SmartDataDistributionType::Uniform) {
        out << "uniform";
    } else {
        out << "normal";
    }

    out << ", seed = " << config.seed;
    out << ", range = [" << config.min_value << ", " << config.max_value << "]";
    out << ", length = " << config.length;

    if (config.distribution_type == SmartDataDistributionType::Normal) {
        out << ", mean = " << config.mean;
        out << ", stddev = " << config.standard_deviation;
    }

    return out.str();
}

std::string ConsoleUI::lazy_length_to_string_(const LazySequence<int>* sequence) const {
    if (sequence == nullptr) {
        return "null";
    }

    return ordinal_to_string(sequence->get_ordinal_length());
}

int* ConsoleUI::read_int_array_(int count) const {
    if (count < 0) {
        throw std::invalid_argument("array count cannot be negative");
    }

    if (count == 0) {
        return nullptr;
    }

    int* items = new int[count];

    try {
        for (int i = 0; i < count; ++i) {
            std::ostringstream prompt;
            prompt << "item[" << i << "]: ";
            items[i] = read_int(prompt.str());
        }
    } catch (...) {
        delete[] items;
        throw;
    }

    return items;
}

void ConsoleUI::print_sequence_prefix_(const Sequence<int>* sequence, std::size_t count) const {
    if (sequence == nullptr) {
        throw std::invalid_argument("sequence is null");
    }

    int size = sequence->get_size();
    std::size_t limit = count < static_cast<std::size_t>(size) ? count : static_cast<std::size_t>(size);

    std::cout << "[";

    for (std::size_t i = 0; i < limit; ++i) {
        if (i > 0) {
            std::cout << ", ";
        }

        std::cout << sequence->get(static_cast<int>(i));
    }

    if (limit < static_cast<std::size_t>(size)) {
        std::cout << ", ...";
    }

    std::cout << "]\n";
}

void ConsoleUI::print_lazy_prefix_(const LazySequence<int>* sequence, std::size_t count) const {
    if (sequence == nullptr) {
        throw std::invalid_argument("lazy sequence is null");
    }

    std::cout << "[";

    for (std::size_t i = 0; i < count; ++i) {
        if (i > 0) {
            std::cout << ", ";
        }

        std::cout << sequence->get(Ordinal(i));
    }

    std::cout << "]\n";
}

void ConsoleUI::cleanup_() {
    for (int i = 0; i < sequence_input_stream_records_.get_size(); ++i) {
        SequenceInputStreamRecord* record = sequence_input_stream_records_.get(i);

        if (record != nullptr) {
            delete record->stream;
            record->stream = nullptr;
            delete record;
        }
    }

    for (int i = 0; i < lazy_input_stream_records_.get_size(); ++i) {
        LazyInputStreamRecord* record = lazy_input_stream_records_.get(i);

        if (record != nullptr) {
            delete record->stream;
            record->stream = nullptr;
            delete record;
        }
    }

    for (int i = 0; i < sequence_output_stream_records_.get_size(); ++i) {
        SequenceOutputStreamRecord* record = sequence_output_stream_records_.get(i);

        if (record != nullptr) {
            delete record->stream;
            record->stream = nullptr;
            delete record;
        }
    }

    for (int i = 0; i < lazy_output_stream_records_.get_size(); ++i) {
        LazyOutputStreamRecord* record = lazy_output_stream_records_.get(i);

        if (record != nullptr) {
            delete record->stream;
            record->stream = nullptr;
            delete record;
        }
    }

    for (int i = 0; i < lazy_records_.get_size(); ++i) {
        LazySequenceRecord* record = lazy_records_.get(i);

        if (record != nullptr) {
            delete record->sequence;
            record->sequence = nullptr;
            delete record;
        }
    }

    for (int i = 0; i < sequence_records_.get_size(); ++i) {
        SequenceRecord* record = sequence_records_.get(i);

        if (record != nullptr) {
            delete record->sequence;
            record->sequence = nullptr;
            delete record;
        }
    }

    for (int i = 0; i < smart_config_records_.get_size(); ++i) {
        SmartConfigRecord* record = smart_config_records_.get(i);

        if (record != nullptr) {
            delete record;
        }
    }
}
