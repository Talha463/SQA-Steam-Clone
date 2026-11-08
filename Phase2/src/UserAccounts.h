#pragma once
#include <memory>
#include <string>
#include <vector>
#include "User.h"

//The set of user accounts read from the current user accounts file.
class UserAccounts {
private:
	std::vector<std::unique_ptr<User>> users;	//unique_ptr keeps User* stable as accounts are added.

public:
	bool load(std::string filename);	//Replaces all accounts with the file's contents.
	bool loadCollection(std::string filename);	//Assigns owned games to users. Lines are UUUUUUUUUUUUUUU_IIIIIIIIIIIIIIIIIIIIIIIII, ending with END.
	User* findUser(std::string username) const;
	bool exists(std::string username) const;
	User* addUser(std::string username, Permissions permissions, int credit);
	const std::vector<std::unique_ptr<User>>& getUsers() const;	//All accounts, used by search
};
