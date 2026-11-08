#pragma once
#include <iostream>
#include <string>
#include "GameCatalog.h"
#include "Session.h"
#include "TransactionLog.h"
#include "UserAccounts.h"

//Reads transactions one line at a time and applies them to the accounts and games.
class FrontEnd {
private:
	std::istream& in;
	std::ostream& out;
	std::string accountsFile;
	std::string gamesFile;
	std::string collectionFile;
	std::string transactionFile;

	UserAccounts accounts;
	GameCatalog catalog;
	Session session;
	TransactionLog log;
	bool running = true;

	bool prompt(std::string message, std::string& response);
	void processTransaction(std::string code);

	void handleHelp();	// Simple function that makes it easier to understand what is happening as a user
	void handleLogin();
	void handleLogout();
	void handleCreate();
	void handleDelete();
	void handleSell();
	void handleBuy();
	void handleRefund();
	void handleAddCredit();
	void handleSearch();

public:
	FrontEnd(std::istream& in, std::ostream& out, std::string accountsFile, std::string gamesFile, std::string collectionFile, std::string transactionFile)
		: in(in), out(out), accountsFile(accountsFile), gamesFile(gamesFile), collectionFile(collectionFile), transactionFile(transactionFile) { }

	void run();
};
