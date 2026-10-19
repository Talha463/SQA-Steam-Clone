#include "FrontEnd.h"
#include "Format.h"

// Implementation of the CL interdace for the application


// Starts the main application loop, reading and proccesing user input
void FrontEnd::run() {
	out << "Steam Clone - type 'help' for a list of transactions\n";
	std::string line;
	while (running && std::getline(in, line)) {
		std::string code = Format::toLower(Format::trim(line));
		if (code.empty()) {
			continue;
		}
		processTransaction(code);
	}
}
// Prompts a user with a messages and reads their response
// Returns true if input was succesfully read, false if the stream ended or failed.
bool FrontEnd::prompt(std::string message, std::string& response) {
	out << message << '\n';
	if (!std::getline(in, response)) {
		running = false;
		return false;
	}
	response = Format::trim(response);
	return true;
}
//  Processes a given transaction code and routes it to the appropriate handler.
void FrontEnd::processTransaction(std::string code) {
	if (code == "help") {
		handleHelp();
		return;
	}
	if (code == "login") {
		if (session.isLoggedIn()) {
			out << "Error: already logged in, please logout first\n";
			return;
		}
		handleLogin();
		return;
	}

	if (!session.isLoggedIn()) {
		out << "Error: please login first\n";
		return;
	}

	if (code == "logout") {
		handleLogout();
	}
	else if (code == "create") {
		handleCreate();
	}
	else if (code == "delete") {
		handleDelete();
	}
	else if (code == "sell") {
		handleSell();
	}
	else if (code == "buy") {
		handleBuy();
	}
	else if (code == "refund") {
		handleRefund();
	}
	else if (code == "addcredit" || code == "add credit") {
		handleAddCredit();
	}
	else {
		out << "Error: unrecognized transaction\n";
	}
}
 // Displays the list of available transactions based on the user's role and login status.
void FrontEnd::handleHelp() {
	out << "Available transactions:\n";
	if (!session.isLoggedIn()) {
		out << "  login      - start a session\n";
		out << "  help       - list available transactions\n";
		return;
	}

	User* current = session.getCurrentUser();
	out << "  logout     - end the session\n";
	if (current->isAdmin()) {
		out << "  create     - add a new user\n";
		out << "  delete     - remove a user\n";
		out << "  refund     - move credit from a seller to a buyer\n";
	}
	if (current->canSell()) {
		out << "  sell       - put a game up for sale\n";
	}
	if (current->canBuy()) {
		out << "  buy        - purchase a game\n";
	}
	out << "  addcredit  - add credit to " << (current->isAdmin() ? "an account" : "your account") << '\n';
	out << "  help       - list available transactions\n";
}
 // Handles the user login process by validating credentials and loading data.
void FrontEnd::handleLogin() {
	if (!accounts.load(accountsFile)) {
		out << "Error: could not read the user accounts file\n";
		return;
	}

	std::string username;
	if (!prompt("Enter username:", username)) {
		return;
	}
	User* user = accounts.findUser(username);
	if (user == nullptr) {
		out << "Error: user does not exist\n";
		return;
	}

	if (!catalog.load(gamesFile)) {
		out << "Error: could not read the available games file\n";
		return;
	}

	session.login(user);
	log.clear();
	out << "Login successful\n";
}
// handles logout function and writes session logs to file
void FrontEnd::handleLogout() {
	log.recordEndOfSession(*session.getCurrentUser());
	if (!log.writeToFile(transactionFile)) {
		out << "Error: could not write the daily transaction file\n";
	}
	log.clear();
	session.logout();
	out << "Logout successful\n";
}
	// handles creation of new accounts (admins only)
