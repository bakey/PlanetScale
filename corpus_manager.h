#pragma once
#include "cppjieba/Jieba.hpp"
#include "document.h"
#include "filter.h"
#include "searcher.h"
#include "uchardet.h"

namespace planet_scale {
class CorpusManager {
public:
    enum {
        SUCCESS = 0,
        FILE_NOT_EXIST = 1,
        FILE_OPEN_ERROR = 2,
        ENCODING_DETEC_FAILED = 3,
        JSON_PARSE_FAILED = 4,
    };
    CorpusManager() {
        const char* const DICT_PATH = "contrib/cppjieba/dict/jieba.dict.utf8";
        const char* const HMM_PATH = "contrib/cppjieba/dict/hmm_model.utf8";
        const char* const USER_DICT_PATH = "contrib/cppjieba/dict/user.dict.utf8";
        const char* const IDF_PATH = "contrib/cppjieba/dict/idf.utf8";
        const char* const STOP_WORD_PATH = "contrib/cppjieba/dict/stop_words.utf8";
        filter_ = std::make_unique<Filter>();
        enc_dector_ = std::make_unique<uchardet_t>(uchardet_new());
        term_splitter_ = std::make_unique<cppjieba::Jieba>(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);
    }
    int ProcessNewDocument(std::string_view file_path);
    int AddCorpus(std::string_view file_path);
    const Document* GetDocument(std::string_view doc_id) const {
        auto doc_it = document_index_.find(std::string(doc_id));
        if (doc_it == document_index_.end()) {
            return nullptr;
        }
        return doc_it->second.get();
    }
    const std::unordered_map<std::string, std::vector<std::string>>* GetFullTextIndex() const {
        return &text_index_;
    }
private:
    int extractDocument(std::string_view content, std::unique_ptr<Document>& document);
private:
    std::unique_ptr<Filter> filter_;
    std::unique_ptr<uchardet_t> enc_dector_;
    std::unordered_map<std::string, std::vector<std::string>> text_index_;
    std::unordered_map<std::string, std::unique_ptr<Document>> document_index_;
    std::unique_ptr<cppjieba::Jieba> term_splitter_;
};

} // namespace planet_scale