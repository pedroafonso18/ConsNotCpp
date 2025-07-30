#ifndef REQUESTS_H
#define REQUESTS_H

#include <memory>
#include "Models.h"

class Requests {
public:
    static Models::AccessTokenResp* GetAccessToken(std::string access, std::string password);
    static Models::ResponseContent* GetApiReturn(std::string token, std::string cpf, std::string apikey);
};

#endif //REQUESTS_H
