#include "../include/D_Fetch.h"

#include <iostream>
#include <memory>
#include <stdexcept>

std::unique_ptr<Models::Pessoa> Fetch::fetchConsultas(const Database *stormClient, std::string cpf) {
    auto c = db->getConnection();
    auto c_storm = stormClient->getConnection();
    if (!c->is_open() || !c_storm->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        throw std::runtime_error("DB CONNECTION IS NOT ACTIVE");
    }

    auto cleaned_cpf = Utils::cleanCpf(cpf);

    pqxx::work transaction(*c);
    pqxx::work storm_transaction(*c_storm);

    auto pessoa = std::make_unique<Models::Pessoa>();

    std::string query = "SELECT nome, numero FROM contatos WHERE cpf is not null AND (cpf = $1 OR cpf = $2)";
    std::cout << "Executing query: " << query << std::endl;
    std::cout << "With parameters: " << cpf << " and " << cleaned_cpf << std::endl;

    try {
        auto result = transaction.exec(pqxx::zview(query), pqxx::params(cpf, cleaned_cpf));
        if (!result.empty()) {
            pessoa->nome = result[0][0].as<std::string>();
            pessoa->numero = result[0][1].as<std::string>();
            std::cout << "Contato encontrado na base principal: " << pessoa->nome << " - " << pessoa->numero << std::endl;
            transaction.commit();
            storm_transaction.commit();
            return pessoa;
        }

        std::cout << "Nenhum contato encontrado na base principal para o CPF: " << cpf << std::endl;
        std::cout << "Tentando buscar na base Storm..." << std::endl;

        std::string stormQuery = "SELECT cliente, telefone_celular FROM digitados_sistema WHERE cpf_cliente = $1 LIMIT 1";
        auto stormResult = storm_transaction.exec(pqxx::zview(stormQuery), pqxx::params(cpf));

        if (!stormResult.empty()) {
            pessoa->nome = stormResult[0][0].as<std::string>();
            pessoa->numero = stormResult[0][1].as<std::string>();
            std::cout << "Contato encontrado na Storm: " << pessoa->nome << " - " << pessoa->numero << std::endl;
            transaction.commit();
            storm_transaction.commit();
            return pessoa;
        }

        std::cout << "Nenhum contato encontrado para o CPF: " << cpf << " dentro da Storm." << std::endl;
        transaction.commit();
        storm_transaction.commit();
        return nullptr;
    } catch (pqxx::sql_error const &e) {
        transaction.abort();
        storm_transaction.abort();
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }
}

std::string Fetch::countConsultas(std::string user1, std::string user2, std::string user3, std::string user4) const {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        throw std::runtime_error("DB CONNECTION IS NOT ACTIVE");
    }
    pqxx::work transaction(*c);

    try {
        auto result = transaction.exec(pqxx::zview("WITH counts AS ( SELECT COUNT(CASE WHEN login = $1 THEN 1 END) AS login1, COUNT(CASE WHEN login = $2 THEN 1 END) AS login2, COUNT(CASE WHEN login = $3 THEN 1 END) AS login3, COUNT(CASE WHEN login = $4 THEN 1 END) AS login4 FROM logs_consultas WHERE created_at >= NOW() - INTERVAL '24 hours' ) SELECT login, count FROM counts, LATERAL ( VALUES ($1, login1), ($2, login2), ($3, login3), ($4, login4) ) AS t(login, count) ORDER BY count ASC LIMIT 1;"), pqxx::params(user1, user2, user3, user4));
        if (!result.empty()) {
            auto login = result.one_field().as<std::string>();
            transaction.commit();
            return login;
        }

        transaction.commit();
        return "";
    } catch (pqxx::sql_error const& e) {
        transaction.abort();
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }
}

std::string Fetch::fetchCurrentCampaign() const {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        throw std::runtime_error("DB CONNECTION IS NOT ACTIVE");
    }

    try {
        pqxx::work transaction(*c);
        const auto result = transaction.exec("SELECT campanha_ativa FROM config LIMIT 1");
        std::string campanha = result[0][0].as<std::string>();
        transaction.commit();
        return campanha;
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }
}

bool Fetch::isPaused() const {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        throw std::runtime_error("DB CONNECTION IS NOT ACTIVE");
    }

    try {
        pqxx::work transaction(*c);
        const auto result = transaction.exec("SELECT pausado FROM config");
        transaction.commit();
        return result[0][0].as<bool>();
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }
}

std::vector<std::string> Fetch::fetchCustomers() const {
    auto c = db->getConnection();
    auto campanha = fetchCurrentCampaign();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        throw std::runtime_error("DB_CONNECTION IS NOT ACTIVE");
    }

    try {
        std::vector<std::string> vec;
        pqxx::work transaction(*c);
        const auto result = transaction.exec(pqxx::zview("SELECT cpf FROM consultar WHERE consultado = false AND campanha = $1 LIMIT 10"), pqxx::params(campanha));
        for (auto row : result) {
            vec.push_back(row[0].as<std::string>());
        }
        return vec;
    } catch (const pqxx::sql_error& e) {
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }
}

