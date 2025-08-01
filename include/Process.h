#ifndef PROCESS_H
#define PROCESS_H
#include <string>

class Process {
public:
    Process() = delete;
    static void processCpf(std::string cpf);
};

#endif