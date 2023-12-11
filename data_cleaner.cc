#include "data_cleaner.h"
#include "utf8.h"

namespace planet_scale {
std::string DataCleaner::CleanDataEncoding(std::string_view content) {
    std::string cleaned_data;
    return utf8::replace_invalid(content.begin(), content.end(), back_inserter(cleaned_data));
}

} // namespace planet_scale
