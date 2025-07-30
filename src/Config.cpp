#include "../include/Config.h"
#include "../lib/dotenv.h"
#include <memory>

Config::Config() : env(std::unique_ptr<Env>(new Env)) {
    std::string root_path = "../.env";
    dotenv::init(root_path.c_str());
}

const Env& Config::getEnv() const {
    env->acesso1 = std::make_pair(dotenv::getenv("ACESSO_1"), dotenv::getenv("SENHA_1"));
    env->acesso2 = std::make_pair(dotenv::getenv("ACESSO_2"), dotenv::getenv("SENHA_2"));
    env->acesso3 = std::make_pair(dotenv::getenv("ACESSO_3"), dotenv::getenv("SENHA_3"));
    env->acesso4 = std::make_pair(dotenv::getenv("ACESSO_4"), dotenv::getenv("SENHA_4"));
    env->apikey = dotenv::getenv("APIKEY");
    env->db_consultas = dotenv::getenv("DB_CONSULTAS");
    env->db_search = dotenv::getenv("DB_SEARCH");
    env->db_storm = dotenv::getenv("DB_STORM");
    return *env;
}
