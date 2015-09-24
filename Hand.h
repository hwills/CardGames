//
// Hand.h
// Authors: Hunter Wills
// 
//
#ifndef HAND_H
#define HAND_H
#include "Card.h"
//#include "stdafx.h"
#include "Deck.h"

enum poker_ranks {noRank, onePair, twoPairs, threeOfAKind, striaght, flush, fullHouse, fourOfAKind, straightFlush, INVALID};
class Deck;
class Hand {
private:
	vector<Card> cards;
public:
	Hand();
	Hand(const Hand &);
	//destructor not necessary since we only have a vector member variable which controls its own memory
	Hand & operator= (const Hand &);
	int size() const;
	bool operator== (const Hand &) const;
	bool operator< (const Hand &) const;
    Card & operator[] (const size_t);
    void remove_card(const size_t);
	void add_card(Card c);
	string asString() const;
	vector<Card> & getCards();
	friend ostream & operator<< (ostream &, const Hand &);
	friend int operator<< (Hand &, Deck &);
	friend int poker_type(const Hand &);
	friend bool poker_rank(const Hand &, const Hand &);
};

#endif
