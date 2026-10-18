#include "User.h"


std::string User::getUsername() const {
	return username;
}
int User::getCredit() const {
	return credit;
}
Permissions User::getPermissions() const {
	return level;
}
void User::addCredit(int amount) {
	credit += amount;
}
bool User::deductCredit(int amount) {
	if (amount <= credit) {
		credit -= amount;
		return true;
	}
	return false;
}
bool User::canBuy() const {
	return (level == Permissions::ADMIN || level == Permissions::FULL_STANDARD || level == Permissions::BUY_STANDARD);
}
bool User::canSell() const {
	return (level == Permissions::ADMIN || level == Permissions::FULL_STANDARD || level == Permissions::BUY_STANDARD);
}
bool User::isAdmin() const {
	return (level == Permissions::ADMIN);
}
bool User::ownsGame(std::string gameName) const {
	if (games.empty()) {
		return false;
	}
	if (std::find(games.begin(), games.end(), gameName) != games.end()) {
		return true;
	}
	return false;
}
void User::addGameToCollection(std::string gameName) {
	games.push_back(gameName);
}	