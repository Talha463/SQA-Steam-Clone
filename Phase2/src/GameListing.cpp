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