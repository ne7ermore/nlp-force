#pragma once

#include <string>
#include <vector>

namespace BkTree {
    namespace utils {
        std::wstring str2wstr(const std::string&);

        void split_string(const std::string&, std::vector<std::string>&, const std::string&);
    } // namespace utils
} // namespace BkTree
