#pragma once
#include <string>
#include <vector>
#include "GameListing.h"

//Available games, read from file
class GameCatalog {
private:
	std::vector<GameListing> games;

public:
	bool load(std::string filename);
	GameListing* findGame(std::string name);
	bool exists(std::string name) const;
	void addGame(const GameListing& game);
};
