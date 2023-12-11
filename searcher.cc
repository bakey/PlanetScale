#include "searcher.h"
namespace planet_scale {

std::vector<const Document*> Searcher::Query(std::string_view query, int limit) {
    std::vector<const Document*> query_result;
    auto index_it = text_index_->find(std::string(query));
    if (index_it == text_index_->end()) {
        return query_result;
    }
    for (const auto& did : index_it->second) {
        const Document* d = corpus_manager_->GetDocument(did);
        std::cout << "document url : " << d->url << std::endl;
        query_result.emplace_back(d);
    }
    
    return query_result;
}
} // namespace planet_scale