#include "mexc_api.h"
#include <iostream>
#include<vector>
#include <fstream>
#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h> 
#include <openssl/hmac.h>
#include <openssl/evp.h>
// #include <openssl/hex.h>

using json = nlohmann::json;

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return std::to_string(milliseconds);
}


MEXCAPI::MEXCAPI(const std::string& apiKey, const std::string& apiSecret)
    : apiKey(apiKey), apiSecret(apiSecret) {}

// Place a limit order
std::string MEXCAPI::placeLimitOrder(const std::string& symbol, const double& amount, const double& price) {
    std::string endpoint = "/api/v1/private/order/create";
    
    // Create JSON body with necessary parameters
    json body = {
        {"leverage", 10},
        {"marketCeiling", false},
        {"price", std::to_string(price)},
        {"priceProtect", "0"},
        {"side", 1},  // Open long (Buy)
        {"openType", 2}, // Cross margin
        {"symbol", symbol},
        {"ts", std::stoll(getCurrentTimestamp())}, // Generate a timestamp
        {"type", "1"},  // Limit order
        {"vol", amount}
    };

    // Instead of generating a signature, we will use the provided authorization token
    std::string authToken = "WEB87e304455be4918d17780d3e5800da670eedc24ff62d5b4c80f961371e97d4ff"; // Provided token
    
    // Prepare headers
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: " + authToken).c_str()); // Add authorization token
    headers = curl_slist_append(headers, "Content-Type: application/json"); // Set content type to JSON

    // Send request
    return sendRequest(endpoint, "POST", body.dump(), headers);
}

// Modify an order by canceling the old one and placing a new one
std::string MEXCAPI::modifyOrder(const std::string& orderId, const double& newPrice, const double& newVolume) {
        std::string endpoint = "/api/v1/private/order/change_limit_order";
    
    // Create the JSON body for modifying an existing limit order
    json body = {
        {"orderId", orderId},   // Existing order ID to modify
        {"price", std::to_string(newPrice)},    // New price for the order
        {"vol", newVolume}      // New volume for the order
    };

    // Authorization token
    std::string authToken = "WEB87e304455be4918d17780d3e5800da670eedc24ff62d5b4c80f961371e97d4ff"; // Replace with actual token

    // Prepare headers
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: " + authToken).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Send the request
    return sendRequest(endpoint, "POST", body.dump(), headers);
}


std::string MEXCAPI::cancelOrder(const std::vector<std::string>& orderIds) {
    std::string endpoint = "/api/v1/private/order/cancel";
    
    // Create the JSON body, which is an array of order IDs to cancel
    json body = orderIds;

    // Authorization token
    std::string authToken = "WEB87e304455be4918d17780d3e5800da670eedc24ff62d5b4c80f961371e97d4ff"; // Replace with actual token

    // Prepare headers
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: " + authToken).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Send the request
    std::string response = sendRequest(endpoint, "POST", body.dump(), headers);

    return response;
}


// View current positions
std::string MEXCAPI::viewPositions() {
    std::string endpoint = "/api/v1/private/position/open_positions";
    std::string authToken = "WEB87e304455be4918d17780d3e5800da670eedc24ff62d5b4c80f961371e97d4ff"; // Replace with actual token

    // Prepare headers with authorization token
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: " + authToken).c_str()); // Use authToken instead of signature
    headers = curl_slist_append(headers, "Content-Type: application/json");

    return sendRequest(endpoint, "GET", "", headers);
}




// Send HTTP requests
std::string MEXCAPI::sendRequest(const std::string& endpoint, const std::string& method, const std::string& body, struct curl_slist* headers) {
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string response_string;

    if (curl) {

        std::string url = "https://futures.testnet.mexc.com" + endpoint;

        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        }


        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);


        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        // std::cout << "Response: " << response_string << std::endl;
    }
    return response_string;
}

// Write callback function for CURL
size_t MEXCAPI::writeCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch (std::bad_alloc& e) {
        return 0;
    }
    return newLength;
}

