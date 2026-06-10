#pragma once

#include <cstddef>
#include <string>

#include "adt_lab_2/array_sequence.hpp"
#include "adt_lab_2/sequence.hpp"
#include "lazy_sequence/lazy_sequence.hpp"
#include "smart_data/smart_data_config.hpp"
#include "streams/sequence_input_stream.hpp"
#include "streams/sequence_output_stream.hpp"
#include "console/ui_records.hpp"

class ConsoleUI {
public:
    ConsoleUI();
    ~ConsoleUI();

    ConsoleUI(const ConsoleUI& other) = delete;
    ConsoleUI& operator=(const ConsoleUI& other) = delete;

    void run();

private:
    MutableArraySequence<SequenceRecord*> sequence_records_;
    MutableArraySequence<LazySequenceRecord*> lazy_records_;
    MutableArraySequence<SmartConfigRecord*> smart_config_records_;
    MutableArraySequence<InputStreamRecord*> input_stream_records_;
    MutableArraySequence<OutputStreamRecord*> output_stream_records_;

    void main_menu_();

    void ordinary_sequence_menu_();
    void lazy_sequence_menu_();
    void smart_data_menu_();
    void input_stream_menu_();
    void output_stream_menu_();
    void deletion_menu_();
    void planned_lazy_streams_menu_();

    void create_ordinary_sequence_();
    void list_ordinary_sequences_() const;
    void print_ordinary_sequence_prefix_();
    void get_ordinary_sequence_element_();
    void show_ordinary_sequence_size_();
    void append_to_ordinary_sequence_();
    void prepend_to_ordinary_sequence_();
    void insert_into_ordinary_sequence_();
    void set_ordinary_sequence_element_();
    void remove_ordinary_sequence_element_();
    void ordinary_sequence_subsequence_();
    void concat_ordinary_sequences_();

    void create_empty_lazy_sequence_();
    void create_lazy_sequence_from_array_();
    void create_lazy_sequence_from_sequence_();
    void create_lazy_sequence_from_function_preset_();
    void create_lazy_sequence_from_recurrence_preset_();
    void create_lazy_sequence_from_smart_config_();
    void list_lazy_sequences_() const;
    void print_lazy_sequence_prefix_();
    void get_lazy_sequence_finite_element_();
    void get_lazy_sequence_ordinal_element_();
    void show_lazy_sequence_info_();
    void append_to_lazy_sequence_();
    void prepend_to_lazy_sequence_();
    void insert_into_lazy_sequence_();
    void set_lazy_sequence_element_();
    void remove_lazy_sequence_element_();
    void lazy_sequence_subsequence_();
    void concat_lazy_sequences_();

    void create_smart_data_config_();
    void list_smart_data_configs_() const;
    void preview_smart_data_config_();

    void create_input_stream_();
    void list_input_streams_() const;
    void open_input_stream_();
    void close_input_stream_();
    void reset_input_stream_();
    void read_one_from_input_stream_();
    void read_many_from_input_stream_();
    void seek_input_stream_();
    void show_input_stream_position_();
    void show_input_stream_end_state_();

    void create_output_stream_();
    void list_output_streams_() const;
    void open_output_stream_();
    void close_output_stream_();
    void write_one_to_output_stream_();
    void write_many_to_output_stream_();
    void show_output_stream_position_();
    void release_output_stream_sequence_();

    void delete_ordinary_sequence_();
    void delete_lazy_sequence_();
    void delete_smart_config_();
    void delete_input_stream_();
    void delete_output_stream_();

    int add_sequence_record_(Sequence<int>* sequence, SequenceContainerKind container_kind, SequenceMutability mutability, const std::string& description);
    int add_lazy_record_(LazySequence<int>* sequence, LazySequenceOrigin origin, const std::string& description);
    int add_smart_config_record_(const SmartDataConfig& config, const std::string& description);
    int add_input_stream_record_(SequenceInputStream<int>* stream, StreamSourceKind source_kind, int source_id, const std::string& description);
    int add_output_stream_record_(SequenceOutputStream<int>* stream, StreamTargetKind target_kind, int target_id, const std::string& description);

    SequenceRecord* get_sequence_record_(int id) const;
    LazySequenceRecord* get_lazy_record_(int id) const;
    SmartConfigRecord* get_smart_config_record_(int id) const;
    InputStreamRecord* get_input_stream_record_(int id) const;
    OutputStreamRecord* get_output_stream_record_(int id) const;

    int choose_sequence_id_() const;
    int choose_lazy_id_() const;
    int choose_smart_config_id_() const;
    int choose_input_stream_id_() const;
    int choose_output_stream_id_() const;

    void handle_ordinary_sequence_result_(int source_id, Sequence<int>* result, const std::string& operation_description);
    bool ordinary_sequence_has_stream_dependency_(int id) const;
    bool lazy_sequence_has_stream_dependency_(int id) const;

    std::string sequence_record_title_(const SequenceRecord* record) const;
    std::string smart_config_to_string_(const SmartDataConfig& config) const;
    std::string lazy_length_to_string_(const LazySequence<int>* sequence) const;

    int* read_int_array_(int count) const;
    void print_sequence_prefix_(const Sequence<int>* sequence, std::size_t count) const;
    void print_lazy_prefix_(const LazySequence<int>* sequence, std::size_t count) const;

    void cleanup_();
};
