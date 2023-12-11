#include <iostream>
#include <fstream>
#include <string_view>

#include "corpus_manager.h"
#include "document.h"
#include "glog/logging.h"
#include "searcher.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << "[corpus path]!" << std::endl;
        return -1;
    }
    google::InitGoogleLogging(argv[0]);
    namespace ps = planet_scale;
    std::string_view corpus_dir = argv[1];
    auto corpus_mgr = std::make_unique<ps::CorpusManager>();
    if (corpus_mgr->ProcessNewDocument(corpus_dir) != 0) {
        LOG(ERROR) << "add corpus failed" << std::endl;
        return -1;
    }
    std::unique_ptr<planet_scale::Searcher> searcher = 
        std::make_unique<planet_scale::Searcher>(corpus_mgr->GetFullTextIndex(), corpus_mgr.get());
    std::string query;
    std::cout << "ready to read query" << std::endl;
    while (std::cin >> query) {
        std::cout << "Query result: " << std::endl;
        std::vector<const planet_scale::Document*> result = searcher->Query(query, 10);
        std::cout << "query result size = " << result.size() << std::endl;
        for (size_t i = 0; i < result.size(); ++i) {
            std::cout << "time : " << result[i]->timestamp << ", url : " << result[i]->url << std::endl;
        }
    }

    return 0;
}