#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <cpprest/ws_client.h>
#include <cpprest/json.h>
#include <map>
#include <string>
#include <vector>

class WebSocketClient {
public:
    WebSocketClient(const web::uri& server_uri, const std::string& symbol);
    ~WebSocketClient();

    // Connects to the WebSocket and starts receiving data
    void connect();

private:
    web::uri server_uri;       // WebSocket server URI
    std::string symbol;         
    std::map<double, double> asks;  // Orderbook asks
    std::map<double, double> bids;  // Orderbook bids

    // Sends subscription message to WebSocket
    void subscribe();

    // Receives and handles incoming messages
    void receiveMessages();

    // Updates the orderbook with the received data
    void updateOrderbook(const web::json::array& asksArray, const web::json::array& bidsArray);
    // Handles incoming WebSocket messages
    void handleMessage(const std::string& message);

    // WebSocket client
    web::websockets::client::websocket_client client;
};

#endif // WEBSOCKETCLIENT_H