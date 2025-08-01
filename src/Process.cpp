#include "../include/Process.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#include "../include/D_Fetch.h"
#include "../include/D_Insert.h"
#include "../include/Requests.h"
#include "../include/Config.h"

void Process::processCpf(std::string cpf) {
    auto start = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::to_time_t(start);
    std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Starting processing for CPF: " << cpf << std::endl;

    const Config cfg;
    const auto& env = cfg.getEnv();
    Fetch db_consultas(env.db_consultas);
    Fetch db_search(env.db_search);
    Database db_storm(env.db_storm);
    std::string login;
    std::string password;
    auto campanha = db_consultas.fetchCurrentCampaign();
    auto counted = db_consultas.countConsultas(env.acesso1.first, env.acesso2.first, env.acesso3.first, env.acesso4.first);
    if (env.acesso1.first == counted) {
        login = env.acesso1.first;
        password = env.acesso1.second;
    } else if (env.acesso2.first == counted) {
        login = env.acesso2.first;
        password = env.acesso2.second;
    } else if (env.acesso3.first == counted) {
        login = env.acesso3.first;
        password = env.acesso3.second;
    } else if (env.acesso4.first == counted) {
        login = env.acesso4.first;
        password = env.acesso4.second;
    }

    auto accessToken = Requests::GetAccessToken(login, password);
    if (!accessToken) {
        std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Error getting token for CPF " << cpf << std::endl;
        return;
    }
    std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Got access token for CPF: " << cpf << std::endl;

    auto responseContent = Requests::GetApiReturn(accessToken->AuthenticationResult.AccessToken, cpf, env.apikey);
    if (!responseContent) {
        std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Error calling API for CPF " << cpf << std::endl;
        return;
    }
    std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] API call successful for CPF: " << cpf << std::endl;

    std::string nome, numero;
    try {
        auto consultas = db_search.fetchConsultas(&db_storm, cpf);
        if (consultas) {
            nome = consultas->nome;
            numero = consultas->numero;
            std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Found nome/numero for CPF " << cpf << ": " << nome << " / " << numero << std::endl;
        } else {
            std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] No nome/numero found for CPF " << cpf << std::endl;
        }
    } catch (...) {
        std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Error fetching nome/numero for CPF " << cpf << std::endl;
    }

    std::string saldo;
    std::string aviso;
    bool erro = false;
    if (responseContent->simulacoes) {
        saldo = responseContent->simulacoes->valorLiberado;
    }
    if (responseContent->avisos && !responseContent->avisos->empty()) {
        aviso = responseContent->avisos->at(0).aviso;
    }
    if (responseContent->error) {
        erro = *(responseContent->error);
    }

    Insert inserter(env.db_consultas);
    try {
        inserter.insertConsultaLog(cpf, saldo, aviso, login, nome, numero, erro);
        std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Inserted consulta log for CPF " << cpf << std::endl;
    } catch (...) {
        std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Error inserting consulta log for CPF " << cpf << std::endl;
    }

    try {
        inserter.updateConsultado(cpf, campanha);
        std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] CPF " << cpf << " marked as processed" << std::endl;
    } catch (...) {
        std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Error marking as processed for CPF " << cpf << std::endl;
    }

    auto end = std::chrono::system_clock::now();
    std::clog << "[" << std::put_time(std::localtime(&now), "%FT%T") << "] [WORKER] Finished processing CPF: " << cpf << " in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}
