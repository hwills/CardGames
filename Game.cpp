//
// Game.cpp
// Authors: Hunter Wills, Tung Tran
// 
//
//#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include "TexasHoldEm.h"

//initialize game pointer
shared_ptr<Game> Game::game = nullptr;

//
// Game::instance()
// return a copy of the static Game::game if it is not nullptr
// throw instance_not_available error otherwise
//
shared_ptr<Game> Game::instance()
{
	if (game == nullptr){
		throw errors::instance_not_available;
	}
	else {
		return game; 
	}
}

//
// Game::start_game
// take in a string that represents the name of the game
// if the game pointer is not nullptr, throw game_already_started error
// if not, init game pointer to corresponding game (or throw unknown_game if  
// the game is not recognized)
//
void Game::start_game(const string name)
{
	const string fiveCardDrawName = "FiveCardDraw";
	const string sevenCardStudName = "SevenCardStud";
	const string texasHoldEmName = "TexasHoldEm";
	if (game == nullptr) {
		if (name == fiveCardDrawName) {
			game = make_shared<FiveCardDraw>(FiveCardDraw());
		}
		else if (name == sevenCardStudName){
			game = make_shared<SevenCardStud>(SevenCardStud());
		}
		else if (name == texasHoldEmName){
			game = make_shared<TexasHoldEm>(TexasHoldEm());
		}
		else{
			throw errors::unknown_game;
		}
	}
	else {
		throw errors::game_already_started;
	}
}

//
// Game::stop_game()
// if there is a game in progress, stop the game by making its pointer point to nullptr
// otherwise, throw no_game_in_progress error
//
void Game::stop_game()
{
	if (game == nullptr) {
		throw errors::no_game_in_progress;
	}
	else {
		game = nullptr;
	}
}

//
// Game::add_player(const string name) 
// add a player with the name passed to the function to the game
// if the player already exists, throw already_playing error
//
void Game::add_player(const string name, bool isAI){
	bool nameInUse = false;
	for (auto it = players.begin(); it < players.end(); it++){
		if (name == (*it)->name){
			nameInUse = true;
			break;
		}
	}
	if (nameInUse) {
		throw errors::already_playing;
	}
	else {
		shared_ptr<Player> player = make_shared<Player>(name);
		player->isAI = isAI;
		players.push_back(player);
	}
}

//
// Game:: find_player
// given the name of the player, go through the list of players and find him/her
// if the player is in the game, return pointer to the player, nullptr otherwise
//
shared_ptr<Player> Game::find_player(const string name)
{
	for (auto it = players.begin(); it < players.end(); it++){
		if (name == (*it)->name){
			return *it;
		}
	}
	return nullptr;
}

//
// Game:: remove_player(const string name)
// given a name of a player, go through list of players in the game and remove 
// that player
//
void Game::remove_player(const string name)
{
	for (auto it = players.begin(); it < players.end(); it++){
		if (name == (*it)->name){
			players.erase(it);
			break;
		}
	}
}

