#pragma once

#include <string>

#include "adt_lab_2/sequence.hpp"
#include "lazy_sequence/lazy_sequence.hpp"
#include "streams/sequence_input_stream.hpp"
#include "streams/sequence_output_stream.hpp"
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

enum class StreamSourceKind {
    OrdinarySequence,
    LazySequence
};

enum class StreamTargetKind {
    OrdinarySequence,
    LazySequence
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

struct InputStreamRecord {
    SequenceInputStream<int>* stream;
    StreamSourceKind source_kind;
    int source_id;
    bool is_deleted;
    std::string description;
};

struct OutputStreamRecord {
    SequenceOutputStream<int>* stream;
    StreamTargetKind target_kind;
    int target_id;
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

inline const char* to_string(StreamSourceKind value) {
    switch (value) {
        case StreamSourceKind::OrdinarySequence:
            return "ordinary sequence";
        case StreamSourceKind::LazySequence:
            return "lazy sequence";
        default:
            return "unknown";
    }
}

inline const char* to_string(StreamTargetKind value) {
    switch (value) {
        case StreamTargetKind::OrdinarySequence:
            return "ordinary sequence";
        case StreamTargetKind::LazySequence:
            return "lazy sequence";
        default:
            return "unknown";
    }
}
