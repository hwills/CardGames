//
// FiveCardDraw.h
// Authors: Hunter Wills, Tung Tran
//
//
#ifndef _FIVECARDDRAW_H_
#define _FIVECARDDRAW_H_
//#include "stdafx.h"
#include "Game.h"

class FiveCardDraw : public Game {
protected:
    size_t dealer_index;
    Deck discard_deck;
public:
    FiveCardDraw(); //init dealer to 0 add all standard cards to main deck
	virtual int before_turn(Player & player) override;
	virtual int turn(Player & player) override;
	virtual int after_turn(Player & player) override;
	virtual int before_round() override;
	virtual int round() override;
	virtual int after_round() override;
};



#endif
