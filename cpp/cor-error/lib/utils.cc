#include <string>
#include <stdio.h> 
#include <sys/time.h>
#include <algorithm> 
#include <math.h>

#include "utils.h"

namespace BkTree {
    namespace utils {
        std::wstring str2wstr(const std::string& str) {
            if (str.empty()) return L"";

            size_t len = str.size() + 1;
            setlocale(LC_CTYPE, "");
            wchar_t *p = new wchar_t[len];
            mbstowcs(p, str.c_str(), len);
            std::wstring w_str(p);
            delete[] p;
            return w_str;            
        }

        void split_string(const std::string& s, std::vector<std::string>& v, const std::string& c) {
            std::string::size_type pos1 = 0, pos2 = s.find(c);
            while (std::string::npos != pos2) {
                v.push_back(s.substr(pos1, pos2));
                pos1 = pos2 + c.size();
                pos2 = s.find(c, pos1);
            }
            if (pos1 != s.length()) v.push_back(s.substr(pos1));
        }
    } // namespace utils
} // namespace BkTree