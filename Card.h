//
// Card.h
// Authors: Hunter Wills
//
//
#ifndef _CARD_H_
#define _CARD_H_
//#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
const int req_args = 4;
const int programNameIndex = 0;
const int gameNameIndex = 1;
const int firstPlayerIndex = 2;
const int req_players = 2;
const enum errors { noError, fileOpenError, cardReadError, usageError, shuffleUsageError, wrongArgsError, 
					noArgsError, manyArgsError, not_enough_cards, index_out_of_bounds, instance_not_available, 
					unknown_game, game_already_started, no_game_in_progress, already_playing};
const int numSuits = 4;
const int numRanks = 13;
const string suitStrings[numSuits] = { "C", "D", "H", "S" };
const string rankStrings[numRanks] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

//
//struct Card
//fields: rank, suit
//
struct Card	{	
	enum suit { CLUBS, DIAMONDS, HEARTS, SPADES };
	enum rank { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };
	bool operator< (const Card &) const;
	bool operator== (const Card & crd) const;
	Card & operator= (const Card &);
	Card();
	Card(int, int);
	suit cardSuit;
	rank cardRank;
};

int parse_cards(vector<Card> & cards, char * file_name);
int print_cards(const vector<Card> & cards);
void usage_message(string program_name, string message);
int print_poker_rank(vector<Card> & cards);
int rank_count(const Card * begin, const Card * end, const Card card);

#endif