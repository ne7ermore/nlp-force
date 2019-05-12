#pragma once

#include <string>
#include <vector>
#include <sys/time.h>

namespace Bkt {
    namespace utils {
        class time {
            public:
                time () { gettimeofday(&start, NULL); }
                void refresh();
                void print();
            private:
                struct timeval start, end;
        };

        std::wstring str2wstr(std::string);

        bool pair_sort(const std::pair<float, std::string>&, const std::pair<float, std::string>&);
        bool is_digits(const std::string&);

        float std_dev(const std::vector<float>&);

        void split_string(const std::string&, std::vector<std::string>&, const std::string&);
    } // namespace utils
} // namespace Bkt