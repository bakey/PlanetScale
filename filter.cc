#include "filter.h"
namespace planet_scale {
int Filter::exists(std::string_view doc) {
    uint64_t signature = 0;
    size_t distance = 0;
    if (!sim_hasher_->make(std::string(doc), distance, signature)) {
        return FILTER_ERROR;
    }
    if (distinct_filter_.count(signature) == 0) {
        return NOT_EXIST;
    }
    return EXIST;
}

int Filter::AddDocument(std::string_view doc) {
    uint64_t signature = 0;
    size_t distance = 0;
    if (!sim_hasher_->make(std::string(doc), distance, signature)) {
        return FILTER_ERROR;
    }
    auto insert_res = distinct_filter_.insert(signature);
    if (insert_res.second) {
        return ADD_DOC_FIRST;
    } else {
        return ADD_DOC_EXIST;
    }
}
} // namespace planet_scale