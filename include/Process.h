#ifndef PROCESS_H
#define PROCESS_H
#include <string>

class Process {
public:
    Process() = delete;
    static void processCpf(const std::string& cpf);
};

#endif