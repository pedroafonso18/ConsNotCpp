#include "../include/Requests.h"
#include <iostream>
#include <curl/curl.h>
#include "../lib/fmt/format.h"
#include "../lib/json.h"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* resp = static_cast<std::string*>(userp);
    resp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

Models::AccessTokenResp* Requests::GetAccessToken(std::string access, std::string password) {
    CURL* curl = curl_easy_init();
    std::string respBody;
    if (!curl) {
        std::clog << "ERROR: Failed to initialize CURL\n";
        return nullptr;
    }
    nlohmann::json req_body = {
        {"ClientId", "63ccaojkma1th1pucikhn1n19k"},
        {"AuthFlow", "USER_PASSWORD_AUTH"},
        {"AuthParameters", {{"USERNAME", access}, {"PASSWORD", password}}}
    };

    std::string str_bod = req_body.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/x-amz-json-1.1");
    headers = curl_slist_append(headers, "X-Amz-Target: AWSCognitoIdentityProviderService.InitiateAuth");

    curl_easy_setopt(curl, CURLOPT_URL, "https://cognito-idp.us-east-2.amazonaws.com");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_bod.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respBody);

    Models::AccessTokenResp* result = nullptr;
    if (curl_easy_perform(curl) == CURLE_OK) {
        nlohmann::json resp_json = nlohmann::json::parse(respBody, nullptr, false);
        if (!resp_json.is_discarded() && resp_json.contains("AuthenticationResult")) {
            result = new Models::AccessTokenResp();
            result->AuthenticationResult.AccessToken = resp_json["AuthenticationResult"]["AccessToken"].get<std::string>();
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return result;
}