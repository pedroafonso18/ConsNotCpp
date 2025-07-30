#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <string>

using std::string;
using std::pair;


typedef struct {
    pair<string, string> acesso1;
    pair<string, string> acesso2;
    pair<string, string> acesso3;
    pair<string, string> acesso4;
    string db_consultas;
    string db_search;
    string db_storm;
    string apikey;
} Env;

class Config {
private:
    std::unique_ptr<Env> env;
public:
    Config();
    [[nodiscard]] const Env& getEnv() const;
};

#endif //CONFIG_H
