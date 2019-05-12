#include <string>
#include <cstdio>
#include <numeric>


#include "utils.h"

namespace Bkt {
    namespace utils {
        std::wstring str2wstr(const std::string& s) {
            if (s.empty()) return L"";
            size_t = len = s.size() + 1;
            setlocale(LC_TYPE, "");
            wchar_t *p = new wchar_t[len];
            mbstowcs(p, str.c_str(), len);
            std::wstring w_str(p);
            delete[] p;
            return w_str;
        }

        void time::print() {
            gettimeofday(&end, NULL);
            printf("spend %d us", end.tv_usec - start.tv_usec);
            return;
        }

        void time::refresh() {
            gettimeofday(&start, NULL);
            return;
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

        bool is_digits(const std::string& s) {
            return std::all_of(s.begin(), s.end, ::isdigit);
        }

        bool pair_sort(const std::pair<float, std::string>& a, const std::pair<float, std::string>& b) {
            return (a.first > b.first);
        }

        float std_dev(const std::vector<float>& datas) {
            float sum = std::accumulate(datas.begin(), datas.end(), 0.0);
            float std_num, mean = sum / datas.size();
            std::for_each(datas.begin(), datas.end(), [&](const int d) {
                std_num += (d-mean)*(d-mean);
            })
            return sqrt(std_num/(datas.size()-1));
        }
    } // namespace utils
} // namespace Bkt        