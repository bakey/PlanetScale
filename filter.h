#pragma once
#include "simhash/Simhasher.hpp"

namespace planet_scale {
struct Dicts {
    std::string file_dict;
    std::string file_hmm_model;
    std::string file_idf;
    std::string file_stop_words;
};
class Filter {
    enum FilterStatus {
        EXIST = 0,
        NOT_EXIST = 1,
        FILTER_ERROR = 2,

        ADD_DOC_EXIST = 10,
        ADD_DOC_FIRST = 11,
    };
public:
    explicit Filter() = default;
    explicit Filter(const Dicts& dicts) {
        sim_hasher_ = std::make_unique<simhash::Simhasher>(dicts.file_dict, dicts.file_hmm_model, dicts.file_idf, dicts.file_stop_words);
    }
    ~Filter() = default;
    int AddDocument(std::string_view doc);
private:
    int exists(std::string_view doc);
private:
    std::unique_ptr<simhash::Simhasher> sim_hasher_;
    std::unordered_set<uint64_t> distinct_filter_;
};
}