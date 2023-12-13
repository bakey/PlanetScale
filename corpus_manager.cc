#include "corpus_manager.h"

#include <random>

#include "document.h"
#include "glog/logging.h"
#include "nlohmann/json.hpp"

namespace planet_scale {
using nlohmann::json;
int CorpusManager::AddCorpus(std::string_view file_path) {
    const fs::path path(file_path);
    std::error_code ec;
    std::error_condition ok;
    if (fs::is_directory(path, ec); ec == ok) { 
        for (const auto & entry : fs::directory_iterator(path)) {
            if (int process_res = ProcessNewDocument(entry.path().string()); process_res != SUCCESS) {
                return process_res;
            }
        }
    }
    return SUCCESS;
}
int CorpusManager::extractDocument(std::string_view content, std::unique_ptr<Document>& document) {
    nlohmann::json docs;
    try {
        docs = nlohmann::json::parse(content);
    } catch(const std::exception& ex) {
        LOG(ERROR) << "parse document got exception: " << ex.what();
        return JSON_PARSE_FAILED;
    }
    std::random_device rdevice;
    std::uniform_int_distribution<uint64_t> distribution;
    document = std::make_unique<Document>(docs["text"].template get<std::string>(), docs["timestamp"].template get<std::string>(), 
                docs["url"].template get<std::string>(), std::to_string(distribution(rdevice)));
    return SUCCESS;
}

int CorpusManager::detectEncoding(const fs::path& doc_file, std::string& encoding) {
    std::ifstream doc_file_stream(doc_file, std::ios::in | std::ios::binary);
    if (!doc_file_stream.is_open()) {
        LOG(ERROR) << "file open filed : " << doc_file.string();
        return FILE_OPEN_ERROR;
    }
    std::uintmax_t file_size = std::__fs::filesystem::file_size(doc_file);
    constexpr static int kReadBufferSize = 1024;
    char buffer[kReadBufferSize];
    std::string file_content;
    file_content.reserve(file_size);
    while (doc_file_stream.read(buffer, kReadBufferSize)) {
        file_content.append(buffer);
        int det_res = uchardet_handle_data(*(enc_dector_.get()), file_content.data(), file_content.length());
        if (det_res != 0) {
            LOG(ERROR) << "Encoding detection failed, error : " << det_res;
            return ENCODING_DETEC_FAILED;
        }
        if (const char* charset = uchardet_get_charset(*enc_dector_.get()); charset != nullptr) {
            LOG(INFO) << "get charset success : " << charset << " of " << file_content.size();
            uchardet_data_end(*enc_dector_.get());
            break;
        }
    }
    return SUCCESS;
}

int CorpusManager::ProcessNewDocument(std::string_view file_path) {
    fs::path doc_file(file_path);
    if (!fs::exists(doc_file)) {
        LOG(ERROR) << "file: " << file_path << " not exist";
        return FILE_NOT_EXIST;
    }
    std::string encoding;
    if (detectEncoding(doc_file, encoding) != SUCCESS) {
        LOG(WARNING) << "detect encoding failed";
    }
    LOG(INFO) << "detect encoding = " << encoding;
    std::ifstream doc_file_stream(doc_file, std::ios::in | std::ios::binary);
    if (!doc_file_stream.is_open()) {
        LOG(ERROR) << "file open filed : " << file_path;
        return FILE_OPEN_ERROR;
    }
    std::string line;
    LOG(INFO) << "Building full text index ...";
    int procssed_lines = 0;
    while (std::getline(doc_file_stream, line)) {
        std::unique_ptr<Document> document;
        int extract_res = extractDocument(line, document);
        if (extract_res != SUCCESS) {
            return extract_res;
        }
       
        //LOG(INFO) << "detec encoding = " << uchardet_get_charset(*enc_dector_.get());
        std::vector<std::string> words;
        term_splitter_->Cut(document->text, words, true);
        for (const auto& w : words) {
            auto key_it = text_index_.find(w);
            if (key_it == text_index_.end()) {
                text_index_[w] = std::vector<std::string>{document->doc_id};
            } else {
                key_it->second.emplace_back(document->doc_id);
            }
        }
        document_index_[document->doc_id] = std::move(document);
        if (++ procssed_lines > 100) break;
        //std::cout << limonp::Join(words.begin(), words.end(), "/") << std::endl;
        //break;
    }
    LOG(INFO) << "Total added " << text_index_.size() << std::endl;
    return SUCCESS;

    /*std::string content{std::istreambuf_iterator<char>(doc_file_stream), std::istreambuf_iterator<char>()};
    std::vector<std::unique_ptr<Document>> documents;
    int extract_res = extractDocument(content, documents);
    if (extract_res != SUCCESS) {
        return extract_res;
    }
    LOG(INFO) << "extract " << documents.size() << " documents from file";
    
    uchardet_data_end(*(enc_dector_.get()));
    size_t enc_candidate = 3;
    LOG(INFO) << "detec encoding = " << uchardet_get_charset(*enc_dector_.get());
    return SUCCESS;*/
}
} // namespace planet_scale