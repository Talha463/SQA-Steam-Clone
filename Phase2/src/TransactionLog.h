#pragma once
#include <string>
#include <vector>
#include "GameListing.h"
#include "User.h"

//Collects transactions over a session and the daily transaction file
class TransactionLog {
private:
	std::vector<std::string> entries;

	//XX_UUUUUUUUUUUUUUU_TT_CCCCCCCCC
	void recordUserTransaction(std::string code, const User& user, int credit);

public:
	void recordCreate(const User& user);
	void recordDelete(const User& user);
	void recordSell(const GameListing& game);
	void recordBuy(const GameListing& game, const User& buyer);	
	void recordRefund(const User& buyer, const User& seller, int amount);
	void recordAddCredit(const User& user, int amount);
	void recordEndOfSession(const User& user);

	bool writeToFile(std::string filename) const;	//Appends, so each session adds to the day's file.
	void clear();
};
