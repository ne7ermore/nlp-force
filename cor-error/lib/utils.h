#pragma once

#include <string>
#include <vector>
#include <sys/time.h>

namespace Bkt {
    namespace utils {
        class time {
            public:
                print_time () { gettimeofday(&start, NULL); }
                void refresh();
                void print();
            private:
                struct timeval start, end;
        }

        int levenshtein_distance(std::wstring, std::wstring);

        std::wstring str2wstr(std::string);

        bool pair_sort(const std::);
        bool is_digits(const std::string&);

        float std_dev(const std::vector<float>&);
    } // namespace utils
} // namespace Bkt