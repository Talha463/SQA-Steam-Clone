#include "Session.h"


bool Session::login(User* user) {
	if (isLoggedIn()) {
		return false;
	}
	currentUser = user;
	return true;
}
bool Session::logout() {
	if (!isLoggedIn()) {
		return false;
	}

	// Add transaction log

	currentUser = nullptr;
	return true;
}
bool Session::isLoggedIn() const {
	return (currentUser != nullptr);
}
User* Session::getCurrentUser() const {
	return currentUser;
}
int Session::creditAdded() const {
	return addedCredit;
}
void Session::recordCreditAddition(int amount) {
	//create log
	addedCredit += amount;
}
int Session::remainingCreditLimit() const {
	return 100000 - addedCredit; //credit limit is $1000.00
}