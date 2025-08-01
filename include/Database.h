#ifndef DATABASE_H
#define DATABASE_H
#include <pqxx/pqxx>
#include <memory>
#include <string>

class Database {
private:
    std::unique_ptr<pqxx::connection> c;
public:
    explicit Database(const std::string& dbUrl)
        : c(std::make_unique<pqxx::connection>(dbUrl)) {
        if (!c->is_open()) {
            throw std::runtime_error("Failed to open database");
        }
    }

    [[nodiscard]] pqxx::connection* getConnection() const {
        return c.get();
    }
};


#endif //DATABASE_H
