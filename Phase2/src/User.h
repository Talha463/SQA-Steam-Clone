#include <string>
#include <vector>

enum Permissions {
	ADMIN,
	FULL_STANDARD,
	BUY_STANDARD,
	SELL_STANDARD,
};

class User {
private:
	std::string username = "";
	Permissions level;
	int credit = 0;
	std::vector<std::string> games;

public:
	User(std::string user, Permissions permissions)
		: username(user), level(permissions) { }

	virtual ~User() = default;

	std::string getUsername() const;
	int getCredit() const;
	Permissions getPermissions() const;
	void addCredit(int amount);
	bool deductCredit(int amount);
	bool canBuy() const;
	bool canSell() const;
	bool isAdmin() const;
	bool ownsGame(std::string gameName) const;	//Checks whether the user's collection contains a game.
	void addGameToCollection(std::string gameName);	//Adds a purchased game to the collection.
};


class Admin : User {
	//overload a few functions
};