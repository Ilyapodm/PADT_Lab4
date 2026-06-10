#pragma once

#include <string>

#include "adt_lab_2/sequence.hpp"
#include "lazy_sequence/lazy_sequence.hpp"
#include "streams/sequence_input_stream.hpp"
#include "streams/sequence_output_stream.hpp"
#include "streams/lazy_sequence_input_stream.hpp"
#include "streams/lazy_sequence_output_stream.hpp"
#include "smart_data/smart_data_config.hpp"

// metadata records are ui-level objects; they do not affect sequence semantics

enum class SequenceContainerKind {
    Array,
    List
};

enum class SequenceMutability {
    Mutable,
    Immutable
};

enum class LazySequenceOrigin {
    Empty,
    ArrayInput,
    SequenceSnapshot,
    FunctionPreset,
    RecurrencePreset,
    SmartDataConfig,
    Append,
    Prepend,
    InsertAt,
    Set,
    RemoveAt,
    Subsequence,
    Concat,
    OutputStreamRelease
};

struct SequenceRecord {
    Sequence<int>* sequence;
    SequenceContainerKind container_kind;
    SequenceMutability mutability;
    bool is_deleted;
    std::string description;
};

struct LazySequenceRecord {
    LazySequence<int>* sequence;
    LazySequenceOrigin origin;
    bool is_deleted;
    std::string description;
};

struct SmartConfigRecord {
    SmartDataConfig config;
    bool is_deleted;
    std::string description;
};

struct SequenceInputStreamRecord {
    SequenceInputStream<int>* stream;
    int source_sequence_id;
    bool is_deleted;
    std::string description;
};

struct LazyInputStreamRecord {
    LazySequenceInputStream<int>* stream;
    int source_lazy_id;
    bool is_deleted;
    std::string description;
};

struct SequenceOutputStreamRecord {
    SequenceOutputStream<int>* stream;
    int target_sequence_id;
    bool released;
    bool is_deleted;
    std::string description;
};

struct LazyOutputStreamRecord {
    LazySequenceOutputStream<int>* stream;
    int target_lazy_id;
    bool released;
    bool is_deleted;
    std::string description;
};

inline const char* to_string(SequenceContainerKind value) {
    switch (value) {
        case SequenceContainerKind::Array:
            return "array";
        case SequenceContainerKind::List:
            return "list";
        default:
            return "unknown";
    }
}

inline const char* to_string(SequenceMutability value) {
    switch (value) {
        case SequenceMutability::Mutable:
            return "mutable";
        case SequenceMutability::Immutable:
            return "immutable";
        default:
            return "unknown";
    }
}