//
// TexasHoldEm.cpp
// Authors: Hunter Wills, Tung Tran
// 
//

//#include "stdafx.h"
#include "TexasHoldEm.h"
#include "Card.h"
#include <set>

using namespace std;
//
// TexasHoldEm::TexasHoldEm()
// default constructor
// initialize dealer_index to be the first player and push 52 cards to the main_deck
//
TexasHoldEm::TexasHoldEm() : dealer_index(0)
{
	int i;
	for (i = 0; i < numRanks; i++){
		int j;
		for (j = 0; j < numSuits; j++) {
			Card card((Card::rank)i, (Card::suit)j);
			main_deck.addCard(card);
		}
	}
}

//
// TexasHoldEm::before_turn(Player & player)
// do nothing, return no errors
//
int TexasHoldEm::before_turn(Player & player){
	return errors::noError;
}

//
// TexasHoldEm::turn(Player & player)
// do nothing, return no errors
//
int TexasHoldEm::turn(Player &player){
	return errors::noError;
}

//
// TexasHoldEm::after_turn(Player & player)
// print out players' name and their hands after a turn
//
int TexasHoldEm::after_turn(Player & player) {
	cout << player.name << endl;
	cout << player.hand << endl;
	return errors::noError;
}

//
// TexasHoldEm::before_round()
// collect an ante of 1
// shuffle the main_deck and deal cards to players 
// each player has 2 cards
//
int TexasHoldEm::before_round()
{
	cout << endl;
	cout << "The round is about to begin! Prepare to play some Texas Hold 'Em!" << endl;
	totalPot = 0;
	cout << "Collecting Ante" << endl;
	collectAnte();
	main_deck.shuffle();
	int nextPlayer = 1;
	const unsigned int downCardsPerPlayer = 2;
	int dealToIndex = dealer_index;
	if (players.size()*downCardsPerPlayer > main_deck.size()){
		throw errors::not_enough_cards;
	}
	for (unsigned int j = 0; j < players.size(); ++j)
	{
		dealToIndex = (dealToIndex + nextPlayer) % players.size();
		while ((*players[dealToIndex]).downCards.size() < downCardsPerPlayer)
		{
			players[dealToIndex]->downCards << main_deck;
		}
	}

	for (auto it = players.begin(); it < players.end(); it++){
		(*it)->folded = false;
	}
	bet();

	if (!allButOneFolded()){
		unsigned int i = 0;
		dealToIndex = dealer_index;
		while (i < players.size())
		{
			dealToIndex = (dealToIndex + nextPlayer) % players.size();
			before_turn(*players[dealToIndex]);
			i++;
		}
	}
	return errors::noError;
}

//
// TexasHoldEm::round() 
// loop through the list of players and call after_turn on each of them
//
int TexasHoldEm::round()
{
	//deal the first 3 community cards
	int flopCards = 3;
	for (auto j = 0; j < flopCards; j++){
		if (!allButOneFolded()){
			sharedCards << main_deck;
		}
	}
	bet();

	//deal the last 2 community cards
	int turnAndRiverCards = 2;
	for (auto j = 0; j < flopCards; j++){
		if (!allButOneFolded()){
			sharedCards << main_deck;
			bet();
		}
	}
	return errors::noError;
}

