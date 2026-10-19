#pragma once
#include <string>
#include <vector>

enum Permissions {
	ADMIN,
	FULL_STANDARD,
	BUY_STANDARD,
	SELL_STANDARD,
};

// Defines a user
class User {
private:
	std::string username = "";
	Permissions level;
	int credit = 0;	
	std::vector<std::string> games;

public:
	static constexpr int MAX_USERNAME_LENGTH = 15;
	static constexpr int MAX_CREDIT = 99999900;	//$999,999.00

	User(std::string user, Permissions permissions, int credit = 0)
		: username(user), level(permissions), credit(credit) { }

	std::string getUsername() const;
	int getCredit() const;
	Permissions getPermissions() const;
	std::string getTypeCode() const;
	bool addCredit(int amount);
	bool deductCredit(int amount);
	bool canBuy() const;
	bool canSell() const;
	bool isAdmin() const;
	bool ownsGame(std::string gameName) const;
	void addGameToCollection(std::string gameName);

	static bool parseType(std::string text, Permissions& permissions);
	static bool isValidUsername(std::string name);
};
