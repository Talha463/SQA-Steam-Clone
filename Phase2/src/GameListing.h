#include <string>

class GameListing {
private:
    std::string name;
    std::string sellerUsername;
    int price;
    bool available = false;

public:
    GameListing(std::string name, std::string seller, int price)
        : name(name), sellerUsername(seller), price(price) { };

    std::string getName() const;
    std::string getSellerUsername() const;
    int getPrice() const;
    bool isPurchasable() const;
};