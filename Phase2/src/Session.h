#pragma once
#include <map>
#include <string>
#include "User.h"

class Session {
private:
	User* currentUser = nullptr;
	std::map<std::string, int> addedCredit;	//Credit added to each account this session <username, amount>

public:
	static constexpr int CREDIT_LIMIT = 100000;	//$1000.00 per account per session

	bool login(User* user);
	bool logout();
	bool isLoggedIn() const;
	User* getCurrentUser() const;
	int creditAdded(std::string username) const;
	void recordCreditAddition(std::string username, int amount);
	int remainingCreditLimit(std::string username) const;
};
