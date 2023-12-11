#pragma once
#include "corpus_manager.h"
#include "document.h"

namespace planet_scale {
class CorpusManager;
class Searcher {
public:
    Searcher(const std::unordered_map<std::string, std::vector<std::string>>* text_index, const CorpusManager* corpus_mgr)
        :text_index_(text_index), corpus_manager_(corpus_mgr) {}
    Searcher(const std::unordered_map<std::string, std::vector<std::string>>* text_index)
        :text_index_(text_index) {}
    std::vector<const Document*> Query(std::string_view query, int limit);
private:
    const std::unordered_map<std::string, std::vector<std::string>>* text_index_;
    const CorpusManager* corpus_manager_;
};
} // namespace planet_scale