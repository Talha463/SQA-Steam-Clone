#include "User.h"

class Session {
private:
	User* currentUser = nullptr;
	int addedCredit;

public:
	bool login(User* user);
	bool logout();
	bool isLoggedIn() const;
	User* getCurrentUser() const;
	int creditAdded() const;
	void recordCreditAddition(int amount);
	int remainingCreditLimit() const;
};