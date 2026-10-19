#pragma once
#include <string>

//Helpers for reading and writing fields, used by the data file
namespace Format {
	constexpr int USERNAME_WIDTH = 15;
	constexpr int TYPE_WIDTH = 2;
	constexpr int CREDIT_WIDTH = 9;
	constexpr int GAME_NAME_WIDTH = 25;
	constexpr int PRICE_WIDTH = 6;

	std::string trim(std::string text);
	std::string toLower(std::string text);
	std::string padRight(std::string text, int width);
	std::string formatMoney(int cents, int width);
	bool parseMoney(std::string text, int& cents);
}