//
// TexasHoldEm::after_round()
// sort and print out players by their hand strength (strongest to weakest)
// retrieve the cards from players for next game
// letting players to drop out of the game and other players to join
//
int TexasHoldEm::after_round()
{
	cout << endl;
	cout << "Here is what each player had: " << endl;

	for (auto it = players.begin(); it < players.end(); it++){
		cout << (*it)->name << "'s Down Cards: " << (*it)->downCards << endl;
	}

	cout << "And here are the shared cards: " << sharedCards << endl;

	//put up and down cards together into the hand of each player
	for (auto it = players.begin(); it < players.end(); it++){
		for (int i = 0; i < (*it)->downCards.size(); i++){
			(*it)->hand.add_card((*it)->downCards[i]);
		}
		for (int i = 0; i < sharedCards.size(); i++){
			(*it)->hand.add_card(sharedCards[i]);
		}
	}

	cout << "The round has come to a close here are the post round stats: " << endl;

	//make each player's hand their best of 5
	for (auto it = players.begin(); it < players.end(); it++){
		Hand tmpHand((*it)->hand);
		vector<Hand> possibleHands;
		int handOffset = 1;
		for (int i = 0; i < tmpHand.size() - handOffset; i++){
			Hand tmpHand2((*it)->hand);
			tmpHand2.remove_card(i);
			for (int j = 0; j < tmpHand2.size(); j++){
				Hand tmpHand3(tmpHand2);
				tmpHand3.remove_card(j);
				possibleHands.push_back(tmpHand3);
			}
		}
		int firstCardIndex = 0;
		sort(possibleHands.begin(), possibleHands.end(), poker_rank);
		(*it)->hand = Hand(possibleHands[firstCardIndex]);
	}

	const int empty = 0;
	vector<shared_ptr<Player>> tmpPlayers;
	for (auto it = players.begin(); it != players.end(); it++){
		if (!(*it)->folded){
			tmpPlayers.push_back(*it);
		}
	}
	sort(tmpPlayers.begin(), tmpPlayers.end(), player_rank);
	for (auto it = tmpPlayers.begin(); it != tmpPlayers.end(); ++it)
	{
		if (poker_rank((*it)->hand, (*tmpPlayers.begin())->hand) == poker_rank((*tmpPlayers.begin())->hand, (*it)->hand))
		{
			(*it)->win++;
			(*it)->chips += totalPot;
		}
		else
		{
			(*it)->lose++;
		}
		cout << *(*it) << " current chips: " << (*it)->chips << endl;
		cout << (*it)->hand << endl;
	}
	for (auto it = players.begin(); it != players.end(); it++){
		if ((*it)->folded){
			(*it)->lose++;
			cout << *(*it) << " (FOLDED)" << " current chips: " << (*it)->chips << endl;
		}
	}

	for (auto it = players.begin(); it != players.end(); ++it)
	{
		for (auto i = 0; i < (*it)->downCards.size(); ++i)
		{
			main_deck.addCard((*it)->downCards[i]);
		}
		(*it)->hand = Hand();
		(*it)->downCards = Hand();
	}

	while (discard_deck.size() != empty)
	{
		main_deck << discard_deck;
	}

	for (auto i = 0; i < sharedCards.size(); ++i)
	{
		main_deck.addCard(sharedCards[i]);
	}
	sharedCards = Hand();

	string response = "";
	string saidNo = "no";
	cout << "Anybody wanna call it quits?" << endl;
	getline(cin, response);
	while (response != saidNo)
	{
		istringstream iss(response);
		string word;
		while (iss >> word) {
			shared_ptr<Player> p = find_player(word);
			if (p != nullptr) {
				ofstream ofs;
				ofs.open(word);
				ofs << p->name << endl;
				ofs << p->win << endl;
				ofs << p->lose << endl;
				ofs << p->chips << endl;
				ofs.close();
				remove_player(word);
			}
		}
		cout << "Anybody wanna call it quits?" << endl;
		getline(cin, response);
	}

	string response2 = "";
	cout << "Anybody wanna join?" << endl;
	getline(cin, response2);
	while (response2 != saidNo)
	{
		istringstream iss2(response2);
		string word;
		const int AIOffset = 1;
		const int beginIndex = 0;
		while (iss2 >> word) {
			try{
				bool isAI = false;
				add_player(word, isAI);
			}
			catch (...){
				throw;
			}
		}
		cout << "Anybody wanna join?" << endl;
		getline(cin, response2);
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
	int nextPlayer = 1;
	if (players.size() != empty)
	{
		dealer_index = (dealer_index + nextPlayer) % players.size();
	}
	return errors::noError;
}

//
// SevenCardStud:: printPlayerCards
// Print the community cards
//
void TexasHoldEm::printPlayerCards(string name){
	cout << "Shared Cards Between All Players : " << sharedCards << endl;
	for (auto it = players.begin(); it < players.end(); it++){
		if ((*it)->name == name){
			cout << "Your Hand (" << (*it)->name << "): " << (*it)->downCards << endl;
		}
		else{
			cout << (*it)->name << "'s Hand:";
			for (int i = 0; i < (*it)->downCards.size(); i++){
				cout << " *";
			}
			cout << endl;
		}
	}
}
