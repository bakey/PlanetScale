#pragma once
#include <string>

namespace planet_scale {
class DataCleaner {
public:
    DataCleaner();
    std::string CleanDataEncoding(std::string_view content);
private:

};
} // namespace planet_scale