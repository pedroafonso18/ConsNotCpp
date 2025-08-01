#ifndef D_INSERT_H
#define D_INSERT_H
#include "Database.h"

class Insert {
private:
    std::unique_ptr<Database> ownedDb;
    Database* db;
public:
    explicit Insert(Database& db) : db(&db) {}
    explicit Insert(std::string Url)
        : ownedDb(std::make_unique<Database>(Url)), db(ownedDb.get()) {}
    void insertConsultaLog(std::string cpf, std::string saldo, std::string aviso, std::string login, std::string nome, std::string numero, bool erro) const;
    void updateConsultado(std::string cpf, std::string campanha) const;
};
#endif //D_INSERT_H
