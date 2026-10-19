#include "Format.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>


std::string Format::trim(std::string text) {
	const std::string whitespace = " \t\r\n";
	size_t start = text.find_first_not_of(whitespace);
	if (start == std::string::npos) {
		return "";
	}
	size_t end = text.find_last_not_of(whitespace);
	return text.substr(start, end - start + 1);
}
std::string Format::toLower(std::string text) {
	std::transform(text.begin(), text.end(), text.begin(),
		[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return text;
}
std::string Format::padRight(std::string text, int width) {
	if (static_cast<int>(text.size()) >= width) {
		return text.substr(0, width);
	}
	return text + std::string(width - text.size(), ' ');
}
std::string Format::formatMoney(int cents, int width) {
	std::ostringstream money;
	money << (cents / 100) << '.' << std::setw(2) << std::setfill('0') << (cents % 100);

	std::ostringstream padded;
	padded << std::setw(width) << std::setfill('0') << money.str();
	return padded.str();
}
bool Format::parseMoney(std::string text, int& cents) {
	std::string value = trim(text);
	if (!value.empty() && value[0] == '$') {
		value = value.substr(1);
	}

	size_t point = value.find('.');
	std::string dollars = value.substr(0, point);
	std::string fraction = (point == std::string::npos) ? "" : value.substr(point + 1);

	//7 figures is a good saftey net for a upper limit
	if (dollars.empty() || dollars.size() > 7 || fraction.size() > 2) {
		return false;
	}
	if (point != std::string::npos && fraction.empty()) {
		return false;
	}
	auto isDigits = [](const std::string& s) {
		return std::all_of(s.begin(), s.end(), [](unsigned char c) { return std::isdigit(c); });
	};
	if (!isDigits(dollars) || !isDigits(fraction)) {
		return false;
	}

	int fractionCents = 0;
	if (!fraction.empty()) {
		fractionCents = std::stoi(fraction) * (fraction.size() == 1 ? 10 : 1);
	}
	cents = std::stoi(dollars) * 100 + fractionCents;
	return true;
}
