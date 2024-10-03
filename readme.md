
# goquant assessment

Objective: Create an order execution and management system using C++ to trade on MEXC Futures.

Starting Points:
- [MEXC Futures API Information](https://www.mexc.com/support/articles/5705980230169)
  - Create a new MEXC Futures account at [MEXC Testnet](https://futures.testnet.mexc.com/exchange). You may need a VPN depending on your location; free services like Hotspot Shield or ProtonVPN set to the Netherlands may be useful.
  - Navigate to API Management under user settings on the MEXC Testnet site.
  - Generate an API Key and Secret for your demo trading account (2FA is required).
  - Request a Demo Coins account with fake money (10,000 USD should be sufficient).

Functions:
- Place Order: Submit a limit order using the symbol BTC/USDT to take a long position for 0.1 BTC for USD as a market-making order that goes live but does not fill (important for subsequent steps).
- Modify Order: Update the order placed above ensuring it does not fill.
- Cancel Order: Cancel the order mentioned above.
- View Current Positions: Retrieve positions.
- Get Orderbook: Set up a WebSocket streamhandler for `Depth` for ETH/USDT as detailed in the [API documentation](https://mexcdevelop.github.io/apidocs/contract_v1_en/?python#public-channels).

