#include "User.h"
#include "Format.h"
#include <algorithm>


std::string User::getUsername() const {
	return username;
}
int User::getCredit() const {
	return credit;
}
Permissions User::getPermissions() const {
	return level;
}
std::string User::getTypeCode() const {
	switch (level) {
	case Permissions::ADMIN:
		return "AA";
	case Permissions::FULL_STANDARD:
		return "FS";
	case Permissions::BUY_STANDARD:
		return "BS";
	case Permissions::SELL_STANDARD:
		return "SS";
	}
	return "  ";
}
bool User::addCredit(int amount) {
	if (amount < 0 || credit + amount > MAX_CREDIT) {
		return false;
	}
	credit += amount;
	return true;
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
	return (level == Permissions::ADMIN || level == Permissions::FULL_STANDARD || level == Permissions::SELL_STANDARD);
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

bool User::parseType(std::string text, Permissions& permissions) {
	std::string type = Format::toLower(Format::trim(text));
	if (type == "aa" || type == "admin") {
		permissions = Permissions::ADMIN;
	}
	else if (type == "fs" || type == "full-standard") {
		permissions = Permissions::FULL_STANDARD;
	}
	else if (type == "bs" || type == "buy-standard") {
		permissions = Permissions::BUY_STANDARD;
	}
	else if (type == "ss" || type == "sell-standard") {
		permissions = Permissions::SELL_STANDARD;
	}
	else {
		return false;
	}
	return true;
}
bool User::isValidUsername(std::string name) {
	return (!name.empty() && name.size() <= MAX_USERNAME_LENGTH);
}