void FrontEnd::handleCreate() {
	if (!session.getCurrentUser()->isAdmin()) {
		out << "Error: only admins can create users\n";
		return;
	}

	std::string username;
	if (!prompt("Enter new username:", username)) {
		return;
	}
	if (!User::isValidUsername(username)) {
		out << "Error: username must be between 1 and " << User::MAX_USERNAME_LENGTH << " characters\n";
		return;
	}
	if (accounts.exists(username)) {
		out << "Error: username already exists\n";
		return;
	}

	std::string typeText;
	if (!prompt("Enter user type (admin, full-standard, buy-standard, sell-standard):", typeText)) {
		return;
	}
	Permissions permissions;
	if (!User::parseType(typeText, permissions)) {
		out << "Error: invalid user type\n";
		return;
	}

	User* created = accounts.addUser(username, permissions, 0);
	log.recordCreate(*created);
	out << "User created successfully\n";
}
// Handles deletion of accounts ( Admins only aswell)
void FrontEnd::handleDelete() {
	if (!session.getCurrentUser()->isAdmin()) {
		out << "Error: only admins can delete users\n";
		return;
	}
	//TODO: cleanup associated needs to be completed
}
// Handles sellig of games and putting up listings
void FrontEnd::handleSell() {
	User* seller = session.getCurrentUser();
	if (!seller->canSell()) {
		out << "Error: your account type cannot sell games\n";
		return;
	}

	std::string name;
	if (!prompt("Enter game name:", name)) {
		return;
	}
	if (!GameListing::isValidName(name)) {
		out << "Error: game name must be between 1 and " << GameListing::MAX_NAME_LENGTH << " characters\n";
		return;
	}
	if (catalog.exists(name)) {
		out << "Error: a game with that name already exists\n";
		return;
	}

	std::string priceText;
	if (!prompt("Enter price:", priceText)) {
		return;
	}
	int price = 0;
	if (!Format::parseMoney(priceText, price) || price > GameListing::MAX_PRICE) {
		out << "Error: price must be between $0.00 and $999.99\n";
		return;
	}

	GameListing game(name, seller->getUsername(), price, false);
	catalog.addGame(game);
	log.recordSell(game);
	out << "Game listed successfully\n";
}
// handles purchasing a game from a seller
void FrontEnd::handleBuy() {
	User* buyer = session.getCurrentUser();
	if (!buyer->canBuy()) {
		out << "Error: your account type cannot buy games\n";
		return;
	}

	std::string name;
	if (!prompt("Enter game name:", name)) {
		return;
	}
	GameListing* game = catalog.findGame(name);
	if (game == nullptr) {
		out << "Error: game does not exist\n";
		return;
	}

	std::string sellerName;
	if (!prompt("Enter seller username:", sellerName)) {
		return;
	}
	if (game->getSellerUsername() != sellerName) {
		out << "Error: that seller is not selling this game\n";
		return;
	}
	User* seller = accounts.findUser(sellerName);
	if (seller == nullptr) {
		out << "Error: seller does not exist\n";
		return;
	}

	if (!game->isPurchasable()) {
		out << "Error: this game cannot be bought until the next session\n";
		return;
	}
	if (buyer->ownsGame(name)) {
		out << "Error: game is already in your collection\n";
		return;
	}
	if (!buyer->deductCredit(game->getPrice())) {
		out << "Error: insufficient credit\n";
		return;
	}
	if (!seller->addCredit(game->getPrice())) {
		buyer->addCredit(game->getPrice());
		out << "Error: seller's credit would exceed the maximum\n";
		return;
	}

	buyer->addGameToCollection(name);
	log.recordBuy(*game, *buyer);
	out << "Game purchased successfully\n";
}
// Handles giving out refunds (Admin only)
void FrontEnd::handleRefund() {
	if (!session.getCurrentUser()->isAdmin()) {
		out << "Error: only admins can issue refunds\n";
		return;
	}
	//TODO: need to complete refund
}
// handles adding crediot to a user account
void FrontEnd::handleAddCredit() {
	User* current = session.getCurrentUser();

	std::string amountText;
	if (!prompt("Enter credit amount:", amountText)) {
		return;
	}
	int amount = 0;
	if (!Format::parseMoney(amountText, amount) || amount <= 0) {
		out << "Error: invalid credit amount\n";
		return;
	}

	User* target = current;
	if (current->isAdmin()) {
		std::string username;
		if (!prompt("Enter username:", username)) {
			return;
		}
		target = accounts.findUser(username);
		if (target == nullptr) {
			out << "Error: user does not exist\n";
			return;
		}
	}
		// verify session
	if (amount > session.remainingCreditLimit(target->getUsername())) {
		out << "Error: a maximum of $1000.00 can be added to an account per session\n";
		return;
	}
	if (!target->addCredit(amount)) {
		out << "Error: account credit cannot exceed $999,999.00\n";
		return;
	}

	session.recordCreditAddition(target->getUsername(), amount);
	log.recordAddCredit(*target, amount);
	out << "Credit added successfully\n";
}
