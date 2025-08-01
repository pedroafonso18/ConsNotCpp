#include <chrono>
#include <iostream>
#include <thread>

#include "Config.h"
#include "Database.h"
#include "D_Fetch.h"
#include "Utils.h"

[[noreturn]] int main() {
    const auto &cfg = Config();
    while (true) {
        std::clog << "[MAIN] Starting new processing cycle\n";
        Fetch db = Fetch(cfg.getEnv().db_consultas);
        if (!Utils::IsAllowedTime()) {
            std::clog << "[MAIN] System is not in allowed time, waiting...\n";
            std::this_thread::sleep_for(std::chrono::minutes(5));
            break;
        }
        if (auto is_paused = db.isPaused()) {
            std::clog << "[MAIN] System is paused, waiting...\n";
            std::this_thread::sleep_for(std::chrono::minutes(5));
            break;
        }

        std::clog << "[MAIN] System is not paused, starting processing...\n";

        auto customer_list = db.fetchCustomers();
        if (customer_list.empty()) {
            std::clog << "[MAIN] List has no cpf's left, waiting...\n";
            std::this_thread::sleep_for(std::chrono::minutes(5));
            break;
        }
        for (auto const &customer : customer_list) {
            Process::processCpf(customer);
        }

        std::clog << "[MAIN] Successfully processed all cpf's, waiting...\n";
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }
}
