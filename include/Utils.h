#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>

class Utils {
public:
    static std::string cleanCpf(std::string cpf) {
        cpf.erase(std::remove_if(cpf.begin(), cpf.end(),
                  [](char c) { return c == '.' || c == '-'; }),
                  cpf.end());
        return cpf;
    }

    static bool IsAllowedTime() {
        const auto now = std::chrono::system_clock::now();
        const auto time_t_now = std::chrono::system_clock::to_time_t(now);
        const std::tm* tm_now = std::localtime(&time_t_now);

        std::tm start_tm = *tm_now;
        start_tm.tm_hour = 21;
        start_tm.tm_min = 30;
        start_tm.tm_sec = 0;
        const auto start_time = std::chrono::system_clock::from_time_t(std::mktime(&start_tm));

        std::tm end_tm = *tm_now;
        end_tm.tm_hour = 2;
        end_tm.tm_min = 30;
        end_tm.tm_sec = 0;

        if (auto end_time = std::chrono::system_clock::from_time_t(std::mktime(&end_tm)); start_time <= end_time) {
            return (now >= start_time) && (now <= end_time);
        } else {
            return (now >= start_time) || (now <= end_time);
        }
    }

};

#endif