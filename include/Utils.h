#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <algorithm>

class Utils {
public:
    static std::string cleanCpf(std::string cpf) {
        cpf.erase(std::remove_if(cpf.begin(), cpf.end(),
                  [](char c) { return c == '.' || c == '-'; }),
                  cpf.end());
        return cpf;
    }
};

#endif