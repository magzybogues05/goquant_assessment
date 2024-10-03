#include "websocket_client.h"
#include <iostream>
#include <cpprest/ws_client.h>
#include <cpprest/json.h>
#include <thread>
#include <unordered_map>

using namespace std;
using namespace web;
using namespace web::websockets::client;

WebSocketClient::WebSocketClient(const uri& server_uri, const std::string& symbol)
    : server_uri(server_uri), symbol(symbol) {
}

WebSocketClient::~WebSocketClient() {
}

void WebSocketClient::connect() {
    client.connect(server_uri).wait();
    cout << "WebSocket connection opened and subscribed to depth for " << symbol << "." << endl;

    subscribe();  // Subscribe to the depth channel
    receiveMessages();  // Start receiving messages
}

void WebSocketClient::subscribe() {
    json::value request;
    request[U("method")] = json::value::string(U("sub.depth"));
    request[U("param")] = json::value::object();
    request[U("param")][U("symbol")] = json::value::string(U(symbol));

    websocket_outgoing_message out_msg;
    out_msg.set_utf8_message(request.serialize());

    client.send(out_msg).wait();
}

void WebSocketClient::receiveMessages() {
    while (true) {
        client.receive().then([this](websocket_incoming_message msg) {
            if (msg.message_type() == websocket_message_type::text_message) {
                auto body = msg.extract_string().get();
                handleMessage(body);
            } else {
                std::cerr << "Received non-text message type." << std::endl;
            }
        }).wait();
    }
}

void WebSocketClient::handleMessage(const std::string& message) {
    // Parse the message as JSON
    json::value response = json::value::parse(message);

    // Check if the message is from the depth channel
    if (response.has_field(U("channel")) && response[U("channel")].as_string() == U("push.depth")) {
        auto newAsks = response[U("data")][U("asks")].as_array();
        auto newBids = response[U("data")][U("bids")].as_array();

        // Update the order book with new asks and bids
        updateOrderbook(newAsks, newBids);
    }
}


void WebSocketClient::updateOrderbook(const web::json::array& asksArray, const web::json::array& bidsArray) {
    // Example structures to hold the order book
    std::unordered_map<double, double> asks;
    std::unordered_map<double, double> bids;

    for (const auto& ask : asksArray) {
        double price = ask.at(0).as_double();
        double volume = ask.at(1).as_double();
        asks[price] = volume;                
    }

    for (const auto& bid : bidsArray) {
        double price = bid.at(0).as_double(); 
        double volume = bid.at(1).as_double(); 
        bids[price] = volume;                 
    }

    // Display the updated order book
    cout << "Order Book Update for " << symbol << ":\n";

    // Check if asks are present and display them
    if (!asks.empty()) {
        cout << "Asks:\n";
        for (const auto& [price, volume] : asks) {
            cout << "Price: " << price << ", Volume: " << volume << "\n";
        }
    } else {
        cout << "No asks available.\n";
    }

    // Check if bids are present and display them
    if (!bids.empty()) {
        cout << "Bids:\n";
        for (const auto& [price, volume] : bids) {
            cout << "Price: " << price << ", Volume: " << volume << "\n";
        }
    } else {
        cout << "No bids available.\n";
    }
}
