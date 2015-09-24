//
// SevenCardStud.cpp
// Authors: Hunter Wills, Tung Tran
// 
//

//#include "stdafx.h"
#include "SevenCardStud.h"
#include "Card.h"
#include <set>

using namespace std;

//
// SevenCardStud::SevenCardStud()
// default constructor
// initialize dealer_index to be the first player and push 52 cards to the main_deck
//
SevenCardStud::SevenCardStud() : dealer_index(0)
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
// SevenCardStud::before_turn(Player & player)
// do nothing here
//
int SevenCardStud::before_turn(Player & player){
	return errors::noError;
}

//
// SevenCardStud::turn(Player & player)
// do nothing here
//
int SevenCardStud::turn(Player &player){
	return errors::noError;
}

//
// SevenCardStud::after_turn(Player & player)
// print out players' name and their hands after a turn
//
int SevenCardStud::after_turn(Player & player) {
	cout << player.name << endl;
	cout << player.hand << endl;
	return errors::noError;
}

//
// SevenCardStud::before_round()
// shuffle the main_deck and deal cards to players
// each player would have 2 down cards and 1 up card
//
int SevenCardStud::before_round()
{
	cout << endl;
	cout << "The round is about to begin! Prepare to play some Seven Card Stud!" << endl;
	totalPot = 0;
	cout << "Collecting Ante" << endl;
	collectAnte();
	main_deck.shuffle();
	int nextPlayer = 1;
	const unsigned int downCardsPerPlayer = 2;
	const unsigned int upCardsPerPlayer = 1;
	int dealToIndex = dealer_index;
	if (players.size()*downCardsPerPlayer + players.size()*upCardsPerPlayer > main_deck.size()){
		throw errors::not_enough_cards;
	}
	for (unsigned int j = 0; j < players.size(); ++j)
	{
		dealToIndex = (dealToIndex + nextPlayer) % players.size();
		while ((*players[dealToIndex]).downCards.size() < downCardsPerPlayer)
		{
			players[dealToIndex]->downCards << main_deck;
		}
		while ((*players[dealToIndex]).upCards.size() < upCardsPerPlayer)
		{
			players[dealToIndex]->upCards << main_deck;
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
// SevenCardStud::round() 
// loop through the list of players and call after_turn on each of them
// manage the game, deal cards to players in each round and ask for betting
//
int SevenCardStud::round()
{
	//deal up cards to players 3 times
	int upCardRounds = 3;
	for (auto j = 0; j < upCardRounds; j++){
		if (!allButOneFolded()){
			unsigned int nextPlayer = 1;
			unsigned int i = 0;
			int dealToIndex = dealer_index;
			auto upCardsPerPlayer = 1;
			if (players.size()*upCardsPerPlayer > main_deck.size()){
				throw errors::not_enough_cards;
			}
			while (i < players.size())
			{
				dealToIndex = (dealToIndex + nextPlayer) % players.size();
				players[dealToIndex]->upCards << main_deck;
				i++;
			}
			bet();
		}
	}

	// deal the last down cards to players
	if (!allButOneFolded()){
		unsigned int nextPlayer = 1;
		unsigned int i = 0;
		int dealToIndex = dealer_index;
		auto downCardsPerPlayer = 1;
		if (players.size()*downCardsPerPlayer > main_deck.size()){
			throw errors::not_enough_cards;
		}
		while (i < players.size())
		{
			dealToIndex = (dealToIndex + nextPlayer) % players.size();
			players[dealToIndex]->downCards << main_deck;
			i++;
		}
		bet();
	}
	return errors::noError;
}

//
// SevenCardStud::after_round()
// sort and print out players by their hand strength (strongest to weakest)
// retrieve the cards from players for next game
// letting players to drop out of the game and other players to join
//
int SevenCardStud::after_round()
{
	cout << endl;
	cout << "Here is what each player had: " << endl;
	//put up and down cards together into the hand of each player
	for (auto it = players.begin(); it < players.end(); it++){
		for (int i = 0; i < (*it)->downCards.size(); i++){
			(*it)->hand.add_card((*it)->downCards[i]);
		}
		for (int i = 0; i < (*it)->upCards.size(); i++){
			(*it)->hand.add_card((*it)->upCards[i]);
		}
	}

	for (auto it = players.begin(); it < players.end(); it++){
		cout << (*it)->name << "'s Hand: " << (*it)->hand << endl;
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
		for (auto i = 0; i < (*it)->hand.size(); ++i)
		{
			main_deck.addCard((*it)->hand[i]);
		}
		(*it)->hand = Hand();
		(*it)->upCards = Hand();
		(*it)->downCards = Hand();
	}

	while (discard_deck.size() != empty)
	{
		main_deck << discard_deck;
	}

	//ask if any player want to leave
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

	//ask if any player want to join
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
			}
		}
		cout << "Anybody wanna join?" << endl;
		getline(cin, response2);
	}

	//store players' data in case there is not enough player and the game has to end
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
// Print the hand of the player with the given name
// Print the up cards of other players
//
void SevenCardStud::printPlayerCards(string name){
	for (auto it = players.begin(); it < players.end(); it++){
		if ((*it)->name == name){
			cout << "Your Hand ("<< (*it)->name <<"): " << (*it)->downCards << " " << (*it)->upCards << endl;
		}
		else{
			cout << (*it)->name << "'s Hand: " << (*it)->upCards;
			for (int i = 0; i < (*it)->downCards.size(); i++){
				cout << " *";
			}
			cout << endl;
		}
	}
}
