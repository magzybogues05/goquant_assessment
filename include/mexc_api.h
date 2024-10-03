#ifndef MEXC_API_H
#define MEXC_API_H

#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <openssl/hmac.h>
#include <openssl/sha.h> 

using json = nlohmann::json;

class MEXCAPI {
public:
    MEXCAPI(const std::string& apiKey, const std::string& apiSecret);

    std::string getContractDetails();
    std::string placeLimitOrder(const std::string& symbol, const double& amount, const double& price);
    std::string modifyOrder(const std::string& orderId, const double& newPrice,const double& newVolume);
    std::string cancelOrder(const std::vector<std::string>& orderIds);
    std::string viewPositions();

private:
    std::string apiKey;
    std::string apiSecret;

    std::string sendRequest(const std::string& endpoint, const std::string& method, const std::string& body,struct curl_slist* headers);
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* s);
};

#endif // MEXC_API_H