#include "Session.h"


bool Session::login(User* user) {
	if (isLoggedIn() || user == nullptr) {
		return false;
	}
	currentUser = user;
	addedCredit.clear();
	return true;
}
bool Session::logout() {
	if (!isLoggedIn()) {
		return false;
	}
	currentUser = nullptr;
	addedCredit.clear();
	return true;
}
bool Session::isLoggedIn() const {
	return (currentUser != nullptr);
}
User* Session::getCurrentUser() const {
	return currentUser;
}
int Session::creditAdded(std::string username) const {
	auto entry = addedCredit.find(username);
	if (entry == addedCredit.end()) {
		return 0;
	}
	return entry->second;
}
void Session::recordCreditAddition(std::string username, int amount) {
	addedCredit[username] += amount;
}
int Session::remainingCreditLimit(std::string username) const {
	return CREDIT_LIMIT - creditAdded(username);
}
