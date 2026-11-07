#include "Session.h"

// Logs in a user session
bool Session::login(User* user) {
	if (isLoggedIn() || user == nullptr) {
		return false;
	}
	currentUser = user;
	addedCredit.clear();
	return true;
}
// Logs out the current user and clears session credit tracking
bool Session::logout() {
	if (!isLoggedIn()) {
		return false;
	}
	currentUser = nullptr;
	addedCredit.clear();
	return true;
}
// Validates  whether a user is currently logged into a session
bool Session::isLoggedIn() const {
	return (currentUser != nullptr);
}
// Returns the current user
User* Session::getCurrentUser() const {
	return currentUser;
}
// Gets the total amount of credit added to a users's account during the current session
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
