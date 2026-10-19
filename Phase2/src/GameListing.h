#pragma once
#include <string>
// Game listing class with its respective data
class GameListing {
private:
    std::string name;
    std::string sellerUsername;
    int price;
    bool available = false; //Games listed during the current session cant be bought until the next

public:
    static constexpr int MAX_NAME_LENGTH = 25;
    static constexpr int MAX_PRICE = 99999;   //$999.99

    GameListing(std::string name, std::string seller, int price, bool available)
        : name(name), sellerUsername(seller), price(price), available(available) { };

    std::string getName() const;
    std::string getSellerUsername() const;
    int getPrice() const;
    bool isPurchasable() const;

    static bool isValidName(std::string name);
};
