#include "TransactionLog.h"
#include "Format.h"
#include <fstream>


void TransactionLog::recordUserTransaction(std::string code, const User& user, int credit) {
	entries.push_back(code + " "
		+ Format::padRight(user.getUsername(), Format::USERNAME_WIDTH) + " "
		+ user.getTypeCode() + " "
		+ Format::formatMoney(credit, Format::CREDIT_WIDTH));
}

void TransactionLog::recordCreate(const User& user) {
	recordUserTransaction("01", user, user.getCredit());
}
void TransactionLog::recordDelete(const User& user) {
	recordUserTransaction("02", user, user.getCredit());
}
//XX_IIIIIIIIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS_PPPPPP
void TransactionLog::recordSell(const GameListing& game) {
	entries.push_back("03 "
		+ Format::padRight(game.getName(), Format::GAME_NAME_WIDTH) + " "
		+ Format::padRight(game.getSellerUsername(), Format::USERNAME_WIDTH) + " "
		+ Format::formatMoney(game.getPrice(), Format::PRICE_WIDTH));
}
//XX_IIIIIIIIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS_UUUUUUUUUUUUUUU_PPPPPP
void TransactionLog::recordBuy(const GameListing& game, const User& buyer) {
	entries.push_back("04 "
		+ Format::padRight(game.getName(), Format::GAME_NAME_WIDTH) + " "
		+ Format::padRight(game.getSellerUsername(), Format::USERNAME_WIDTH) + " "
		+ Format::padRight(buyer.getUsername(), Format::USERNAME_WIDTH) + " "
		+ Format::formatMoney(game.getPrice(), Format::PRICE_WIDTH));
}
//XX_UUUUUUUUUUUUUUU_SSSSSSSSSSSSSSS_CCCCCCCCC
void TransactionLog::recordRefund(const User& buyer, const User& seller, int amount) {
	entries.push_back("05 "
		+ Format::padRight(buyer.getUsername(), Format::USERNAME_WIDTH) + " "
		+ Format::padRight(seller.getUsername(), Format::USERNAME_WIDTH) + " "
		+ Format::formatMoney(amount, Format::CREDIT_WIDTH));
}
void TransactionLog::recordAddCredit(const User& user, int amount) {
	recordUserTransaction("06", user, amount);
}
void TransactionLog::recordEndOfSession(const User& user) {
	recordUserTransaction("00", user, user.getCredit());
}

bool TransactionLog::writeToFile(std::string filename) const {
	std::ofstream file(filename, std::ios::app | std::ios::binary);	//Binary keeps line endings as \n on every platform.
	if (!file) {
		return false;
	}
	for (const auto& entry : entries) {
		file << entry << '\n';
	}
	return static_cast<bool>(file);
}
void TransactionLog::clear() {
	entries.clear();
}
