#include "GameCatalog.h"
#include "Format.h"
#include <fstream>


//Each line is  formatted as IIIIIIIIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS_PPPPPP, ending with a game named END

// Loads game catalog from a text file
bool GameCatalog::load(std::string filename) {
	std::ifstream file(filename);
	if (!file) {
		return false;
	}

	games.clear();
	const size_t sellerStart = Format::GAME_NAME_WIDTH + 1;
	const size_t priceStart = sellerStart + Format::USERNAME_WIDTH + 1;
	std::string line;
	// extract and trim game names
	while (std::getline(file, line)) {
		std::string name = Format::trim(line.substr(0, Format::GAME_NAME_WIDTH));
		// markes the end of the catalog entries
		if (name == "END") {
			break;
		}
		// confirming the line is long enough to contain req price fields
		if (line.size() < priceStart + Format::PRICE_WIDTH) {
			continue;
		}
		// Extract seller username and parse the price amount
		std::string seller = Format::trim(line.substr(sellerStart, Format::USERNAME_WIDTH));
		int price = 0;
		if (!Format::parseMoney(line.substr(priceStart, Format::PRICE_WIDTH), price)) {
			continue;
		}
		games.emplace_back(name, seller, price, true);
	}
	return true;
}

// Finds and returns a pointer to a game listing by its name
GameListing* GameCatalog::findGame(std::string name) {
	for (auto& game : games) {
		if (game.getName() == name) {
			return &game;
		}
	}
	return nullptr;
}

// checks if a game exists in the catalog
bool GameCatalog::exists(std::string name) const {
	for (const auto& game : games) {
		if (game.getName() == name) {
			return true;
		}
	}
	return false;
} 
// adds a new game to the catalog collection
void GameCatalog::addGame(const GameListing& game) {
	games.push_back(game);
}

// returns every listing in the catalog
const std::vector<GameListing>& GameCatalog::getGames() const {
	return games;
}
