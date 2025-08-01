#ifndef D_FETCH_H
#define D_FETCH_H
#include "Database.h"
#include "Models.h"
#include "Utils.h"
#include <memory>

class Fetch {
private:
    std::unique_ptr<Database> ownedDb;
    Database* db;
public:
    explicit Fetch(Database& db) : db(&db) {}
    explicit Fetch(std::string Url)
        : ownedDb(std::unique_ptr<Database>(new Database(Url))), db(ownedDb.get()) {}
    [[nodiscard]] std::string countConsultas(std::string user1, std::string user2, std::string user3, std::string user4) const;
    std::unique_ptr<Models::Pessoa> fetchConsultas(const Database* stormClient, std::string cpf);
    std::string fetchCurrentCampaign() const;
    bool isPaused() const;
    std::vector<std::string> fetchCustomers() const;
};

#endif //D_FETCH_H
