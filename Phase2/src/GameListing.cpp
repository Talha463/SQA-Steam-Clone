#include "GameListing.h"


std::string GameListing::getName() const {
	return name;
}
std::string GameListing::getSellerUsername() const {
	return sellerUsername;
}
int GameListing::getPrice() const {
	return price;
}
bool GameListing::isPurchasable() const {
	return available;
}

bool GameListing::isValidName(std::string name) {
	return (!name.empty() && name.size() <= MAX_NAME_LENGTH);
}
