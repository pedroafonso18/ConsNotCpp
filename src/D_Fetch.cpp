#include "../include/D_Fetch.h"

#include <iostream>

Models::Pessoa *Fetch::fetchConsultas(const Database *stormClient, std::string cpf) {
    auto c = db->getConnection();
    auto c_storm = stormClient->getConnection();
    if (!c->is_open() || !c_storm->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        throw std::runtime_error("DB CONNECTION IS NOT ACTIVE");
    }

    auto cleaned_cpf = Utils::cleanCpf(cpf);

    pqxx::work transaction(*c);
    pqxx::work storm_transaction(*c_storm);

    Models::Pessoa* pessoa = new Models::Pessoa();

    std::string query = "SELECT nome, numero FROM contatos WHERE cpf is not null AND (cpf = $1 OR cpf = $2)";
    std::cout << "Executing query: " << query << std::endl;
    std::cout << "With parameters: " << cpf << " and " << cleaned_cpf << std::endl;

    try {
        auto result = transaction.exec(query, cpf, cleaned_cpf);
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
        auto stormResult = storm_transaction.exec(stormQuery, cpf);

        if (!stormResult.empty()) {
            pessoa->nome = stormResult[0][0].as<std::string>();
            pessoa->numero = stormResult[0][1].as<std::string>();
            std::cout << "Contato encontrado na Storm: " << pessoa->nome << " - " << pessoa->numero << std::endl;
            transaction.commit();
            storm_transaction.commit();
            return pessoa;
        }

        std::cout << "Nenhum contato encontrado para o CPF: " << cpf << " dentro da Storm." << std::endl;
        delete pessoa;
        transaction.commit();
        storm_transaction.commit();
        return nullptr;
    } catch (pqxx::sql_error const &e) {
        transaction.abort();
        storm_transaction.abort();
        delete pessoa;
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }
}
