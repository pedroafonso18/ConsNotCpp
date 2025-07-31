#include "../include/D_Insert.h"

#include <iostream>

void Insert::insertConsultaLog(std::string cpf, std::string saldo, std::string aviso, std::string login, std::string nome, std::string numero, bool erro) {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        throw std::runtime_error("DB CONNECTION IS NOT ACTIVE");
    }
    pqxx::work transaction(*c);
    std::string err_str = (erro) ? "true" : "false";

    try {
        pqxx::result res;
        if (nome != "" && numero != "") {
            res = transaction.exec_params("INSERT INTO logs_consultas (cpf, saldo_consultado, erro, aviso, login, nome, numero) VALUES ($1, $2, $3, $4, $5, $6, $7)", cpf, saldo, err_str, aviso, login, nome, numero);
        } else {
            res = transaction.exec_params("INSERT INTO logs_consultas (cpf, saldo_consultado, erro, aviso, login) VALUES ($1, $2, $3, $4, $5)", cpf, saldo, err_str, aviso, login);
        }
        if (!res.affected_rows() == 0) {
            std::clog << "ERROR: error when inserting Enqueued.\n";
        }
    } catch (pqxx::sql_error& e) {
        transaction.abort();
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }
}

void Insert::updateConsultado(std::string cpf, std::string campanha) {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        throw std::runtime_error("DB CONNECTION IS NOT ACTIVE");
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec_params("UPDATE consultar SET consultado = true WHERE cpf = $1 AND campanha = $2", cpf, campanha);
        if (!res.affected_rows() == 0) {
            std::clog << "ERROR: error when inserting Enqueued.\n";
        }
    } catch (pqxx::sql_error& e) {
        transaction.abort();
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }
}
