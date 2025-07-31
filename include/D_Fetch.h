#ifndef D_FETCH_H
#define D_FETCH_H
#include "Database.h"
#include "Models.h"
#include "Utils.h"

class Fetch {
private:
    std::unique_ptr<Database> ownedDb;
    Database* db;
public:
    explicit Fetch(Database& db) : db(&db) {}
    explicit Fetch(std::string Url)
        : ownedDb(std::unique_ptr<Database>(new Database(Url))), db(ownedDb.get()) {}
    Models::Logins* countConsultas(std::string user1, std::string user2, std::string user3, std::string user4);
    Models::Pessoa* fetchConsultas(const Database* stormClient, std::string cpf);
    std::string fetchCurrentCampaign();
    bool isPaused();
    std::vector<std::string> fetchCustomers();
};

#endif //D_FETCH_H
