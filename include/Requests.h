#ifndef REQUESTS_H
#define REQUESTS_H

#include <memory>
#include "Models.h"

class Requests {
public:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static Models::AccessTokenResp* GetAccessToken(std::string access, std::string password);
    static Models::ResponseContent* GetApiReturn(std::string token, std::string cpf, std::string apikey);
};

#endif //REQUESTS_H
