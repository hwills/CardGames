//
// lab3.cpp
// Authors: Hunter Wills, Tung Tran
// 
//
//#include "stdafx.h"
#include "Player.h"

//
// Player::Player(string n)
// constructor
// initialize player name to be the given string passed in the constructor
// find the player record in a file named after the player
// if the player already exists, init their win and lose accordingly
// if not, init their win and lose to be 0
//
Player::Player(string n) : name(n), win(0), lose(0), chips(20), isAI(false), cBet(0), folded(false), hand(Hand()), upCards(Hand()), downCards(Hand()) {
	ifstream ifs;
	ifs.open(n);
	if (ifs.is_open()) {
		string line;
		string temp;
		int lineNumber = 0;
		while (getline(ifs, line)) {
			lineNumber++;
			switch (lineNumber) {
			case 1:
				name = line;
				break;
			case 2:
				temp = line;
				break;
			case 3:
				win = stoul(temp, nullptr, 0);
				lose = stoul(line, nullptr, 0);
				break;
			case 4:
				chips = stoul(line, nullptr, 0);
				break;
			}
		}
	}
	ifs.close();
}

ostream & operator<<(ostream &o, const Player & p) {
	o << p.name << " " << p.win << " " << p.lose;
	return o;
}

// 
// player_rank(const shared_ptr<Player> & p1, const shared_ptr<Player> & p2)
// given two pointers to two players, return true if player one has stronger
// hand than player two, false otherwise
//
bool player_rank(const shared_ptr<Player> & p1, const shared_ptr<Player> & p2)
{
	if (p1 == nullptr) {
		return false;
	}
	else if (p2 == nullptr){
		return true;
	}
	else {
		return poker_rank(p1->hand, p2->hand);
	}
}

void Player::deductChips(int c){
	chips -= c;
}

