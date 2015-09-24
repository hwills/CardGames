//
// Deck.h
// Authors: Hunter Wills, Tung Tran
//
//
#ifndef _DECK_H_
#define _DECK_H_
//#include "stdafx.h"
#include "Card.h"
#include "Hand.h"
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>

class Hand;
class Deck {
private: 
	vector<Card> cards;
public: 
    Deck(); 
	Deck(char * filename); 
	//destructor not necessary since we only have a vector member variable which controls its own memory
	int load(char * filename); //load (read cards from file inserting into the deck cards)
	int shuffle();
    void addCard(Card);
	int size() const;
	friend ostream & operator<< (ostream &, const Deck &);
	friend int operator<< (Hand &, Deck &);
	friend int operator<< (Deck &, Deck &);
};

#endif