//
// Game:: bet()
// this is the betting mechanism used in every poker game
// players will keep betting until an agreed bet is made or players run out of chips
//
void Game::bet(){
	unsigned int currentBet = 0;
	unsigned int roundPot = 0;
	for (auto it = players.begin(); it < players.end(); it++){
		(*it)->cBet = 0;
	}
	auto it = players.begin();
	auto lastPlayer = players.end();
	for (auto it = players.begin(); it != lastPlayer ; it++) {
		if (it == players.end()) {
			it = players.begin();
		}
		cout << endl;
		printPlayerCards((*it)->name);
		cout << "total chips= " << (*it)->chips << " current bet: " << currentBet << " to call: " << currentBet - (*it)->cBet << endl;
		bool validResponse = false;
		int nothing = 0;
		int oneChip = 1;
		int twoChips = 2;
		int foldIndex = 7;
		if (!(*it)->folded && (*it)->chips != nothing && currentBet == nothing) {
			while (!validResponse) {
				cout << "Bet 1/2 Check 0?" << endl;
				string response;
				getline(cin, response);
				istringstream iss(response);
				string word;
				iss >> word;
				int singleChar = 1;
				int firstIndex = 0;
				if (word.size() == singleChar && isdigit(word[firstIndex])){
					size_t num = std::stoul(word, nullptr, 0);
					
					if (num == oneChip || num == twoChips) {
						if (num <= (*it)->chips) {
							(*it)->deductChips(num);
							roundPot += num;
							if (it == players.begin()){
								lastPlayer = players.end();
							}
							else{
								lastPlayer = it;
							}
							currentBet = num;
							(*it)->cBet = num;
							validResponse = true;
						}
					}
					else if (num == nothing) {
						validResponse = true;
					}
				}
			}
		}

		else if (!(*it)->folded && (*it)->chips != nothing) {
			while (!validResponse) {
				cout << "Raise 1/2 Call 0 Fold 7?" << endl;
				string response;
				getline(cin, response);
				istringstream iss(response);
				string word;
				iss >> word;
				int singleChar = 1;
				int firstIndex = 0;
				if (word.size() == singleChar && isdigit(word[firstIndex])){
					size_t num = std::stoul(word, nullptr, 0);

					if (num == oneChip || num == twoChips) {
						unsigned int raiseAmount = num + currentBet - (*it)->cBet;
						if (raiseAmount <= (*it)->chips) {
							(*it)->deductChips(raiseAmount);
							roundPot += raiseAmount;
							if (it == players.begin()){
								lastPlayer = players.end();
							}
							else{
								lastPlayer = it;
							}							
							(*it)->cBet += raiseAmount;
							currentBet += num;
							validResponse = true;
						}
						else {
							validResponse = false;
							cout << "You don't have enough chips to raise" << endl;
						}
					}
					else if (num == nothing) {
						unsigned int callAmount = currentBet - (*it)->cBet;
						if (callAmount <= (*it)->chips) {
							(*it)->deductChips(callAmount);
							roundPot += callAmount;
							(*it)->cBet += callAmount;
							validResponse = true;
						}
						else{
							unsigned int numChips = (*it)->chips;
							(*it)->deductChips(numChips);
							roundPot += numChips;
							(*it)->cBet += (*it)->chips;
							validResponse = true;
						}
					}
					else if (num == foldIndex){
						(*it)->folded = true;
						validResponse = true;
					}
				}
			}
		}
		else if (!(*it)->folded){
			cout << "you no longer have enough chips to bet, enjoy watching :)" << endl;
		}
	}
	totalPot += roundPot;
}

int Game::before_turn(Player & player){
	return 0;
}
int Game::turn(Player & player){
	return 0;
}
int Game::after_turn(Player & player){
	return 0;
}
int Game::before_round(){
	return 0;
}
int Game::round(){
	return 0;
}
int Game::after_round(){
	return 0;
}
int Game::numPlayers(){
	return players.size();
}

//
// Game:: allButOneFolded()
// check whether all players except one have folded 
//
bool Game::allButOneFolded(){
	shared_ptr<Player> tmp = nullptr;
	int count = 0;
	for (auto it = players.begin(); it < players.end(); it++){
		if (!(*it)->folded){
			tmp = (*it);
			count++;
		}
	}

	return count == 1;
}

//
// Game:: resetChips()
// Go around the table and see if any player runs out of chips and ask them to reset chips
// to 20, if they deny, then they must quit the game
//
void Game::resetChips() {
	vector<shared_ptr<Player>> tmpPlayers(players);
	for (auto it = tmpPlayers.begin(); it < tmpPlayers.end(); it++){
		int noChips = 0;
		int startingChips = 20;
		if ((*it)->chips == noChips) {
			bool validResponse = false;
			while (!validResponse) {
				cout << (*it)->name <<": Do you want to reset your chips? yes/no" << endl;
				string response;
				getline(cin, response);
				string saidNo = "no";
				string saidYes = "yes";
				if (response == saidNo) {
					//kick
					ofstream ofs;
					ofs.open((*it)->name);
					ofs << (*it)->name << endl;
					ofs << (*it)->win << endl;
					ofs << (*it)->lose << endl;
					ofs << (*it)->chips << endl;
					ofs.close();
					validResponse = true;
					remove_player((*it)->name);
				}
				else if (response == saidYes) {
					//reset
					validResponse = true;
					(*it)->chips = startingChips;
				}
				else {
					validResponse = false;
				}
			}
		}
	}
	int lastPlayer = 1;
	if (players.size() == lastPlayer){
		int playerIndex = 0;
		shared_ptr<Player> p = players[playerIndex];
		ofstream ofs;
		ofs.open(p->name);
		ofs << p->name << endl;
		ofs << p->win << endl;
		ofs << p->lose << endl;
		ofs << p->chips << endl;
		ofs.close();
		remove_player(p->name);
	}
}

//
// Game:: collectAnte()
// Go around the table and collect an ante of 1 from each player
//
void Game::collectAnte() {
	for (auto it = players.begin(); it < players.end(); it++){
		(*it)->chips--;
		totalPot++;
	}
}

//
// Game:: printPlayerCards
// By default, method will print the hand of the player with a given name
// This method will get override accordingly by the game playing
//
void Game::printPlayerCards(string name){
	auto player = find_player(name);
	cout << "Current Player: " << name << endl;
	cout << "Your Hand: " << (player)->hand;
}
