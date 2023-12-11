#pragma once
#include <string>
namespace planet_scale {
struct Document {
    Document(const std::string& text, const std::string& ts, const std::string& u, const std::string& did)
        :text(text), timestamp(ts), url(u), doc_id(did) {}
    std::string text;
    std::string timestamp;
    std::string url;
    std::string doc_id;
};
} // namespace planet_scale