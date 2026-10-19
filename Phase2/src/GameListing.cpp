#include "GameListing.h"

// Getters and setters for GameListing class which contains data for each game
std::string GameListing::getName() const {
	return name;
}
std::string GameListing::getSellerUsername() const {
	return sellerUsername;
}
int GameListing::getPrice() const {
	return price;
}
// checks if a game is available to purchase.
bool GameListing::isPurchasable() const {
	return available;
}
// validates if a given name meets the length and not empty criteria
bool GameListing::isValidName(std::string name) {
	return (!name.empty() && name.size() <= MAX_NAME_LENGTH);
}
