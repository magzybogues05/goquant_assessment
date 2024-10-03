#include "mexc_api.h"
#include "websocket_client.h"
#include <iostream>
#include <thread>
#include<vector>
#include <fstream>
#include "nlohmann/json.hpp"
#include "websocket_client.h" 
#include <cpprest/uri.h>
using json = nlohmann::json;
int main() {
    // Load config and initialize API
    
    std::ifstream configFile("config/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Error opening config.json file." << std::endl;
    }
    json config;
    configFile >> config;
    std::cout << "Config content: " << config << std::endl;
    std::string apiKey=config["api_key"];
    std::string apiSecret=config["api_secret"];
    MEXCAPI mexc(apiKey,apiSecret);
    
    // // Step 1: Place a limit order for BTC/USDT

    // std::string orderResponse = mexc.placeLimitOrder("BTC_USDT", 1, 49200.5);
    // std::cout << "Order placed: " << orderResponse << std::endl;

    // // Step 2: Modify the order

    // std::string orderId = "596709111547120128";
    // std::string modifyResponse= mexc.modifyOrder(orderId, 55288.2,2);
    // std::cout << "modified order: "<<modifyResponse <<std::endl; 

    // Step 3: Cancel the order

    // std::vector<std::string>orderId={"596710657051674112"};
    // std::string cancelOrderResponse=mexc.cancelOrder(orderId);
    // std::cout<< "cancelled order: "<<cancelOrderResponse<< std::endl;

    // Step 4: View current positions
    // std::string positions = mexc.viewPositions();
    // std::cout << "Current positions: " << positions << std::endl;



    //order book (web socket)

    web::uri server_uri(U("wss://futures.testnet.mexc.com/edge"));
    std::string symbol = "ETH_USDT";
    WebSocketClient client(server_uri, symbol);
    
    try {
        // Connect to the WebSocket server
        client.connect();
    } catch (const std::exception& e) {
        std::cerr << "Error connecting to WebSocket: " << e.what() << std::endl;
        return 1; // Exit with an error code
    }

    return 0;
}
