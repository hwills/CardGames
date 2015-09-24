//
// FiveCardDraw.h
// Authors: Hunter Wills, Tung Tran
//
//
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include <array>
#include <string>
#include "Hand.h"
//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

//
//struct Player
//
struct Player {
	string name;
	bool isAI;
	Hand hand;
	Hand downCards;
	Hand upCards;
	unsigned int win, lose;
	unsigned int chips;
	Player(string n);
	unsigned int cBet;
	bool folded;
	void deductChips(int c);
	void resetChips();
	//set
};

ostream & operator<<(ostream &o, const Player & p);
bool player_rank(const shared_ptr<Player> &, const shared_ptr<Player> &);

#endif