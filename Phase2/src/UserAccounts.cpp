#include "UserAccounts.h"
#include "Format.h"
#include <fstream>


//Each line is UUUUUUUUUUUUUUU_TT_CCCCCCCCC, ending with a user named END.
bool UserAccounts::load(std::string filename) {
	std::ifstream file(filename);
	if (!file) {
		return false;
	}

	users.clear();
	std::string line;
	while (std::getline(file, line)) {
		std::string username = Format::trim(line.substr(0, Format::USERNAME_WIDTH));
		if (username == "END") {
			break;
		}
		if (line.size() < 28) {
			continue;	//Skip malformed lines.
		}

		Permissions permissions;
		int credit = 0;
		if (!User::parseType(line.substr(16, Format::TYPE_WIDTH), permissions)
			|| !Format::parseMoney(line.substr(19, Format::CREDIT_WIDTH), credit)) {
			continue;
		}
		addUser(username, permissions, credit);
	}
	return true;
}
// Finds and returns a pointer to a user by their username.
User* UserAccounts::findUser(std::string username) const {
	for (const auto& user : users) {
		if (user->getUsername() == username) {
			return user.get();
		}
	}
	return nullptr;
}
// Checks if a user with the specific username exists
bool UserAccounts::exists(std::string username) const {
	return (findUser(username) != nullptr);
}
// adds a new user account to the collection with specified permissions and credits
User* UserAccounts::addUser(std::string username, Permissions permissions, int credit) {
	users.push_back(std::make_unique<User>(username, permissions, credit));
	return users.back().get();
}
