#include <iostream>
#include "FrontEnd.h"

//Usage: steam_clone [accountsFile] [gamesFile] [transactionFile]

// Main function that initalizes and runs the steam clone front end interface
int main(int argc, char* argv[]) {
	std::string accountsFile = (argc > 1) ? argv[1] : "currentaccounts.txt";
	std::string gamesFile = (argc > 2) ? argv[2] : "availablegames.txt";
	std::string transactionFile = (argc > 3) ? argv[3] : "dailytransactions.txt";

	FrontEnd frontEnd(std::cin, std::cout, accountsFile, gamesFile, transactionFile);
	frontEnd.run();
	return 0;
}
