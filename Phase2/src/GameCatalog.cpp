#include "GameCatalog.h"
#include "Format.h"
#include <fstream>


//Each line is IIIIIIIIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS_PPPPPP, ending with a game named END
bool GameCatalog::load(std::string filename) {
	std::ifstream file(filename);
	if (!file) {
		return false;
	}

	games.clear();
	const size_t sellerStart = Format::GAME_NAME_WIDTH + 1;
	const size_t priceStart = sellerStart + Format::USERNAME_WIDTH + 1;
	std::string line;
	while (std::getline(file, line)) {
		std::string name = Format::trim(line.substr(0, Format::GAME_NAME_WIDTH));
		if (name == "END") {
			break;
		}
		if (line.size() < priceStart + Format::PRICE_WIDTH) {
			continue;
		}

		std::string seller = Format::trim(line.substr(sellerStart, Format::USERNAME_WIDTH));
		int price = 0;
		if (!Format::parseMoney(line.substr(priceStart, Format::PRICE_WIDTH), price)) {
			continue;
		}
		games.emplace_back(name, seller, price, true);
	}
	return true;
}
GameListing* GameCatalog::findGame(std::string name) {
	for (auto& game : games) {
		if (game.getName() == name) {
			return &game;
		}
	}
	return nullptr;
}
bool GameCatalog::exists(std::string name) const {
	for (const auto& game : games) {
		if (game.getName() == name) {
			return true;
		}
	}
	return false;
}
void GameCatalog::addGame(const GameListing& game) {
	games.push_back(game);
}
